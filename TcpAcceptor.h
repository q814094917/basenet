//
//  TcpAcceptor.h
//  reactor
//
//  Created by 高聪 on 16/7/20.
//  Copyright (c) 2016年 高聪. All rights reserved.
//

#ifndef __reactor__TcpAcceptor__
#define __reactor__TcpAcceptor__

#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
#include "TcpSocket.h"
// make a newconnection task to taskqueue
// must mutex


namespace reactor {
    class TcpAcceptor{
    private:
        int listenfd_;
        bool listening_;
        
    public:
        TcpAcceptor();
        ~TcpAcceptor();
        void bindAddr(std::string ip,uint16_t port);
        void listen();
        int accept();
        int acceptpeer(struct sockaddr_in& addr);
        void newacceptor(){listenfd_=createTcp();
            setNonBlocking(listenfd_);};
        int getfd(){return listenfd_;}
        
        void setReuseAddr(bool on);
        
        void setReusePort(bool on);
    };
}

#endif /* defined(__reactor__TcpAcceptor__) */
