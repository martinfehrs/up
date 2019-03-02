#pragma once


#include <sstream>
#include <mutex>
#include <deque>
#include <condition_variable>

#include <QObject>

#include "up/io.hpp"
#include "up/basetypes.hpp"


template <typename tp_value_type>
class blocking_queue_template
{

  public:


    using value_type = tp_value_type;


    void push(const value_type& value)
    {
        {
            std::unique_lock<std::mutex> lock{ m_mutex };
            m_queue.push_front(value);
        }

        m_condition.notify_one();
    }

    value_type pop()
    {
        std::unique_lock<std::mutex> lock{ m_mutex };
        m_condition.wait(lock, [=]{ return !m_queue.empty(); });
        value_type result{ std::move(m_queue.back()) };
        m_queue.pop_back();

        return result;
    }

  private:

    std::mutex m_mutex;
    std::condition_variable m_condition;
    std::deque<value_type> m_queue;

};


