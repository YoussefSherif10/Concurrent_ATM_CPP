//
// Created by youssef fekry on 14/02/2023.
//

#ifndef ATM_TEMPLATEDISPATCHER_H
#define ATM_TEMPLATEDISPATCHER_H

#include "messaging.h"
#include "dispatcher.h"

namespace messaging {
    template<typename PreviousDispatcher, typename Msg, typename Func>
    class TemplateDispatcher {
        queue *q;
        PreviousDispatcher *prev;
        Func f;
        bool chained;

        // disable copy and assign
        TemplateDispatcher(TemplateDispatcher const&) = delete;
        TemplateDispatcher& operator=(TemplateDispatcher const&) = delete;

        template<typename Dispatcher, typename OtherMsg, typename OtherFunc>
        friend class TemplateDispatcher;

        void wait_and_dispatch(){
            for (;;){
                auto msg = q->wait_and_pop();
                if (dispatch(msg))
                    break;
            }
        }

        bool dispatch(std::shared_ptr<MessageBase> const& msg){
            if (WrappedMessage<Msg>* wrapper = dynamic_cast<WrappedMessage<Msg>*>(msg.get())){
                f(wrapper->content);
                return true;
            } else {
                // chain to the prev dispatcher
                return prev->dispatch(msg);
            }
        }

    public:
        TemplateDispatcher(TemplateDispatcher&& other) : q(other.q), prev(other.prev), f(std::move(other.f)) {
            other.chained = true;
        }

        TemplateDispatcher(queue *q_, PreviousDispatcher* prev_, Func&& f_) :
        q(q_), prev(prev_), f(std::forward<Func>(f_)), chained(false){
            prev_->chained = true;
        }

        template<typename OtherMsg, typename OtherFunc>
        TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc>
        handle(OtherFunc&& of){
            return TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc>(
                    q, this, std::forward<OtherFunc>(of)
                    );
        }

        ~TemplateDispatcher() noexcept(false){
            if (!chained)
                wait_and_dispatch();
        }
    };
}

#endif //ATM_TEMPLATEDISPATCHER_H
