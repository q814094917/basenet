//
//  Thread.h
//  reactor
//
//  Created by 高聪 on 16/7/21.
//  Copyright (c) 2016年 高聪. All rights reserved.
//

#ifndef __reactor__Thread__
#define __reactor__Thread__

#include <stdio.h>
#include "Define.h"
#include <sys/types.h>
namespace reactor{
    class Thread
    {
    private:
        ThreadTask task_;
        void* param_;
        bool started_;
        bool joined_;
        pthread_t pthreadId_;
    public:
        Thread(ThreadTask& task,void* param);
        ~Thread();
        void start();
        void stop();
        static pid_t gettid();
        
    };
    
}
#endif /* defined(__reactor__Thread__) */
