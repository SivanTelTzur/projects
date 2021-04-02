#include "queries_tcp_server.hpp"

#include <iostream>


int main(void){
    advcpp::MtWaitingQueue<advcpp::Query> a(0);
    advcpp::QueriesTcpServer s(8080, a);
    s.Run();
    return 0;
}