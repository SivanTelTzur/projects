#ifndef DBC1B3D2_3759_42BA_AFFD_D141946A9ACA__
#define DBC1B3D2_3759_42BA_AFFD_D141946A9ACA__

#include "mutex.hpp"

#include <iostream>
#include <string>
#include <exception>

namespace advcpp
{
class Logger
{
public:
    explicit Logger(std::ostream& os = std::cerr);

    void Exception(std::exception const& a_ex);
    void Message(const std::string& a_message);

private:
    void write(const std::string& a_str);

private:
    std::ostream& m_outStream;
    advcpp::Mutex m_mtxLogger;
};


Logger& StartLogger();

} // namespace advcpp



#endif // DBC1B3D2_3759_42BA_AFFD_D141946A9ACA__