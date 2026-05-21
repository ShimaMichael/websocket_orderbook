#include "messagequeue.hpp"

MessageQueue::MessageQueue() = default;

MessageQueue::~MessageQueue() = default;

void MessageQueue::push (std::string message){
    std::unique_lock<std::mutex> lock(mutex_);
    queue.push(std::move(message));
    cv_.notify_one();
}

bool MessageQueue::pop (std::string& message){
    std::unique_lock<std::mutex> lock(mutex_);

    while(queue.empty()){
        auto status = cv_.wait_for(lock, std::chrono::milliseconds(1));

        if (status == std::cv_status::timeout && queue.empty()){
            return false;
        }
    }
    
    message = std::move(queue.front());
    queue.pop();
    return true;
}
