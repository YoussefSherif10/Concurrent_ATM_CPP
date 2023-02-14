//
// Created by youssef fekry on 13/02/2023.
/*
 * the complexity of the message dispatching starts with the wait(). this creates
 * dispatcher object that reference the queue from the recevier.
 * waiting for message and dispatching it is made in the destructor.
 * simply, it checks if the message is closing_queue message or not. if not it returns
 * false
 */

#ifndef ATM_DISPATCHER_H
#define ATM_DISPATCHER_H

#include "messaging.h"
#include "recevier.h"
#include "templateDispatcher.h"

namespace messaging {
    // the message for closing queue
    class CloseQueue {};

    class Dispatcher {
        queue *q;
        bool chained;

        // disable copy constructor and assign operator
        Dispatcher (Dispatcher const&) = delete;
        Dispatcher& operator=(Dispatcher const&) = delete;

        // Allow template dispatcher class instance to access dispatcher
        template<
                typename Dispatcher,
                typename Msg,
                typename Func>
        friend class TemplateDispatcher;

        /**
         * loop, wait for the msg, dispatch it.
         */
        void wait_and_dispatch(){
            for(;;){
                auto msg = q->wait_and_pop();
                dispatch(msg);
            }
        }

        /**
         * checks if the message is closing message
         * @param msg : the content of the popped pointer
         * @return throws exception if closing message otherwise, false
         */
        bool dispatch(std::shared_ptr<MessageBase> const &msg){
            if (dynamic_cast<WrappedMessage<CloseQueue>*>(msg.get()))
                throw CloseQueue();
            return false;
        }

    public:
        // instance can be moved
        Dispatcher(Dispatcher &&other) : q(other.q), chained(other.chained) {
            other.chained = true;
        }

        explicit Dispatcher(queue *q_): q(q_), chained(false) {}

        /**
         * handle a specific type of message by the template dispatcher class by passing the queue, the current
         * dispatcher object and the handling function.
         * @tparam Message : type of message
         * @tparam Func : TemplateDispatcher method type
         * @param f : TemplateDispatcher method
         * @return
         */
        template<typename Message, typename Func>
        TemplateDispatcher<Dispatcher, Message, Func>
        handle(Func &&f){
            return TemplateDispatcher<Dispatcher, Message, Func>(q, this, std::forward<Func>(f));
        }

        /**
         * the destructor might throw exceptions
         */
        ~Dispatcher() noexcept(false) {
            if(!chained)
                wait_and_dispatch();
        }
    };
}

#endif //ATM_DISPATCHER_H
