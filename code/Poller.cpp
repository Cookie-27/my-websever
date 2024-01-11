#include "Poller.h"
#include <unistd.h>
#include <cstring>
#include "Channel.h"

#define MAX_EVENTS 1000

Poller::Poller()
{
    fd_ = epoll_create1(0);
    //cuo wu chu li
    events_ = new epoll_event[MAX_EVENTS];
    memset(events_,0,sizeof(events_));
}

Poller:: ~Poller()
{
    if(fd_ != -1){
        close(fd_);
    }
    delete events_;
}

RS Poller::UpdateChannel(Channel *ch) const
{
    int sockfd = ch->fd();
    struct epoll_event ev {};
    ev.data.ptr = ch;
    if (ch->listen_events() & Channel::READ_EVENT) {
        ev.events |= EPOLLIN | EPOLLPRI;
    }
    if (ch->listen_events() & Channel::WRITE_EVENT) {
        ev.events |= EPOLLOUT;
    }
    if (ch->listen_events() & Channel::ET) {
        ev.events |= EPOLLET;
    }
    if (!ch->exist()) {
        epoll_ctl(fd_, EPOLL_CTL_ADD, sockfd, &ev);
        //ErrorIf(epoll_ctl(fd_, EPOLL_CTL_ADD, sockfd, &ev) == -1, "epoll add error");
        ch->set_exist();
    } else {
        epoll_ctl(fd_, EPOLL_CTL_MOD, sockfd, &ev);
        //ErrorIf(epoll_ctl(fd_, EPOLL_CTL_MOD, sockfd, &ev) == -1, "epoll modify error");
    }
}

RS Poller::DeleteChannel(Channel *ch) const
{
    int sockfd = ch->fd();
    epoll_ctl(fd_, EPOLL_CTL_DEL, sockfd, nullptr);
    ch->set_exist(false);
}

std::vector<Channel *> Poller::Poll(long timeout = -1) const
{
    std::vector<Channel *> active_channels;
    int nfds = epoll_wait(fd_, events_, MAX_EVENTS, timeout);
    //ErrorIf(nfds == -1, "epoll wait error");
    for (int i = 0; i < nfds; ++i) {
    Channel *ch = (Channel *)events_[i].data.ptr;
    int events = events_[i].events;
    if (events & EPOLLIN) {
        ch->set_ready_event(Channel::READ_EVENT);
    }
    if (events & EPOLLOUT) {
        ch->set_ready_event(Channel::WRITE_EVENT);
    }
    if (events & EPOLLET) {
        ch->set_ready_event(Channel::ET);
    }
    active_channels.push_back(ch);
  }
  return active_channels;
}