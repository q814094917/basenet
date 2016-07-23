//
//  TcpConnector.h
//  reactor
//
//  Created by 高聪 on 16/7/22.
//  Copyright (c) 2016年 高聪. All rights reserved.
//


//just test
#ifndef reactor_TcpConnector_h
#define reactor_TcpConnector_h
#include <sys/socket.h>
#include "Buffer.h"
#include <string>
#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
namespace reactor {
    class TcpConnector{
    private:
        int fd_;
        bool connected_;
        Buffer inputBuffer_;
        Buffer outputBuffer_;
    public:
        TcpConnector(){
            fd_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (fd_ < 0)
            {
                printf("TcpConnector::TcpConnector()");
            }
        }
        ~TcpConnector(){
            close(fd_);
        };
        int getFd(){return fd_;}
        void connectSock(std::string ip,uint16_t port){
            struct sockaddr_in addr;
            addr.sin_family=AF_INET;
            addr.sin_port=htons(port);
            if(::inet_pton(AF_INET,ip.c_str(),&addr.sin_addr)<=0){
                printf("TcpConnector::connectSock inet_pton\n");
            }
            else if(::connect(fd_,(struct sockaddr*)&addr, sizeof(addr))<0){
                printf("TcpConnector::connectSock connect\n");
            }
            else{
                connected_=true;
            }
        }
        
        void sendMessage(std::string str){
            outputBuffer_.append(str);
            send();
        }
        
        
        
        int send(){
            assert(connected_);
            while(outputBuffer_.readableSize()>0){
                
                for(;;){
                    ssize_t size=write(fd_,outputBuffer_.getReadPeek(),outputBuffer_.readableSize());
                    if(size<0){
                        switch (errno) {
                            case EINTR:
                                continue;
                            case EWOULDBLOCK:
                                return 0;
                        }
                        printf("TcpConnection send\n");
                        
                        return -1;
                    }
                    if(size==outputBuffer_.readableSize()){
                        outputBuffer_.retrieve(size);
                        return 0;
                    }
                    outputBuffer_.retrieve(size);
                    break;
                    
                }
            }
            
            return 0;
        }
        
        std::string recvMessage(){
            if(recv()>0){
                return inputBuffer_.retrieveAllAsString();
            }else{
                return NULL;
            }
        }
        
        int recv(){
            assert(connected_);
            for(;;){
                char buf[1024];
                ssize_t size=read(fd_,buf,sizeof(buf));
                if(size<=0){
                    switch(errno) {
                        case EINTR:
                            break;
                        case EWOULDBLOCK:
                            printf("TcpConnection recv: EAGAIN capture.\n");
                            break;
                        default:
                            // close when error
                            return -1;
                    }
                }else if(size==1024){
                    inputBuffer_.append(buf,sizeof(buf));
                    continue;
                }else{
                    return 0;
                }
            }

        }
        
        
    };
}

#endif
