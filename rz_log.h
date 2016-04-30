#pragma once

// http://stackoverflow.com/questions/19415845/a-better-log-macro-using-template-metaprogramming

#define LOG(msg) (Log(__FILE__, __LINE__, __func__, LogData<None>() << msg))
//#define LOG(msg)

#include <iostream>

// Workaround GCC 4.7.2 not recognizing noinline attribute
#ifndef NOINLINE_ATTRIBUTE
  #ifdef __ICC
    #define NOINLINE_ATTRIBUTE __attribute__(( noinline ))
  #else
    #define NOINLINE_ATTRIBUTE
  #endif // __ICC
#endif // NOINLINE_ATTRIBUTE

struct None { };

template<typename List>
struct LogData {
    List list;
};

template<typename List>
void Log(const char* file, int line, char const *func, LogData<List>&& data) NOINLINE_ATTRIBUTE
{
    std::cout << file << ":" << line << ": " << func << ": ";
    output(std::cout, std::move(data.list));
    std::cout << std::endl;
}

template<typename Begin, typename Value>
constexpr LogData<std::pair<Begin&&, Value&&>> operator<<(LogData<Begin>&& begin,
                                                          Value&& value) noexcept
{
    return {{ std::forward<Begin>(begin.list), std::forward<Value>(value) }};
}

template<typename Begin, size_t n>
constexpr LogData<std::pair<Begin&&, const char*>> operator<<(LogData<Begin>&& begin,
                                                              const char (&value)[n]) noexcept
{
    return {{ std::forward<Begin>(begin.list), value }};
}

typedef std::ostream& (*PfnManipulator)(std::ostream&);

template<typename Begin>
constexpr LogData<std::pair<Begin&&, PfnManipulator>> operator<<(LogData<Begin>&& begin,
                                                                 PfnManipulator value) noexcept
{
    return {{ std::forward<Begin>(begin.list), value }};
}

template <typename Begin, typename Last>
void output(std::ostream& os, std::pair<Begin, Last>&& data)
{
    output(os, std::move(data.first));
    os << data.second;
}

inline void output(std::ostream& /*os*/, None)
{
}

#define LOG_CTOR(M) LOG("CTOR: " << M)
#define LOG_DTOR(M) LOG("DTOR: " << M)

#define LOG_WARNING(M) LOG("WARNING: " << M)
#define LOG_ERROR(M) LOG("ERROR: " << M)
#define LOG_VERBOSE(M) //LOG("VERBOSE: " << M)
#define LOG_DEBUG(M) LOG("DEBUG: " << M)

#define LOG_SIGNAL(M) LOG_DEBUG("SIGNAL " << M)
