#include "Channel.h"
#include "EventLoop.h"

const short Channel::READ_EVENT = 1;
const short Channel::WRITE_EVENT = 2;
const short Channel::ET = 4;

Channel::Channel(int fd, EventLoop *loop):fd_(fd),loop_(loop),listen_events_(0),ready_events_(0),exist_(false)
{
}

Channel::~Channel()
{
    loop_ ->DeleteChannel(this);
}

void Channel::HandleEvent() const
{
    if(ready_events_ && READ_EVENT){
        read_callback_();
    }
    else if(ready_events_ && WRITE_EVENT){
        write_callback_();
    }
}

void Channel::EnableRead()
{
    listen_events_ |= READ_EVENT;
    loop_->UpdateChannel(this);
}

void Channel::EnableWrite()
{
    listen_events_ |= WRITE_EVENT;
    loop_->UpdateChannel(this);
}

int Channel::fd() const
{
    return this->fd_;
}

short Channel::listen_events() const
{
    return this->listen_events_;
}

short Channel::ready_events() const
{
    return this->ready_events_;
}

bool Channel::exist() const
{
    return this->exist_;
}

void Channel::set_exist(bool in)
{
    this->exist_ = in;
}

void Channel::EnableET()
{
    listen_events_ |= ET;
    loop_->UpdateChannel(this);
}

void Channel::set_ready_event(short ev)
{
    if (ev & READ_EVENT) {
        ready_events_ |= READ_EVENT;
    }
    if (ev & WRITE_EVENT) {
        ready_events_ |= WRITE_EVENT;
    }
    if (ev & ET) {
        ready_events_ |= ET;
    }
}
  
void Channel::set_read_callback(std::function<void()> const &callback)
{
    this->read_callback_ = callback;
}

void Channel::set_write_callback(std::function<void()> const &callback)
{
    this->write_callback_ = callback;
}