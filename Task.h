//
//  Task.h
//  reactor
//
//  Created by 高聪 on 16/7/20.
//  Copyright (c) 2016年 高聪. All rights reserved.
//

#ifndef reactor_Task_h
#define reactor_Task_h

namespace reactor {
    typedef struct Task_{
        int home_;       //where are you
        int fd_;         //who are you
        int option_;     //what you want do
        size_t size_;
        void* message_;  //what you have
    }Task;
}

#endif
