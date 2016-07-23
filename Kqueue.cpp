//
//  Kqueue.cpp
//  reactor
//
//  Created by 高聪 on 16/7/19.
//  Copyright (c) 2016年 高聪. All rights reserved.
//

#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/event.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Kqueue.h"
#include "Define.h"

using namespace reactor;
Poller::Poller(){
    pollfd_=kqueue();
}

Poller::~Poller(){
    close(pollfd_);
}

int Poller::getPollFd(){
    return pollfd_;
}

void Poller::delFd(int fd){
    struct kevent ke;
    EV_SET(&ke, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    kevent(pollfd_, &ke, 1, NULL, 0, NULL);
    EV_SET(&ke, fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
    kevent(pollfd_, &ke, 1, NULL, 0, NULL);
}

int Poller::addFd(int fd,void* ud){
    struct kevent ke;
    
    EV_SET(&ke, fd, EVFILT_READ, EV_ADD, 0, 0,ud);
    if (kevent(pollfd_, &ke, 1, NULL, 0, NULL) == -1) {
        return 1;
    }
    
    EV_SET(&ke, fd, EVFILT_WRITE, EV_ADD, 0, 0, ud);
    if (kevent(pollfd_, &ke, 1, NULL, 0, NULL) == -1) {
        EV_SET(&ke, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
        kevent(pollfd_, &ke, 1, NULL, 0, NULL);
        return 1;
    }
    
    
    EV_SET(&ke, fd, EVFILT_WRITE, EV_DISABLE, 0, 0, ud);
    if (kevent(pollfd_, &ke, 1, NULL, 0, NULL) == -1) {
        delFd(fd);
        return 1;
    }
    return 0;
}

void Poller::modFd(int fd,eventf events,void* ud){
    struct kevent ke;
    EV_SET(&ke, fd, EVFILT_READ, (events&MPOLL_OUT) ? EV_ENABLE : EV_DISABLE, 0, 0, ud);
    if (kevent(pollfd_, &ke, 1, NULL, 0, NULL) == -1) {
        //error
    }

}

int Poller::wait(poll_event* e,int max){
    struct kevent ev[max];
    int n = kevent(pollfd_, NULL, 0, ev, max, NULL);
    
    int i;
    for (i=0;i<n;i++) {
        e[i].ptr = ev[i].udata;
        e[i].fd=(int)ev[i].ident;
        unsigned filter = ev[i].filter;
        e[i].flag |= (filter == EVFILT_WRITE)?MPOLL_OUT:0;
        e[i].flag |= (filter == EVFILT_READ)?MPOLL_IN:0;
        
    }
    
    return n;
}