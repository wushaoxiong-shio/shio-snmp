#include "../include/Loger.h"

#include <sys/stat.h>
#include <time.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <nlohmann/json.hpp>

namespace shio {

Loger* Loger::SingletonPtr = nullptr;
std::mutex Loger::CMutex;
std::stringstream Loger::sstream;
std::ofstream Loger::fstream;

// 返回文件所在的目录
static std::string Dirname(const std::string& filename)
{
	if(filename.empty())
		return ".";
	auto pos = filename.rfind('/');
	if(pos == 0)
		return "/";
	else if(pos == std::string::npos)
		return ".";
	else
		return filename.substr(0, pos);
}


// lstat 查看文件属性，成功时返回0
static int __lstat(const char* file, struct stat* st = nullptr)
{
	struct stat lst;
	int ret = lstat(file, &lst);
	if(st)
		*st = lst;
	return ret;
}

// mkdir 创建目录，成功时返回0
static int __mkdir(const char* dirname)
{
	if(access(dirname, F_OK) == 0)
		return 0;
	return mkdir(dirname, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}


// 创建目录，一级一级向下创建目录
static bool Mkdir(const std::string& dirname)
{
	if(__lstat(dirname.c_str()) == 0)
		return true;
	// strdup 复制字符串，在堆上开辟，需要手动释放
	char* path = strdup(dirname.c_str());
	char* ptr = strchr(path + 1, '/');
	do
	{
		for(; ptr; *ptr = '/', ptr = strchr(ptr + 1, '/'))
		{
			*ptr = '\0';
			if(__mkdir(path) != 0)
				break;
		}
		if(ptr != nullptr)
			break;
		else if(__mkdir(path) != 0)
			break;
		free(path);
		return true;
	}
	while(0);
	free(path);
	return false;
}



Loger::Loger()
{
    loadconfig();
    reopen();
    sstream.clear();
    sstream.str("");
}

Loger::~Loger()
{
    if (fstream.is_open())
        fstream.close();
}

void Loger::log(const char *str, std::string filename, int line, std::string func)
{
    this->filename = filename;
    this->line = line;
    this->func = func;
    this->logstr = str;

    gettime();
    getlevelstr();
    gettid();
    getfilename();
    getline();
    getfunction();
    getlogstr();

    std::cout << sstream.str() << std::endl;
    fstream << sstream.str() << std::endl;

    sstream.clear();
    sstream.str("");
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

    Level = data["Log"]["Level"];
    FiledDrectory = data["Log"]["FiledDrectory"];
    FilePrefix = data["Log"]["FilePrefix"];
    Format = data["Log"]["Format"];
    MaxSize = data["Log"]["MaxSize"];

    formatfunc =
    {
        {'l', std::bind(&Loger::getlevelstr, this)},
        {'m', std::bind(&Loger::getlogstr, this)},
        {'t', std::bind(&Loger::gettid, this)},
        {'l', std::bind(&Loger::getline, this)},
        {'u', std::bind(&Loger::getfunction, this)},
        {'f', std::bind(&Loger::getfilename, this)},
        {'d', std::bind(&Loger::gettime, this)},
        {'T', std::bind(&Loger::gettab, this)},
        {'n', std::bind(&Loger::getnewline, this)},
        {' ', std::bind(&Loger::getspace, this)}

    };
}

void Loger::reopen()
{
    if (fstream.is_open())
        fstream.close();
    
    splicefilename();
    Mkdir(FiledDrectory);
    fstream.open(logfilename, std::ios::app);
}

void Loger::parselogformat()
{
    // for (int i=0; i<Format.size(); i++)
    // {
        
    // }
}

void Loger::splicefilename()
{
    time_t ts = time(0);
    struct tm mt;
    localtime_r(&ts, &mt);
    char buf[32];
    strftime(buf, 32, "%Y-%m-%d", &mt);
    std::string te = buf;
    logfilename = FiledDrectory + FilePrefix + te + ".log";
}

void Loger::getlogstr()
{
    sstream << logstr;
}

void Loger::gettime()
{
    time_t ts = time(0);
    struct tm mt;
    localtime_r(&ts, &mt);
    char buf[32];
    strftime(buf, 32, "%Y-%m-%d %H:%M:%S", &mt);
    sstream << buf;
}

void Loger::getlevelstr()
{
    sstream << Level;
}

void Loger::gettid()
{
	sstream << syscall(SYS_gettid);
}

void Loger::getfilename()
{
    filename = filename.substr(filename.find_last_of('/') + 1);
    sstream << filename;
}

void Loger::getline()
{
    sstream << line;
}

void Loger::getfunction()
{
    sstream << func;
}

void Loger::gettab()
{
    sstream << "\t";
}

void Loger::getnewline()
{
    sstream << "\n";
}

void Loger::getspace()
{
    sstream << " ";
}

};