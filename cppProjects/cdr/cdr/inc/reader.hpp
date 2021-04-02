#ifndef P78C9F96_E4A9_4DD8_AE85_354EE134789D_H__
#define P78C9F96_E4A9_4DD8_AE85_354EE134789D_H__

#include "cdr_struct.hpp"
#include "noncopyable.hpp"
#include "waiting_queue.hpp"

#include <fstream>
#include <string>

namespace advcpp
{

class Reader : private NonCopyable
{
public:
    Reader(MtWaitingQueue<Cdr>& a_cdrQueue, const std::string& a_source);
    ~Reader();
    void Start();

private:
    void moveToDone();

private:
    MtWaitingQueue<Cdr>&  m_queue;
    const std::string m_fileName;
    std::ifstream m_fp;
};

} // namespace advcpp

#endif // P78C9F96_E4A9_4DD8_AE85_354EE134789D_H__
