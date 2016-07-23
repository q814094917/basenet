//
//  Poller.h
//  reactor
//
//  Created by 高聪 on 16/7/19.
//  Copyright (c) 2016年 高聪. All rights reserved.
//

#ifndef reactor_Poller_h
#define reactor_Poller_h

#ifdef __linux__
#include "Epoll.h"
#endif

#if defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined (__NetBSD__)
#include "Kqueue.h"
#endif



#endif
