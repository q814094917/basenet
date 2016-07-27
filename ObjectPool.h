//
//  ObjectPool.h
//  reactor
//
//  Created by 高聪 on 16/7/25.
//  Copyright (c) 2016年 高聪. All rights reserved.
//

#ifndef reactor_ObjectPool_h
#define reactor_ObjectPool_h

#include <list>
#include <iostream>
#include <memory>
namespace reactor{

template<typename T>
class ObjectPool{
    
private:
    std::list<T*> objectPool_;
    int number_;
    
    
    template <typename ...Args>
    void initObjectPool(Args ...args){
        for(int i=0;i<number_;i++){
            objectPool_.push_back(new T(args...));
        }
    }
    
public:
    template<typename ...Args>
    ObjectPool(int number,Args ...args)
    :number_(number)
    {
        initObjectPool(args...);
    }
    
    
    T *pop(void)
    {
        if(!objectPool_.empty()){
            T *objectPtr= objectPool_.front();
            objectPool_.pop_front();
            return objectPtr;
        }else{
            return NULL;
        }
        
    }
    
    
    void push(T *t)
    {
        objectPool_.push_front(t);
    }
    
    
    ~ObjectPool()
    {
        for(auto x : objectPool_)
        {
            delete x;
        }
    }
};
}

#endif
