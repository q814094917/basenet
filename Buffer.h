//
//  Buffer.h
//  reactor
//
//  Created by 高聪 on 16/7/19.
//  Copyright (c) 2016年 高聪. All rights reserved.
//

#ifndef __reactor__Buffer__
#define __reactor__Buffer__

#include <stdio.h>
#include <assert.h>
#include <vector>
#include <string>
namespace reactor{
    
    //fix char*
    class Buffer{
    private:
        std::vector<char> buffer_;
        
        size_t readableindex_;
        size_t writeableindex_;
        
        
        char* begin(){
            
            return &*buffer_.begin();
            
            
        }
        void makeSpace(size_t len){
            if(writeableSize()+readableSize()<len){
                buffer_.resize(writeableindex_+len);
            }else{
                std::copy(begin()+readableindex_,
                          begin()+writeableindex_,
                          begin());
                readableindex_=0;
                writeableindex_=writeableindex_-readableindex_;
            }
        }
        
        
        
    public:
        
        
        static const size_t defaultbuffersize=0;
        Buffer(size_t size=defaultbuffersize)
        :buffer_(size)
        {
            
            readableindex_=0;
            writeableindex_=0;
            
        }
        
        ~Buffer(){
            
        }
        
        
        
        size_t readableSize(){
            return writeableindex_-readableindex_;
        }
        
        size_t writeableSize(){
            return buffer_.size() -writeableindex_;
        }
        
        char* getReadPeek(){
            return begin()+readableindex_;
        }
        
        char* getWritePeek(){
            return begin()+writeableindex_;
        }
        
        void haveWriten(size_t len){
            writeableindex_=len+writeableindex_;
        }
        
        
        void resetBuffer(){
            readableindex_=0;
            writeableindex_=0;
        }
        
        void append(const char* data,size_t len){
            if(writeableindex_<len){
                makeSpace(len);
            }
            std::copy(data,data+len,getWritePeek());
            haveWriten(len);
            
        }
        
        void append(std::string data){
            append(data.c_str(),static_cast<size_t>(data.size()));
        }
        
        void append(void* data,size_t len){
            append(static_cast<const char*>(data),len);
        }
        
        std::string retrieveAllAsString(){
            return retrieveAsString(readableSize());
        }
        
        std::string retrieveAsString(size_t len){
            assert(len<=readableSize());
            std::string ret(getReadPeek(),len);
            retrieve(len);
            return ret;
        }
        
        void retrieve(size_t len){
            assert(len <= readableSize());
            if (len < readableSize())
            {
                readableindex_ += len;
            }
            else
            {
                retrieveAll();
            }
        }
        
        void retrieveAll(){
            readableindex_=0;
            writeableindex_=0;
        }
        
        
        
        
        
        
    };
    
    
}
#endif /* defined(__reactor__Buffer__) */
