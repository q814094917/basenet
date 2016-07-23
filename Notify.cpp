//
//  Notify.cpp
//  reactor
//
//  Created by 高聪 on 16/7/23.
//  Copyright (c) 2016年 高聪. All rights reserved.
//

#include "Notify.h"
#include <unistd.h>
#include <errno.h>
#include <assert.h>
using namespace reactor;
Notify::Notify(){
    if(pipe(fd_)<0){
        printf("Notify::Notify pipe error!\n");
        assert(0);
    }
}

Notify::~Notify(){
    close(fd_[0]);
    close(fd_[1]);
}

int Notify::getRecvfd(){
    return fd_[0];
}

int Notify::getSendfd(){
    return fd_[1];
}

void Notify::notify(){
    char one='0';
    for(;;){
        ssize_t count=write(fd_[1], &one, sizeof one);
        if(count<0){
            switch (errno) {
                case EINTR:
                    errno=0;
                    continue ;
            }
            errno=0;
            printf("Notify::notify error!\n");
            return ;
        }
        return ;
    }
}

//4>1 避免多次触发
void Notify::disnotify(){
    char one[4];
    ssize_t count=read(fd_[0], &one, sizeof one);
    if(count<0){
        printf("Notify::disnotify error!\n");
        return;
    }
    
}

void Notify::notifyFd(int fd){
    char one='0';
    for(;;){
        ssize_t count=write(fd, &one, sizeof one);
        if(count<0){
            switch (errno) {
                case EINTR:
                    errno=0;
                    continue ;
            }
            errno=0;
            printf("Notify::notifyFd error!\n");
            return ;
        }
        return ;
    }
}

void Notify::disnotifyFd(int fd){
    char one[4];
    ssize_t count=read(fd, &one, sizeof one);
    if(count<0){
        printf("Notify::disnotifyFd error!\n");
        return;
    }
    
}

