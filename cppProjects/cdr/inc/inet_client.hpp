#ifndef K80C10B8_0D74_419F_8F1A_BFD16AD931F1_H__
#define K80C10B8_0D74_419F_8F1A_BFD16AD931F1_H__

namespace advcpp
{
class INetClient
{
public:
    virtual ~INetClient() = 0;

    virtual void Send(const char* a_buffer) const = 0;
private:

};

inline INetClient::~INetClient()
{
    
}

} // namespace advcpp

#endif // K80C10B8_0D74_419F_8F1A_BFD16AD931F1_H__