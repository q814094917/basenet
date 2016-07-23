//
//  TcpConnection.h
//  reactor
//
//  Created by 高聪 on 16/7/19.
//  Copyright (c) 2016年 高聪. All rights reserved.
//

#ifndef __reactor__TcpConnection__
#define __reactor__TcpConnection__

#include <stdio.h>
#include <memory>
#include "Declear.h"
#include "Buffer.h"
#include "Poller.h"
#include "Define.h"
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
//connection in only one thread
namespace reactor{
    class TcpConnection{
    private:
        int fd_;
        bool connected_;
        std::shared_ptr<Poller> poller_; //you dont know
    public:
        Buffer inputBuffer_;
        Buffer outputBuffer_;
        
        TcpConnection(int fd,std::shared_ptr<Poller> poller)
        :poller_(poller)
        ,fd_(fd)
        {
            connected_=true;
        }
        
        
        bool isConnected(){
            return connected_;
        }
        
        void setConnected(bool conn){
            connected_=conn;
        }
        
        
        
        int send(){
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
                        printf("send successs!\n");
                        return 0;
                        
                    }
                    outputBuffer_.retrieve(size);
                    break;
                    
                }
            }
            
            return 0;
            
        }
        
        int recv(){
            //fix effective
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
                    inputBuffer_.append(buf,size);
                    continue;
                }else{
                    inputBuffer_.append(buf,size);
                    //printf("recv success!\n");
                    return 0;
                }
            }
        }
        
        void registerWriteEvent(){
            eventf flag=MPOLL_IN|MPOLL_OUT;
            poller_->modFd(fd_,flag,this);
            
        }
        
        void deleteWriteEvent(){
            eventf flag=MPOLL_IN;
            poller_->modFd(fd_,flag,this);
            
        }
    };
}

#endif /* defined(__reactor__TcpConnection__) */
