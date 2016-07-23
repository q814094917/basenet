//
//  Define.h
//  reactor
//
//  Created by 高聪 on 16/7/19.
//  Copyright (c) 2016年 高聪. All rights reserved.
//

#ifndef reactor_Define_h
#define reactor_Define_h
namespace reactor{
    const int MPOLL_IN=1;
    const int MPOLL_OUT=2;
    
    const int RADD=1;
    const int RRECV=2;
    const int RSEND=4;
    const int RDEL=8;
    const int RLOCAL=16;
    //Thread task
    typedef void* (*ThreadTask)(void*);

}
#endif
