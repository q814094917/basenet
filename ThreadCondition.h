//
//  ThreadCondition.h
//  reactor
//
//  Created by 高聪 on 16/7/21.
//  Copyright (c) 2016年 高聪. All rights reserved.
//

#ifndef __reactor__ThreadCondition__
#define __reactor__ThreadCondition__

#include <stdio.h>
#include "ThreadMutex.h"
#include <pthread.h>
namespace reactor{
    class ThreadCondition{
    private:
        ThreadMutex& mutex_;
        pthread_cond_t condid_;
    public:
        ThreadCondition(ThreadMutex& mutex)
        :mutex_(mutex)
        {
            pthread_cond_init(&condid_, NULL);
        }
        ~ThreadCondition()
        {
            pthread_cond_destroy(&condid_);
        }
        void wait()
        {
            pthread_cond_wait(&condid_, mutex_.getPhtreadMutex());
            
        }
        void notify()
        {
            pthread_cond_signal(&condid_);
        }
        void notifyAll()
        {
            pthread_cond_broadcast(&condid_);
        }
    };

}
#endif /* defined(__reactor__ThreadCondition__) */
