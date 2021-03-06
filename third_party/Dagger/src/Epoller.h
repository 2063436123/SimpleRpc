//
// Created by Hello Peter on 2021/9/8.
//

#pragma once

#include <vector>
#include <unordered_map>
#include <cassert>
#include <sys/epoll.h>
#include <memory>
#include <mutex>
#include "Options.h"

class Event;

class Epoller {
public:
    int epollId() const {
        return epollfd_;
    }

    Epoller();

    ~Epoller();

    void addEvent(Event *event);

    // todo 下面的参数是fd，epoller将从eventList中搜索出对应的shared_ptr<Event>，
    // 然后提取相关信息，并修改eventList或epoll_ctl.
    // muduo方案：直接传Event*，因为外部变量（例如TcpConnection）
    // 将持有Event对象，所以不需要智能指针
    void updateEvent(Event *event);

    void removeEvent(Event *event);

    std::vector<Event*> poll(int timeout = -1);

    // thread-local的evlist，用来缓存epoll_wait返回的结果
    static const int MAX_EVENTS = 64;
    thread_local static epoll_event evlist[MAX_EVENTS];
private:
    // 保存Event，这是为了poll时能返回对应的Events
//    std::unordered_map<int, std::shared_ptr<Event>> eventList_;
    int epollfd_;
};