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
    int fd[2];
    if(pipe(fd)<0){
        assert(0);
    }
    poller_->addFd(fd[0],NULL);
    wakedownfd_=fd[0];
    wakeupfd_=fd[1];
    
    
}

EventLoop::~EventLoop(){
}

void EventLoop::setMainFd(int mainfd){
    mainfd_=mainfd;
    quit=false;
}

void EventLoop::loop(){
    assert(mainfd_);
    notifymain('0');
    
    while(!quit){
        
        int count=poller_->wait(events_, MAXEVENT);
        
        int sockfd;
        for(int i=0;i<count;i++){
            sockfd=events_[i].fd;
            if(sockfd==wakedownfd_){
               
                wakedown();
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
            notifymain('0');
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
                //fix encode
                uint16_t len=Encode16(nowtask.size_);
                connectionMap_[nowtask.fd_]->outputBuffer_.append(&len,sizeof(len));
                connectionMap_[nowtask.fd_]->outputBuffer_.append(nowtask.message_,nowtask.size_);
                //fix jemalloc
                free(nowtask.message_);
                handleWrite(nowtask.fd_);
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
        //printf("read!\n");
        for(;;){
            //fix task*
            
            
            size_t ret=Decode16(&connectionMap_[fd]->inputBuffer_);
            if(ret!=0){
                printf("Decode success!\n");
                Task task;
                task.home_=home_;
                task.fd_=fd;
                task.message_=malloc(ret);
                memcpy(task.message_, connectionMap_[fd]->inputBuffer_.getReadPeek(), ret);
                task.size_=ret;
                connectionMap_[fd]->inputBuffer_.retrieve(ret);
                //printf("%s\n",(char*)task.message_);
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
    newfd->setTcpFd(fd);
    connectionMap_.insert(std::pair<int,TcpConnection*>(fd,newfd));
}


// '0'  the task queue is null

void EventLoop::notifymain(char opt){
    char x=opt;
    for(;;){
        ssize_t count=write(mainfd_, &opt, sizeof opt);
        if(count<0){
            switch (errno) {
                case EINTR:
                    errno=0;
                    continue ;
            }
            errno=0;
            printf("EventLoop::wakeup error");
            return ;
        }
        return ;
    }
}

void EventLoop::wakeup(char opt){
    char one=opt;
    
    for(;;){
        ssize_t count=write(wakeupfd_, &one, sizeof one);
        if(count<0){
            switch (errno) {
                case EINTR:
                    errno=0;
                    continue ;
            }
            errno=0;
            printf("EventLoop::wakeup error");
            return ;
        }
        return ;
    }
}

//'0' is wakeup

void EventLoop::wakedown(){
    char one;
    ssize_t count=read(wakedownfd_, &one, sizeof one);
    if(count<0){
        printf("EventLoop::wakedown error");
        return;
    }
}


