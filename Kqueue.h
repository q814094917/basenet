//
//  Kqueue.h
//  reactor
//
//  Created by 高聪 on 16/7/19.
//  Copyright (c) 2016年 高聪. All rights reserved.
//

#ifndef reactor_Kqueue_h
#define reactor_Kqueue_h

namespace reactor {
    typedef int eventf;
    typedef struct poll_event{
        int fd;
        void* ptr;
        eventf flag;
    }poll_event;
    
#define MAXEVENT 64

    
    class Poller{
    private:
        int pollfd_;
    public:
        Poller();
        ~Poller();
        int getPollFd();
        int addFd(int fd,void*ud);
        void modFd(int fd,eventf events,void* ud);
        void delFd(int fd);
        int wait(poll_event* events,int max);
    };
}


#endif
