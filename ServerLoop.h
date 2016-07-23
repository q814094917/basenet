//
//  ServerLoop.h
//  reactor
//
//  Created by 高聪 on 16/7/20.
//  Copyright (c) 2016年 高聪. All rights reserved.
//

#ifndef __reactor__ServerLoop__
#define __reactor__ServerLoop__

#include <stdio.h>
#include "Declear.h"
#include "Task.h"
#include "TcpAcceptor.h"
#include "EventLoop.h"
#include "ThreadMutex.h"
#include "BlockingQueue.h"
#include <map>
#include <vector>


#define THREADSIZE 4
namespace reactor {
    typedef  Thread*    ThreadPtr;
    typedef  EventLoop* EventLoopPtr;
    
    typedef struct address{
        std::string ip;
        uint16_t port;
    }address;
    
    class ServerLoop{
    private:
        
        
             //dispatch
        BlockingQueue<Task> messagequeue_;
        std::queue<Task> childtaskqueue_[THREADSIZE];
        
        ThreadPtr threadPool_[THREADSIZE];
        ThreadPtr auxiliary_;
        
        int eventindex_;
        ThreadMutex mutex_;
        
        bool listening;
        
    public:
        bool notifytask_;
        int mainfd_[THREADSIZE+1];    //send message
        int recvfd_[THREADSIZE+1];    //recv message
        std::vector<address> acceptors_; //neednt
        EventLoopPtr EventLoops_[THREADSIZE];
        
        ServerLoop();
        ~ServerLoop();
        void start();       //main thread and start other threads
        void stop();
        Task getMessage();
        int dispatchindex();      //init
        void addAcceptor(std::string ip,uint16_t port);     //init
        
        void eventPutMessage(std::queue<Task> &q);
        
        void taskPut(Task &q);
        void taskTake(int home);      //block
        
        static void wakeup(int fd);
        static void wakedown(int fd);
        
        static void* threadfunc(void*);       //eventloop
        static void* auxiliaryfunc(void*);    //acceptor and notify
    };
}

#endif /* defined(__reactor__ServerLoop__) */
