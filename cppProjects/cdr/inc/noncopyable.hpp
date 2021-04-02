#ifndef NONCOPYABLE_H__
#define NONCOPYABLE_H__

namespace advcpp
{
class NonCopyable
{
public:
    NonCopyable();
private:
    NonCopyable(const NonCopyable&);
    NonCopyable& operator=(const NonCopyable&);
};

} // advcpp

#endif // NONCOPYABLE_H__
