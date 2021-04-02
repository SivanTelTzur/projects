#ifndef BC688120_1BD3_4677_AA4A_B0475C4B4B86__
#define BC688120_1BD3_4677_AA4A_B0475C4B4B86__

#include "isensor.hpp"
#include <string>
#include "inet_client.hpp"
#include "sharedptr.hpp"


namespace advcpp
{

class Sensor{
public:
    Sensor(const std::string& a_ip, size_t a_port);
    void Run();
    void Stop();
private:
    char* collectData(unsigned int* a_buffersize);

private:
    SharedPtr<INetClient> m_shpNet;
    AtomicValue<bool> m_active;
};

} // namespace advcpp


#endif // BC688120_1BD3_4677_AA4A_B0475C4B4B86__