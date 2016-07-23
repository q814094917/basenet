//
//  TcpSocket.h
//  reactor
//
//  Created by 高聪 on 16/7/20.
//  Copyright (c) 2016年 高聪. All rights reserved.
//

#ifndef reactor_TcpSocket_h
#define reactor_TcpSocket_h
#include <fcntl.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
namespace reactor {
    static void setNonBlocking(int fd){
        int flag = fcntl(fd, F_GETFL, 0);
        if ( -1 == flag ) {
            return;
        }
        
        fcntl(fd, F_SETFL, flag | O_NONBLOCK);
    }
    
    static int createTcp(){
        int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sockfd < 0)
        {
            printf("TcpSocket::createTcp");
        }
        return sockfd;
    }
    
    static void setTcpNoDelay(int fd,bool on){
        int optval=on ?1:0;
        if(::setsockopt(fd,IPPROTO_TCP,TCP_NODELAY, &optval,sizeof optval)<0){
            printf("TcpSocket::setNoDelay");
        };
    }
    
    static void setTcpKeepAlive(int fd,bool on){
        int optval=on ?1:0;
        if(::setsockopt(fd,SOL_SOCKET,SO_KEEPALIVE, &optval,sizeof optval)<0){
            printf("TcpSocket::setKeepAlive");
        };
    }
    

}

#endif
