#include "Logger.h"

Logger *Logger::instance = nullptr;

Logger::Logger()
{
    print_to_console = LOG_TO_CONSOLE;
    filename = LOG_FILE;

    // ? does this delete the file if it exists?
    if (APPEND_TO_FILE)
        file.open(filename, std::ios_base::app);
    else
        file.open(filename);
}
Logger::~Logger()
{
    // close if open
    if (file.is_open())
        file.close();
}

Logger *Logger::GetInstance()
{
    if (!instance)
        instance = new Logger();
    return instance;
}

// todo rewrite to check if logType is active and only then start building the string
// #define ...
void Logger::log(LogType logType, std::string message)
{
    // todo print logType
    if (print_to_console)
        std::cout << message << std::endl;

    if (file.is_open())
        file << message << std::endl;
}

void log(LogType logType, std::string message)
{
    if (logType & LOG_TYPE_ENABLED)
        Logger::GetInstance()->log(logType, message);
}
