//
//  SocketModel.h
//  reactor
//
//  Created by 高聪 on 16/7/27.
//  Copyright (c) 2016年 高聪. All rights reserved.
//

#ifndef reactor_SocketModel_h
#define reactor_SocketModel_h

namespace gameserver {
    typedef struct SocketModel{
        int type;
        int command;
        int option;
        void* message;
    }SocketModel;
}

#endif
