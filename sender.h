//
// Created by youssef fekry on 13/02/2023.
/*
 * sending messages is handled only by the sender class which is a thin wrapper around
 * message queue. it only allows messages to be pushed.
 * copying instance of the sender class copies a pointer to the queue rather than the queue itself
 */

#ifndef ATM_SENDER_H
#define ATM_SENDER_H

#include "messaging.h"

namespace messaging {
    class Sender {
    private:
        queue *q; // pointer to the queue

    public:
        Sender() : q(nullptr) {}
        explicit Sender(queue *q_) : q(q_) {}

        /**
         * take the message and push it on the queue
         * @tparam Message : type of the message
         * @param msg : content of the message
         */
        template<typename Message>
        void send(const Message &msg){
            if (q)
                q->push(msg);
        }
    };
}

#endif //ATM_SENDER_H
