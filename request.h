#pragma once

#include <deque>
#include <future>
#include <mutex>

class RequestHandler {
public:
    RequestHandler();
    ~RequestHandler();

    bool doBlockingAction();
    std::future<bool> doAsyncAction();

private:
    enum class Event
    {
        BlockingAction,
        AsyncAction,
        Destroy
    };

    std::future<bool> queueEvent(Event e);
    void worker();

    bool handleBlockingEvent();
    bool handleAsyncEvent();
    void handleDestroy();

    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::deque<std::pair<Event, std::promise<bool>>> m_eventQueue;
    std::thread m_thread;
};