#pragma once

#include <iostream>
#include <fstream>
#include <memory.h>
#include <string>
#include <thread>
#include <mutex>

#define LOG(X) \
    do \
    {\
        shio::Loger::getLogerptr()->log(X, __FILE__, __LINE__, __FUNCTION__);\
    }\
    while(0)


namespace shio
{

enum LOGLevel
{
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    UNKNOW
};

static std::string LogleveltoString(LOGLevel level)
{
    switch (level)
    {
        case TRACE: {return "TRACE"; break;}
        case DEBUG: {return "DEBUG"; break;}
        case INFO:  {return "INFO"; break;}
        case WARN:  {return "WARN"; break;}
        case ERROR: {return "ERROR"; break;}
        case FATAL: {return "FATAL"; break;}
        default: {return "UNKNOW"; break;}
    }
}

class Loger
{
public:
    static Loger* SingletonPtr;
    static std::mutex CMutex;

    static Loger* getLogerptr()
    {
        if (SingletonPtr == nullptr)
        {
            CMutex.lock();
            if (SingletonPtr == nullptr)
                SingletonPtr = new Loger;

            CMutex.unlock();
        }
        return SingletonPtr;
    }

    void log(const char *logstr, std::string filename, int line, std::string func);

    void loadconfig();

    void reopen();

    void gettime();

    void getlevelstr();

    void gettid();

    void getfilename();

    void getline();

    void getfunction();

private:
    Loger();
    ~Loger();
    Loger(const Loger&) = delete;
    Loger& operator=(const Loger&) = delete;
    Loger& operator=(const Loger&&) = delete;

    std::string filename;
    int line;
    std::string func;

    std::string Level;
    std::string FiledDrectory;
    std::string FilePrefix;
    std::string Format;
    unsigned int MaxSize;
    static std::stringstream sstream;
    static std::ofstream fstream;

};

}