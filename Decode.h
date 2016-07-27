//
//  Decode.h
//  reactor
//
//  Created by 高聪 on 16/7/20.
//  Copyright (c) 2016年 高聪. All rights reserved.
//

#ifndef reactor_Decode_h
#define reactor_Decode_h
#include <stdio.h>
#include <stdint.h>
#include "Buffer.h"

namespace  reactor {
    
    static size_t Decodeu16(Buffer* inputbuffer){
        
        if(inputbuffer->readableSize()>=sizeof(uint16_t)){
            uint16_t be16=0;
            memcpy(&be16,inputbuffer->getReadPeek(),sizeof(be16));
            be16=ntohs(be16);
            if(inputbuffer->readableSize()>((size_t)be16-sizeof(be16))){
                inputbuffer->retrieve(sizeof(be16));
                return (size_t)be16;
            }else{
                return  0;
            }
        }else{
            //printf("Decode size not enough!\n");
            return 0;
        }
    }
    
    static size_t Decode16(Buffer* inputbuffer){
        
        if(inputbuffer->readableSize()>=sizeof(int16_t)){
            
            int16_t be16=0;
            memcpy(&be16,inputbuffer->getReadPeek(),sizeof(be16));
            be16=ntohs(be16);
            if(inputbuffer->readableSize()>((size_t)be16-sizeof(be16))){
                inputbuffer->retrieve(sizeof(be16));
                return (size_t)be16;
            }else{
                return  0;
            }
        }else{
            
            return 0;
        }
    }
    
    static size_t Decodeu32(Buffer* inputbuffer){
        if(inputbuffer->readableSize()>=sizeof(uint32_t)){
            uint32_t be32=0;
            memcpy(&be32,inputbuffer->getReadPeek(),sizeof(be32));
            be32=ntohl(be32);
            if(inputbuffer->readableSize()>((size_t)be32-sizeof(be32))){
                inputbuffer->retrieve(sizeof(be32));
                return (size_t)be32;
            }else{
                return  0;
            }
        }else{
            return 0;
        }
    }
    
    static size_t Decode32(Buffer* inputbuffer){
        if(inputbuffer->readableSize()>=sizeof(int32_t)){
            int32_t be32=0;
            memcpy(&be32,inputbuffer->getReadPeek(),sizeof(be32));
            be32=ntohl(be32);
            if(inputbuffer->readableSize()>((size_t)be32-sizeof(be32))){
                inputbuffer->retrieve(sizeof(be32));
                return (size_t)be32;
            }else{
                return  0;
            }
        }else{
            return 0;
        }
    }
    
    
}
#endif
