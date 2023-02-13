//
// Created by youssef fekry on 13/02/2023.
/*
 * to receive a message we need to wait for the message and check the type of the
 * wanted message by the caller to be handled with the appropriate function
 */

#ifndef ATM_RECEVIER_H
#define ATM_RECEVIER_H

#include "messaging.h"
#include "sender.h"
#include "dispatcher.h"

namespace messaging {
    class Recevier {
    private:
        queue q; // a recevier owns the queue

    public:
        /**
         * Allow implicit conversion to a sender that references the queue
         * @return
         */
        operator Sender(){
            return Sender(&q);
        }

        /**
         * waiting for a queue creates dispatcher
         * @return
         */
        Dispatcher wait(){
            return Dispatcher(&q);
        }
    };
}

#endif //ATM_RECEVIER_H
