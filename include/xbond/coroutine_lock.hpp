#pragma once
#include "vendor.h"
#include "coroutine_mutex.hpp"

namespace xbond {
// 协程排他锁
template <class Mutex>
class basic_coroutine_unique_lock {
    using mutex_type = typename std::decay<Mutex>::type;
    mutex_type* mutex_;
    coroutine_handler* ch_;
 public:
    // 持有信号量，并加锁
    basic_coroutine_unique_lock(mutex_type& mutex, coroutine_handler& ch)
    : mutex_(&mutex)
    , ch_(&ch) {
        mutex_->lock(*ch_);
    }
    // 持有信号量
    basic_coroutine_unique_lock(mutex_type& mutex)
    : mutex_(&mutex)
    , ch_(nullptr) {}
    // 显式加锁
    void lock(coroutine_handler& ch) {
        assert(mutex_);
        assert(ch_ == nullptr);
        ch_ = &ch;
        mutex_->lock(ch);
    }
    // 显式解锁
    void unlock(coroutine_handler& ch) {
        assert(mutex_);
        assert(ch_ == &ch);
        ch_ = nullptr;
        mutex_->unlock(ch);
    }
    // 释放信号量
    mutex_type* release() {
        mutex_type* m = mutex_;
        mutex_ = nullptr;
        return m;
    }
    // 销毁，释放可能的锁
    ~basic_coroutine_unique_lock() {
        if (mutex_ && ch_) mutex_->unlock(*ch_);
    }
};
using coroutine_unique_lock = basic_coroutine_unique_lock<coroutine_unique_mutex>;
// 协程共享锁 
template <class Mutex>
class basic_coroutine_shared_lock {
    using mutex_type = typename std::decay<Mutex>::type;
    mutex_type* mutex_;
    coroutine_handler* ch_;
 public:
    // 持有指定信号量，并对其加锁
    basic_coroutine_shared_lock(mutex_type& mutex, coroutine_handler& ch)
    : mutex_(&mutex)
    , ch_(&ch) {
        mutex_->shared_lock(ch_);
    }

    basic_coroutine_shared_lock(mutex_type& mutex)
    : mutex_(&mutex)
    , ch_(nullptr) {}
    // 显式加锁
    void lock(coroutine_handler& ch) {
        assert(mutex_);
        assert(ch_ == nullptr);
        ch_ = &ch;
        mutex_->shared_lock(ch);
    }
    // 显式解锁
    void unlock(coroutine_handler& ch) {
        assert(mutex_);
        assert(ch_ == &ch);
        ch_ = nullptr;
        mutex_->shared_unlock(ch);
    }
    // 释放信号量
    mutex_type* release() {
        mutex_type* m = mutex_;
        mutex_ = nullptr;
        return m;
    }
    // 销毁，释放可能的锁
    ~basic_coroutine_shared_lock() {
        if (mutex_ && ch_) mutex_->shared_unlock(*ch_);
    }
};
using coroutine_shared_lock = basic_coroutine_shared_lock<coroutine_shared_mutex>;

}