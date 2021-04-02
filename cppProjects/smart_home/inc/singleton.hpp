#ifndef EF2C34E6_C767_46AB_AF20_C60AFA90C655_H__
#define EF2C34E6_C767_46AB_AF20_C60AFA90C655_H__

#include "noncopyable.hpp"
namespace advcpp
{

    class Mutex;

    template <typename T>
    class Singleton : private NonCopyable
    {
    public:
        static T &GetInstance();

    private:
        Singleton();

    private:
        static T *s_tInstance;
        static bool s_isFirst;
    };

} // namespace advcpp

#include "inl/singleton.hxx"

#endif // EF2C34E6_C767_46AB_AF20_C60AFA90C655_H__
