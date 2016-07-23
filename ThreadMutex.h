//
//  Thread.h
//  reactor
//
//  Created by 高聪 on 16/7/19.
//  Copyright (c) 2016年 高聪. All rights reserved.
//

#ifndef reactor_Mutex_h
#define reactor_Mutex_h


#include<assert.h>
#include<pthread.h>

#define MCHECK(ret) ({ __typeof__ (ret) errnum = (ret);         \
assert(errnum == 0); (void) errnum;})

namespace reactor{
    class ThreadMutex{
    private:
        pthread_mutex_t mutex_;
    public:
        ThreadMutex(){
            MCHECK(pthread_mutex_init(&mutex_,NULL));
        }
        ~ThreadMutex(){
            MCHECK(pthread_mutex_destroy(&mutex_));
        }
        
        void lock(){
            MCHECK(pthread_mutex_lock(&mutex_));
        }
        
        void unlock(){
            MCHECK(pthread_mutex_unlock(&mutex_));
        }
        
        pthread_mutex_t* getPhtreadMutex(){
            return &mutex_;
        }
    };
    
    class ThreadGuard{
    private:
        ThreadMutex& mutex_;
    public:
        explicit ThreadGuard(ThreadMutex&mutex):mutex_(mutex){
            mutex_.lock();
        }
        ~ThreadGuard(){
            mutex_.unlock();
        }
    };
}



#endif
