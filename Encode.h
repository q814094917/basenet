//
//  Encode.h
//  reactor
//
//  Created by 高聪 on 16/7/20.
//  Copyright (c) 2016年 高聪. All rights reserved.
//

#ifndef reactor_Encode_h
#define reactor_Encode_h
#include <stdint.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
namespace reactor {
    
    
    static uint16_t Encodeu16(size_t x){
        uint16_t x_=(uint16_t)x;
        x_=htons(x_);
        return x_;
    }
    
    static int16_t Encode16(size_t x){
        int16_t x_=(int16_t)x;
        x_=htons(x_);
        return x_;
    }
    
    static uint32_t Encodeu32(size_t x){
        uint32_t x_=(uint32_t)x;
        x_=htonl(x_);
        return x_;
    }
    
    static int32_t Encode32(size_t x){
        int32_t x_=(int32_t)x;
        x_=htonl(x_);
        return x_;
    }
    
}
#endif
