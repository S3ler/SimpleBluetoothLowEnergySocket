//
// Created by bele on 29.07.17.
//

#ifndef SIMPLEBLUETOOTHLOWENERGYSOCKET_SAFE_FLAG_H
#define SIMPLEBLUETOOTHLOWENERGYSOCKET_SAFE_FLAG_H


#include <mutex>
#include <condition_variable>

class safe_flag
{
    mutable std::mutex m_;
    mutable std::condition_variable cv_;
    bool flag_;

public:
    safe_flag()
            : flag_(false)
    {}

    bool is_set() const
    {
        std::lock_guard<std::mutex> lock(m_);
        return flag_;
    }

    void set()
    {
        {
            std::lock_guard<std::mutex> lock(m_);
            flag_ = true;
        }
        cv_.notify_all();
    }

    void reset()
    {
        {
            std::lock_guard<std::mutex> lock(m_);
            flag_ = false;
        }
        cv_.notify_all();
    }

    void wait() const
    {
        std::unique_lock<std::mutex> lock(m_);
        cv_.wait(lock, [this] { return flag_; });
    }

    template <typename Rep, typename Period>
    bool wait_for(const std::chrono::duration<Rep, Period>& rel_time) const
    {
        std::unique_lock<std::mutex> lock(m_);
        return cv_.wait_for(lock, rel_time, [this] { return flag_; });
    }

    template <typename Rep, typename Period>
    bool wait_until(const std::chrono::duration<Rep, Period>& rel_time) const
    {
        std::unique_lock<std::mutex> lock(m_);
        return cv_.wait_until(lock, rel_time, [this] { return flag_; });
    }
};


#endif //SIMPLEBLUETOOTHLOWENERGYSOCKET_SAFE_FLAG_H
