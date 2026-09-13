#include "messagequeue.hpp"

MessageQueue::MessageQueue() = default;

MessageQueue::~MessageQueue() = default;

bool MessageQueue::push (std::string message){
    auto current_head = head.load(std::memory_order_acquire);
    auto current_tail = tail.load(std::memory_order_relaxed);

    if ((current_tail - current_head) >= 1024){
        return false;
    }

    auto index = current_tail % 1024;
    queue[index] = message;
    tail.store(current_tail + 1, std::memory_order_release);
    return true;
}

bool MessageQueue::pop (std::string& message){
    auto current_head = head.load(std::memory_order_relaxed);
    auto current_tail = tail.load(std::memory_order_acquire);

    if(current_head == current_tail){
        return false;
    }
    auto index = current_head % 1024;
    message = queue[index];

    head.store(current_head + 1, std::memory_order_release);

    return true;
}
