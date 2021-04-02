#ifndef E5D65145_1379_4B54_ACDE_CF9E6CCBE6EC_H__
#define E5D65145_1379_4B54_ACDE_CF9E6CCBE6EC_H__

#include "inet_server.hpp"
#include "waiting_queue.hpp"
#include "query.hpp"


namespace advcpp
{

INetServer& CreateQueriesServer(size_t a_port, MtWaitingQueue<Query>& a_msgQueue);

} // namespace advcpp

#endif // E5D65145_1379_4B54_ACDE_CF9E6CCBE6EC_H__
