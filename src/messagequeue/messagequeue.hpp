#ifndef MESSAGEQUEUE_HPP
#define MESSAGEQUEUE_HPP

#include <queue>
#include <mutex>
#include <string>
#include <condition_variable>
#include <vector>

/**
 * This class maintains a queue of messages that can be read 
 * and added to to ensure network packets aren't lost and we can have async
 */
class MessageQueue
{
    private:
        std::vector<std::string> queue{1024};

        std::atomic<size_t> head{0};
        std::atomic<size_t> tail{0};

    public:
        MessageQueue();
        ~MessageQueue();

    /**
     * This function addes a new message to the back of our queue
     * @param message The message to be added to Queue
     * @return None
     */
    bool push (std::string message);

    /**
     * This function pops a  message at the top of the queue
     * @param None 
     * @return The message to be removed to Queue
     */
    bool pop (std::string& message);

};

#endif // MESSAGEQUEUE_HPP

