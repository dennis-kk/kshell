/*
 * Copyright (c) 2013-2015, dennis wang
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SERVICE_H
#define SERVICE_H

#include "pragma.h"

#include "service_exception.h"

/**
 * @defgroup service 服务
 * 服务
 *
 * <pre>
 * 可以被其他bundle使用的服务必须实现这个接口.
 * </pre>
 * @{
 */

class Service {
public:
    static const int STARTING = 1; ///< 启动中
    static const int ACTIVE   = 2; ///< 已激活并可以使用
    static const int STOPPING = 4; ///< 停止中
    static const int STOPPED  = 8; ///< 已停止

public:
    virtual ~Service() {}

    /**
     * 自定义初始化
     */
    virtual void initialize() throw(ServiceException) = 0;

    /**
     * 自定义清理函数
     */
	virtual void finalize() throw(ServiceException) = 0;
};

/** @} */

#endif // SERVICE_H
