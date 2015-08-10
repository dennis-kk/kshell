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

#ifndef SERVICE_REFERENCE_H
#define SERVICE_REFERENCE_H

#include "pragma.h"

class Service;

/**
 * @defgroup service_reference 服务引用
 * 服务引用
 *
 * <pre>
 * Service指针引用包装，提供对Service指针的应用计数及生命周期管理
 * </pre>
 * @{
 */

class ServiceReference {
public:
    virtual ~ServiceReference() {}

    /**
     * 获取Service指针并增加引用计数
     * @return Service指针
     */
    virtual Service* get() = 0;

    /**
     * 减少引用计数
     */
    virtual void unget() = 0;

    /**
     * 如果引用计数为零则销毁
     */
    virtual void destroy() = 0;

    /**
     * 引用的Service是否有效
     * @retval true 有效
     * @retval false 无效
     */
    virtual bool isOk() = 0;
};

/** @} */

#endif // SERVICE_REFERENCE_H