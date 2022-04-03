#include "request.h"

#include <iostream>

using namespace std;

RequestHandler::RequestHandler()
    : m_thread(&RequestHandler::worker, this)
{
}

RequestHandler::~RequestHandler()
{
    queueEvent(Event::Destroy);
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

bool RequestHandler::doBlockingAction()
{
    auto f = queueEvent(Event::BlockingAction);
    return f.valid() ? f.get() : false;
}

future<bool> RequestHandler::doAsyncAction()
{
    return queueEvent(Event::BlockingAction);
}

future<bool> RequestHandler::queueEvent(Event e)
{
    future<bool> f;
    {
        lock_guard<mutex> _{m_mutex};
        m_eventQueue.emplace_back(e, promise<bool>{});
        f = m_eventQueue.back().second.get_future();
    }

    m_cv.notify_one();

    return f;
}

void RequestHandler::worker()
{
    bool shutdown = false;
    while (!shutdown)
    {
        Event e{};
        promise<bool> p;
        {
            unique_lock<mutex> lk{m_mutex};
            m_cv.wait(lk, [&]() { return !m_eventQueue.empty(); });
            e = m_eventQueue.front().first;
            swap(p, m_eventQueue.front().second);
            m_eventQueue.pop_front();
        }

        switch(e)
        {
        case Event::BlockingAction:
            p.set_value(handleBlockingEvent());
            break;
        case Event::Destroy:
            handleDestroy();
            p.set_value(true);
            shutdown = true;
            break;
        };
    }
}

bool RequestHandler::handleBlockingEvent()
{
    // do processing for blocking event handling
    cout << "RequestHandler processing blocking request..." << endl;
    this_thread::sleep_for(20ms);
    return true;
}

bool RequestHandler::handleAsyncEvent()
{
    // do processing for async event handling
    cout << "RequestHandler processing async request..." << endl;
    this_thread::sleep_for(20ms);
    return true;
}

void RequestHandler::handleDestroy()
{
    // do processing for cleanup
}