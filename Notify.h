//
//  Notify.h
//  reactor
//
//  Created by 高聪 on 16/7/23.
//  Copyright (c) 2016年 高聪. All rights reserved.
//

#ifndef __reactor__Notify__
#define __reactor__Notify__

//使用pipe来实现notify的作用
//fd[0] is read
//fd[1] is write
#include <stdio.h>
namespace reactor{
    class Notify{
    private:
        int fd_[2];
    public:
        Notify();
        ~Notify();
        int getRecvfd();
        int getSendfd();
        void notify();
        void disnotify();
        
        static void notifyFd(int fd);
        static void disnotifyFd(int fd);
        
    };
}
#endif /* defined(__reactor__Notify__) */
