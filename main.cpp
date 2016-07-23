//
//  main.cpp
//  reactor
//
//  Created by 高聪 on 16/7/19.
//  Copyright (c) 2016年 高聪. All rights reserved.
//

#include <iostream>
#include "TcpConnection.h"
#include "Poller.h"
#include "EventLoop.h"
#include "Encode.h"
#include "Decode.h"
#include "TcpAcceptor.h"
#include "ServerLoop.h"
#include "TcpConnector.h"

#include <memory>
#include <vector>
using namespace reactor;
int main(int argc, const char * argv[]) {
    // insert code here...
    
    //echo
    ServerLoop ser;
    ser.addAcceptor("127.0.0.1", 6558);
    ser.start();
    while(true){
        Task message=ser.getMessage();
        printf("%s",(char*)message.message_);
        
        //avoid free
        Task sender;
        sender.fd_=message.fd_;
        sender.home_=message.home_;
        sender.message_=message.message_;
        sender.option_=RSEND;
        sender.size_=message.size_;
        ser.taskPut(sender);
    }
    
    //sleep(10000);
    std::cout << "Hello, World!11\n";
    return 0;
}
