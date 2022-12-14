/*
 * MIT License
 * Copyright (c) 2021 _VIFEXTech
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef __ACCOUNT_H
#define __ACCOUNT_H

#include <stdint.h>
#include <vector>
#include "PingPongBuffer/PingPongBuffer.h"
#include "lvgl.h"

class AccountBroker;

class Account
{
public:

    /* Event type enumeration */
    typedef enum
    {
        EVENT_NONE,
        EVENT_PUB_PUBLISH, // Publisher posted information   发布消息
        EVENT_SUB_PULL,    // Subscriber data pull request   向发布者请求数据
        EVENT_NOTIFY,      // Subscribers send notifications to publishers  -->Notify
        EVENT_TIMER,       // Timed event
        _EVENT_LAST
    } EventCode_t;

    /* Error type enumeration */
    typedef enum
    {
        ERROR_NONE                = 0,
        ERROR_UNKNOW              = -1,
        ERROR_SIZE_MISMATCH       = -2,
        ERROR_UNSUPPORTED_REQUEST = -3,
        ERROR_NO_CALLBACK         = -4,
        ERROR_NO_CACHE            = -5,
        ERROR_NO_COMMITED         = -6,
        ERROR_NOT_FOUND           = -7,
        ERROR_PARAM_ERROR         = -8
    } ErrorCode_t;

    /* Event parameter structure */
    typedef struct
    {
        EventCode_t event; // Event type
        Account* tran;     // Pointer to sender
        Account* recv;     // Pointer to receiver
        void* data_p;      // Pointer to data
        uint32_t size;     // The length of the data
    } EventParam_t;

    /* Event callback function pointer */
    typedef int (*EventCallback_t)(Account* account, EventParam_t* param);

public:
    Account(
        const char* id,
        AccountBroker* center,
        uint32_t bufSize = 0,
        void* userData = nullptr
    );
    ~Account();

    Account* Subscribe(const char* pubID);
    bool Unsubscribe(const char* pubID);
    bool Commit(const void* data_p, uint32_t size);
    int Publish();
    int Pull(const char* pubID, void* data_p, uint32_t size);
    int Pull(Account* pub, void* data_p, uint32_t size);
    int Notify(const char* pubID, const void* data_p, uint32_t size);
    int Notify(Account* pub, const void* data_p, uint32_t size);
    void SetEventCallback(EventCallback_t callback);
    void SetTimerPeriod(uint32_t period);
    void SetTimerEnable(bool en);
    uint32_t GetPublisherSize();
    uint32_t GetSubscribeSize();

public:
    const char* ID;      /* Unique account ID */
    AccountBroker* Center;  /* Pointer to the data center */
    void* UserData;

    std::vector<Account*> publishers;  /* Followed publishers */  //放着我订阅了的Acount
    std::vector<Account*> subscribers; /* Followed subscribers */ //放着订阅了我的Account

    struct
    {
        EventCallback_t eventCallback;
        lv_timer_t* timer;
        PingPongBuffer_t BufferManager;
        uint32_t BufferSize;
    } priv;

private:
    static void TimerCallbackHandler(lv_timer_t* task);
};

#endif
