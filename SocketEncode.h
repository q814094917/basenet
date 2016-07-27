//
//  SocketEncode.h
//  reactor
//
//  Created by 高聪 on 16/7/27.
//  Copyright (c) 2016年 高聪. All rights reserved.
//

#ifndef reactor_SocketEncode_h
#define reactor_SocketEncode_h

#include "SocketModel.h"
#include "Buffer.h"
//fix protobuf like
using namespace reactor;
namespace gameserver {
    
    
    SocketModel Decode(void* message,size_t size){
        Buffer decoder;
        decoder.append(message,size);
        SocketModel sm;
        memcpy(&sm.type,decoder.getReadPeek(),sizeof(int));
        decoder.retrieve(sizeof(int));
        memcpy(&sm.command,decoder.getReadPeek(),sizeof(int));
        decoder.retrieve(sizeof(int));
        memcpy(&sm.option,decoder.getReadPeek(),sizeof(int));
        decoder.retrieve(sizeof(int));
        printf("%d",size);
        sm.message=malloc(size-(sizeof(int)*3));
        memcpy(sm.message,decoder.getReadPeek(),size-(sizeof(int)*3));
        
        return sm;
    }
}

#endif
