//
//  Epoll.cpp
//  reactor
//
//  Created by 高聪 on 16/7/19.
//  Copyright (c) 2016年 高聪. All rights reserved.
//

#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "Epoll.h"
#include "Define.h"

using namespace reactor;
Poller::Poller(){
    pollfd_=epoll_create(1024);
}

Poller::~Poller(){
    close(pollfd_);
}

int Poller::getPollFd(){
    return pollfd_;
}

void Poller::delFd(int fd){
    epoll_ctl(pollfd_, EPOLL_CTL_DEL, fd, NULL);
}

int Poller::addFd(int fd,void* ud){
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.ptr = ud;
    if (epoll_ctl(pollfd_, EPOLL_CTL_ADD, fd, &ev) == -1) {
        return 1;
    }
    return 0;
}

void Poller::modFd(int fd,eventf events,void* ud){
    struct epoll_event ev;
    ev.events = (events&MPOLL_IN ? EPOLLIN : 0) | (events&MPOLL_OUT ? EPOLLOUT : 0);
    ev.data.ptr = ud;
    epoll_ctl(pollfd_, EPOLL_CTL_MOD, fd, &ev);
    
}

int Poller::wait(poll_event* e,int max){
    struct epoll_event ev[max];
    int n = epoll_wait(pollfd_ , ev, max, -1);
    int i;
    for (i=0;i<n;i++) {
        e[i].ptr = ev[i].data.ptr;
        unsigned flag = ev[i].events;
        e[i].flag |= (flag & EPOLLOUT) != 0;
        e[i].flag |= (flag & EPOLLIN) != 0;
    }
    
    return n;
}