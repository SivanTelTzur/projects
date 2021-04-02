#ifndef DAED8B1_8A98_4A96_B673_7D163B3345A7_H__
#define DAED8B1_8A98_4A96_B673_7D163B3345A7_H__

#include "cdr_struct.hpp"
#include "waiting_queue.hpp"
#include "noncopyable.hpp"
#include "inet_client.hpp"

#include <fstream>
#include <string>

namespace advcpp
{

class CdrSender : private NonCopyable
{
public:
    static const size_t BUFFER_SIZE = 256;
    CdrSender(MtWaitingQueue<Cdr>& a_cdrQueue, const std::string& a_ip, size_t a_port);
    ~CdrSender();
    void Start();

private:

private:
    MtWaitingQueue<Cdr>&  m_queue;
    INetClient& m_clientNet;
};

} // namespace advcpp

#endif // DAED8B1_8A98_4A96_B673_7D163B3345A7_H__
