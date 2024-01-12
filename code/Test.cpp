#include "Server.h"
#include <memory>
#include "EventLoop.h"
#include <iostream>

int main(){
    std::unique_ptr<Server> server_ = std::make_unique<Server>();
    return 0;
}