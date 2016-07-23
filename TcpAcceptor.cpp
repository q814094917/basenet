//
//  TcpAcceptor.cpp
//  reactor
//
//  Created by 高聪 on 16/7/20.
//  Copyright (c) 2016年 高聪. All rights reserved.
//

#include "TcpAcceptor.h"
#include "TcpSocket.h"
#include <errno.h>
#include <assert.h>
#include <unistd.h>
using namespace reactor;
TcpAcceptor::TcpAcceptor(){
    listenfd_=createTcp();
    setNonBlocking(listenfd_);
    listening_=false;
}

TcpAcceptor::~TcpAcceptor(){
    close(listenfd_);
}

void TcpAcceptor::bindAddr(std::string ip, uint16_t port){
    setReuseAddr(true);
    //setReusePort(true);
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    if(::inet_pton(AF_INET,ip.c_str(),&addr.sin_addr)<=0){
        printf("TcpAcceptor::bindAddr inet_pton\n");
    }
    else if(::bind(listenfd_,(struct sockaddr*)&addr, sizeof(addr))<0){
        printf("TcpAcceptor::bindAddr bind\n");
    }
    else{
        listening_=true;
    }
    
}

void TcpAcceptor::listen(){
    assert(listening_);
    if(::listen(listenfd_, SOMAXCONN)<0){
        printf("TcpAcceptor::listen listen\n");
    }
}

int TcpAcceptor::accept(){
    struct sockaddr_in laddr;
    bzero(&laddr,sizeof laddr);
    int confd;
    socklen_t len=sizeof(laddr);
    confd=::accept(listenfd_, (struct sockaddr*)&laddr,&len);
    setNonBlocking(confd);
    setTcpKeepAlive(confd,true);
    if(confd<0){
        //fix errno
        printf("TcpAcceptor::accept\n");
        if(errno==ENFILE||errno==EMFILE){
            errno=0;
            printf("can retry!\n");
        }else{
            
            printf("%d",errno);
            errno=0;
        }
    }
    return confd;
}

int TcpAcceptor::acceptpeer(struct sockaddr_in& addr){
    struct sockaddr_in laddr;
    bzero(&laddr,sizeof laddr);
    int confd;
    socklen_t len=sizeof(laddr);
    confd=::accept(listenfd_, (struct sockaddr*)&laddr,&len);
    setNonBlocking(listenfd_);
    if(confd<0){
        printf("TcpAcceptor::acceptpeer\n");
        if(errno==ENFILE||errno==EMFILE){
            printf("can retry!\n");
            errno=0;
        }
        errno=0;
    }else{
        addr=laddr;
    }
    setNonBlocking(confd);
    return confd;
}

void TcpAcceptor::setReuseAddr(bool on){
    int optval=on ?1:0;
    if(::setsockopt(listenfd_,SOL_SOCKET,SO_REUSEADDR, &optval,sizeof optval)<0){
        printf("TcpAcceptor::setReuseAddr\n");
    };
}

void TcpAcceptor::setReusePort(bool on){
    int optval=on ?1:0;
    if(::setsockopt(listenfd_,SOL_SOCKET,SO_REUSEPORT, &optval,sizeof optval)<0){
        printf("TcpAcceptor::setReusePort\n");
    };
}

