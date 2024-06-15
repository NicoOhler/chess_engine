#include "definitions.h"
#include "configuration.h"
#include "iostream"
#include <fstream>

void log(std::string message, LogType logType);

class Logger
{
private:
    static Logger *instance;
    bool print_to_console;
    std::ofstream file;
    std::string filename;
    Logger();

public:
    Logger(Logger &other) = delete;
    void operator=(const Logger &) = delete;
    static Logger *GetInstance();
    void log(std::string message, LogType logType);
    ~Logger();
};
