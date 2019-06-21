#include "TcpServer.h"
#include <iostream>

namespace zwp
{
void TcpServer::start()
{
    _eventloop.loop();
}

void TcpServer::stop()
{
    _eventloop.unloop();
}

}
