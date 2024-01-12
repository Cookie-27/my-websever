#include "EventLoop.h"
#include "Poller.h"
#include "Channel.h"

EventLoop::EventLoop()
{
    poller_ = std::make_unique<Poller>();
}
EventLoop::~EventLoop() {}

void EventLoop::Loop() const
{
    while(true){
        std::vector<Channel *> channels = poller_->Poll();
        for(auto channel:channels){
            channel->HandleEvent();
        }
    }
}

void EventLoop::UpdateChannel(Channel *ch) const
{
    poller_ ->UpdateChannel(ch);
}
  
void EventLoop::DeleteChannel(Channel *ch) const
{
    poller_ ->DeleteChannel(ch);
}