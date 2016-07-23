//
//  EventLoop.cpp
//  reactor
//
//  Created by 高聪 on 16/7/19.
//  Copyright (c) 2016年 高聪. All rights reserved.
//

#include "EventLoop.h"
#include "Poller.h"
#include "Define.h"
#include "TcpConnection.h"
#include "Encode.h"
#include "Decode.h"

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
using namespace reactor;
EventLoop::EventLoop(ServerLoop* mainloop,int home,int size)
    :poller_(new Poller)
    ,ServerLoopBind_(mainloop)
    ,home_(home)
{
    mainfd_=0;
    
    poller_->addFd(notievent_.getRecvfd(),NULL);
    
    
    
}

EventLoop::~EventLoop(){
}

void EventLoop::setMainFd(int mainfd){
    mainfd_=mainfd;
    quit=false;
}

void EventLoop::loop(){
    assert(mainfd_);
    notievent_.notifyFd(mainfd_);
    
    while(!quit){
        
        int count=poller_->wait(events_, MAXEVENT);
        
        int sockfd;
        for(int i=0;i<count;i++){
            sockfd=events_[i].fd;
            if(sockfd==notievent_.getRecvfd()){
                notievent_.disnotify();
            }else{
                if(events_[i].flag&MPOLL_IN){
                    handleRead(sockfd);
                    
                }else if(events_[i].flag&MPOLL_OUT){
                    handleWrite(sockfd);
                    
                }else{
                    handleClose(sockfd);
                    
                }
                
            }
        }
        
        if(!taskqueue_.empty()){
            doTask();
            //'0': taskqueue is NULL
            notievent_.notifyFd(mainfd_);
            //when null notify  dont need mutex
            //just signal
        }
        
        if(!messagequeue_.empty()){
            
            ServerLoopBind_->eventPutMessage(messagequeue_);
            
        }
        
        
    }
    
}


void EventLoop::doTask(){
    while(!taskqueue_.empty()){
        Task nowtask=taskqueue_.front();
        taskqueue_.pop();
        
        switch(nowtask.option_){
            case RADD:
                newconnection(nowtask.fd_);
                break;
            case RRECV:
                handleRead(nowtask.fd_);
                break;
            
            case RSEND:
            {
                if(connectionMap_.find(nowtask.fd_)!=connectionMap_.end()){
                //fix encode
                uint16_t len=Encode16(nowtask.size_);
                connectionMap_[nowtask.fd_]->outputBuffer_.append(&len,sizeof(len));
                connectionMap_[nowtask.fd_]->outputBuffer_.append(nowtask.message_,nowtask.size_);
                //fix jemalloc
                free(nowtask.message_);
                handleWrite(nowtask.fd_);
                }else{
                    free(nowtask.message_);
                }
            }
                break;
            case RDEL:
                handleClose(nowtask.fd_);
                break;
            case RLOCAL:
                messagequeue_.push(nowtask);
                break;
                
                
            
        }
    }
    
    assert(taskqueue_.empty());
    
}

void EventLoop::handleRead(int fd){
    //fix check
    if(connectionMap_.find(fd)!=connectionMap_.end()){
    int ret=connectionMap_[fd]->recv();
    if(ret<0){
        //printf("EventLoop read ERROR!\n");
        handleClose(fd);
    }else{
        
        for(;;){
            //fix task*
            
            
            size_t ret=Decode16(&connectionMap_[fd]->inputBuffer_);
            if(ret!=0){
                Task task;
                task.home_=home_;
                task.fd_=fd;
                task.message_=malloc(ret);
                memcpy(task.message_, connectionMap_[fd]->inputBuffer_.getReadPeek(), ret);
                task.size_=ret;
                connectionMap_[fd]->inputBuffer_.retrieve(ret);
                messagequeue_.push(task);
                
            }else{
                break;
            }
        }
    }
    }
}

void EventLoop::handleWrite(int fd){
    //fix check
    if(connectionMap_.find(fd)!=connectionMap_.end()){
    int ret=connectionMap_[fd]->send();
    if(ret<0){
        printf("EventLoop write ERROR!\n");
        handleClose(fd);
    }
    }

}

//fix NEW AND DELETE
//fix CLOSE MESSAGE 
void EventLoop::handleClose(int fd){
    
    
    poller_->delFd(fd);
    close(fd);
    
    
    delete connectionMap_[fd];
    connectionMap_.erase(fd);
    
}

void EventLoop::newconnection(int fd){
    poller_->addFd(fd,NULL);
    TcpConnection* newfd=new TcpConnection(fd,this->poller_);
    connectionMap_.insert(std::pair<int,TcpConnection*>(fd,newfd));
}

void EventLoop::swapTask(std::queue<Task>& taskqueue){
    
    std::swap(taskqueue_,taskqueue);
    notievent_.notify();
}





