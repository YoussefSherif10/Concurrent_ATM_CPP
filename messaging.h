//
// Created by youssef fekry on 13/02/2023.
/*
 * A simple message queue that stores messages as pointers to a base class.
 * Pushing an entry constructs an instance of the wrapper class and stores a pointer
 * to it. Popping an entry returns that pointer.
 * the Popping thread will need to cast the pointer to a suitable message wrapper before
 * accessing the stored message.
 * For every entry we construct the message using the wrap struct that inherits from the
 * base struct but the pointer entered to the queue points to the base struct
 */

#ifndef ATM_MESSAGING_H
#define ATM_MESSAGING_H

#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>

namespace messaging {
    // Base class of the queue entries
    struct MessageBase {
        virtual ~MessageBase(){}
    };

    // A wrap around the base class
    template<typename Msg>
    struct WrappedMessage : MessageBase {
        Msg content;
        explicit WrappedMessage(const Msg &content_) : content(content_) {}
    };

    class queue {
    private:
        std::mutex m;
        std::condition_variable c;
        std::queue<std::shared_ptr<MessageBase>> q;

    public:
        /**
         * the push function first acquire the mutex then push pointer to the message after
         * wrapping it with the Wrap-Message struct. then notify condition variable once
         * the pushing is done
         * @tparam T : type of the message
         * @param msg : content of the message
         */
        template<typename T>
        void push(const T &msg){
            std::lock_guard<std::mutex> guard(m);
            q.push(std::make_shared<WrappedMessage<T>>(msg));
            c.notify_all();
        }

        /**
         * Many threads can wait to read an element. this function allows to wait and get the
         * passed message.
         * @return shared pointer to the message wrapped in the base struct.
         */
        std::shared_ptr<MessageBase> wait_and_pop(){
            std::unique_lock<std::mutex> guard(m);
            // wait for the mutex and the q is not empty
            c.wait(guard, [&]{return !q.empty();});
            auto result = q.front();
            q.pop();
            return result;
        }
    };
}

#endif //ATM_MESSAGING_H
