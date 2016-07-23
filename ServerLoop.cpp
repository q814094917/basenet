//
//  ServerLoop.cpp
//  reactor
//
//  Created by 高聪 on 16/7/20.
//  Copyright (c) 2016年 高聪. All rights reserved.
//

#include "ServerLoop.h"
#include "Thread.h"
#include "Task.h"
#include <unistd.h>
#include <errno.h>
using namespace reactor;

ServerLoop::ServerLoop()
:eventindex_(0)
,listening(false)
,notifytask_(false)
{
    
    
    
}

ServerLoop::~ServerLoop(){
    
    stop();
    
}

void ServerLoop::addAcceptor(std::string ip, uint16_t port){
    
    address addr;
    addr.ip=ip;
    addr.port=port;
    acceptors_.push_back(addr);
    listening=true;
    
}

void ServerLoop::start(){
    assert(listening);
    ThreadTask atask=auxiliaryfunc;
    auxiliary_=new Thread(atask,this);
    auxiliary_->start();
    
    for(int i=0;i<THREADSIZE;i++){
        ThreadTask newtask=threadfunc;
        threadPool_[i]=new Thread(newtask,this);
        threadPool_[i]->start();
    }
    
    
}

Task ServerLoop::getMessage(){
    return messagequeue_.take();
}

void ServerLoop::stop(){
    auxiliary_->stop();
    delete auxiliary_;
    
    for(int i=0;i<THREADSIZE;i++){
        threadPool_[i]->stop();
        delete EventLoops_[i];
        delete threadPool_[i];
    }
}



void* ServerLoop::threadfunc(void* a){

    ServerLoop* thisloop=(ServerLoop*)a;
    int thisindex=thisloop->dispatchindex();
    
    thisloop->EventLoops_[thisindex]=new EventLoop(thisloop,thisindex);
    //fix
    thisloop->EventLoops_[thisindex]->setMainFd(thisloop->tasknoti[thisindex].getSendfd());

    thisloop->EventLoops_[thisindex]->loop();
    
 
    return NULL;
}

//NULL is accept
//NOT NULL is child

#define MAXADDRESS 4

void* ServerLoop::auxiliaryfunc(void* a){
    //fix select
    Poller poll;
    poll_event events[32];
    ServerLoop* thisloop=(ServerLoop*)a;
    std::map<int,int> acceptormap;
    std::map<int,int> eventmap;
    std::map<int,bool> needtaskindex;
    int dispatchindex=0;
    //fix
    TcpAcceptor accs[MAXADDRESS];
    assert(thisloop->acceptors_.size()<=MAXADDRESS);
    for(int i=0;i<thisloop->acceptors_.size();i++){
        
        address addr=thisloop->acceptors_[i];
        accs[i].bindAddr(addr.ip,addr.port);
        accs[i].listen();
        acceptormap[accs[i].getfd()]=i;
        poll.addFd(accs[i].getfd(), NULL);
    }
    
    
    for(int i=0;i<THREADSIZE+1;i++){
        poll.addFd(thisloop->tasknoti[i].getRecvfd(),a);
        
        eventmap[thisloop->tasknoti[i].getRecvfd()]=i;
        needtaskindex[i]=false;
    }
    
    while(1){
        int count=poll.wait(events, 32);
        for(int i=0;i<count;i++){
            if(events[i].fd==thisloop->sernoti.getRecvfd()){
                thisloop->sernoti.disnotify();
                
                continue;
            }
            
            if(events[i].ptr==NULL){
                //dispatch average
                int conn=accs[acceptormap[events[i].fd]].accept();
                if(conn>0){
                Task acceptask;
                acceptask.fd_=conn;
                acceptask.option_=RADD;
                acceptask.home_=dispatchindex%THREADSIZE;
                dispatchindex=(dispatchindex+1)%THREADSIZE;
                thisloop->taskPut(acceptask);
                printf("taskqueue%d size=%d",acceptask.home_,thisloop->childtaskqueue_[acceptask.home_].size());
                //printf("accept%d\n",acceptask.home_);
                }else{
                    
                    printf("%d====%d",accs[0].getfd(),events[i].fd);
                }
                
            }else{
                int index=eventmap[events[i].fd];
                ;
               
                needtaskindex[index]=true;
                    
                
                Notify::disnotifyFd(events[i].fd);
                
                
                
            }
            
        }
        
        for(int i=0;i<THREADSIZE;i++){
            if(needtaskindex[i]&&(!thisloop->childtaskqueue_[i].empty())){
                thisloop->taskTake(i);
                needtaskindex[i]=false;
                
            }
        }
        
        thisloop->notifytask_=false;
        
        
    }
    

    return NULL;
}

int ServerLoop::dispatchindex(){
    ThreadGuard lock(mutex_);
    eventindex_+=1;
    return eventindex_-1;
}

void ServerLoop::eventPutMessage(std::queue<Task>&q){
    messagequeue_.putqueue(q);
}






void ServerLoop::taskPut(Task&q){
    ThreadMutex lock(mutex_);
    childtaskqueue_[q.home_].push(q);
    if(!notifytask_){
        notifytask_=true;
        sernoti.notify();
    }
    
}



void ServerLoop::taskTake(int home){
    ThreadMutex lock(mutex_);
    
    EventLoops_[home]->swapTask(childtaskqueue_[home]);
    
}