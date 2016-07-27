//
//  EventLoop.h
//  reactor
//
//  Created by 高聪 on 16/7/19.
//  Copyright (c) 2016年 高聪. All rights reserved.
//

#ifndef __reactor__EventLoop__
#define __reactor__EventLoop__

#include <stdio.h>
#include <map>
#include <vector>
#include <queue>
#include "Declear.h"
#include "Poller.h"
#include "ThreadMutex.h"
#include "Task.h"
#include "Notify.h"
#include "ObjectPool.h"


#include "ServerLoop.h"

namespace reactor{
    class EventLoop{
    private:
        typedef std::map<int,TcpConnection*> ConnectionMap;
        
        std::shared_ptr<ServerLoop> ServerLoopBind_;
        int home_;
        
        std::shared_ptr<Poller> poller_;  //所拥有的epoll循环
        poll_event events_[64];  //获取epoll的就绪事件
        
        ConnectionMap connectionMap_;
        ObjectPool<TcpConnection> connpool_;
        
        
        bool quit;
        
        std::queue<Task> taskqueue_;   //If its null send null message to serverfd_;
                                        //In loop we evidance;
        std::queue<Task> messagequeue_; //If mainmessagequeue is null send message to thisfd_;
        int mainfd_;
        
        Notify notievent_;
        
    public:
        EventLoop(ServerLoop* mainloop,int home,int size=4096);
        ~EventLoop();
        void setMainFd(int mainfd);
        void loop();
        
        void handleRead(int fd);
        void handleWrite(int fd);
        void handleClose(int fd);
        
        
        
        void doTask();
        void newconnection(int fd);
        
        void swapTask(std::queue<Task>& taskqueue);
        
        
    
    };
    
    
}

#endif /* defined(__reactor__EventLoop__) */
