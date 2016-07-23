//
//  BlockingQueue.h
//  reactor
//
//  Created by 高聪 on 16/7/22.
//  Copyright (c) 2016年 高聪. All rights reserved.
//

#ifndef reactor_BlockingQueue_h
#define reactor_BlockingQueue_h

#include <deque>
#include <queue>
#include "ThreadMutex.h"
#include "ThreadCondition.h"

//fix blockingQueue  !!!
namespace reactor{
    
#define READYQUEUE 4
    
    template <class T>
    class BlockingQueue{
    private:
        std::queue<T> queue_;
        ThreadMutex mutex_;
        ThreadCondition cond_;
        
    public:
        BlockingQueue()
        :cond_(mutex_)
        {}
        void put(const T& one)
        {
            ThreadGuard lock(mutex_);
            queue_.push(one);
            cond_.notify();
        }
        
        //fix!!
        void putqueue(std::queue<T>& q){
            ThreadGuard lock(mutex_);
            for(int i=0;i<q.size();i++){
                queue_.push(q.front());
                q.pop();
            }
            cond_.notify();
            
        }
        
        T take()
        {
            ThreadGuard lock(mutex_);
            while(queue_.empty())
            {
                cond_.wait();
            }
            T front(queue_.front());
            queue_.pop();
            return front;
        }
        
    };
    
    
}


#endif
