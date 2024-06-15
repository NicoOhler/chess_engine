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

void Logger::log(std::string message, LogType logType)
{
    if (logType.active)
    {
        if (print_to_console)
            std::cout << logType.name << ": " << message << std::endl;

        if (file.is_open())
            file << logType.name << ": " << message << std::endl;
    }
}

void log(std::string message, LogType logType)
{
    Logger::GetInstance()->log(message, logType);
}
