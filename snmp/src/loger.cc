#include "../include/Loger.h"

#include <sys/stat.h>
#include <nlohmann/json.hpp>

namespace shio {

Loger* Loger::SingletonPtr = nullptr;
std::mutex Loger::CMutex;
std::ofstream Loger::fstream;

Loger::Loger()
{
    loadconfig();
    reopen();
}

Loger::~Loger()
{
    fstream.close();
}

void Loger::log(const char *logstr)
{
    std::cout << "SHIO TEST: " << logstr << std::endl;
    fstream << logstr << std::endl;
}

void Loger::loadconfig()
{
    struct stat buffer;
    if (stat("/root/code/shio-snmp/conf/config.json", &buffer) != 0)
    {
        std::cout << "Config File no Find !" << std::endl;
        exit(-1);
    }

    std::ifstream f("/root/code/shio-snmp/conf/config.json");
    nlohmann::json data = nlohmann::json::parse(f);

    for(auto i : data["log"].items())
    {
        if (strncmp(i.key().c_str(), "Level", 5) == 0)
            Level = i.value();
        
        if (strncmp(i.key().c_str(), "filename", 8) == 0)
            logfile = i.value();
        
        if (strncmp(i.key().c_str(), "Format", 6) == 0)
            format = i.value();
    }
}

void Loger::reopen()
{
    if (Loger::fstream.is_open())
        Loger::fstream.close();

    Loger::fstream.open("/root/code/shio-snmp/log/shio.log", std::ios::app);
}

};