#ifndef MESSAGEQUEUE_HPP
#define MESSAGEQUEUE_HPP

#include <queue>
#include <mutex>
#include <string>
#include <condition_variable>

/**
 * This class maintains a queue of messages that can be read 
 * and added to to ensure network packets aren't lost and we can have async
 */
class MessageQueue
{
private:
    std::mutex mutex_;
    std::condition_variable cv_;
    std::queue<std::string> queue;

public:
    MessageQueue();
    ~MessageQueue();

/**
 * This function addes a new message to the back of our queue
 * @param message The message to be added to Queue
 * @return None
 */
void push (std::string message);

/**
 * This function pops a  message at the top of the queue
 * @param None 
 * @return The message to be removed to Queue
 */
bool pop (std::string& message);

};

#endif // MESSAGEQUEUE_HPP

