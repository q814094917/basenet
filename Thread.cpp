//
//  Thread.cpp
//  reactor
//
//  Created by 高聪 on 16/7/21.
//  Copyright (c) 2016年 高聪. All rights reserved.
//

#include "Thread.h"


#include <unistd.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <assert.h>

namespace CurrentThread
{
    __thread int t_cachedTid = 0;
}

using namespace reactor;


Thread::Thread(ThreadTask& task,void* param)
:task_(task)
,started_(false)
,joined_(false)
,pthreadId_(0)
,param_(param)
{
}

Thread::~Thread(){
    if (started_ && !joined_)
    {
        pthread_detach(pthreadId_);
    }
}

void Thread::start(){
    assert(!started_);
    started_=true;
    if(pthread_create(&pthreadId_, NULL,task_,param_)){
        started_=false;
        printf("Thread::start");
    }
}

void Thread::stop(){
    assert(started_);
    assert(!joined_);
    joined_ = true;
    pthread_join(pthreadId_, NULL);
    
}

pid_t Thread::gettid(){
    return static_cast<pid_t>(::syscall(SYS_gettid));
}
