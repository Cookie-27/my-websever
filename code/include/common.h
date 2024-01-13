#pragma once

class Channel;
class EventLoop;
class Poller;
class Socket;
class Acceptor;
class ThreadPool;
class Buffer;
class Connection;
class HttpContext;
class HttpRequest;


enum RS //Run State
{
    RS_DEFEAT,
    RS_SUCCES,
    RS_UNDEFINED
};