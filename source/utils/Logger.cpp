#include "Logger.h"

#if HIGHLIGHTING_LOGS
#include "termcolor/termcolor.hpp"

using namespace termcolor;

#endif //HIGHLIGHTING_LOGS

namespace v3d
{
namespace utils
{

const std::string k_loggerType[ELoggerType::eLoggerCount] =
{
    "",
    "DEBUG",
    "INFO",
    "WARNING",
    "ERROR"
};

#if HIGHLIGHTING_LOGS

using colorFunc = std::ostream& (*)(std::ostream& stream);

const colorFunc k_colorList[ELoggerType::eLoggerCount] =
{
    green,
    white,
    green,
    yellow,
    red
};


#endif //HIGHLIGHTING_LOGS

CLogger::CLogger()
    : m_logFilename("logfile.log")
{
    m_file.open(m_logFilename, std::ofstream::out);
    m_file.clear();
    m_file.close();
}

CLogger::~CLogger()
{
    m_file.close();
}

void CLogger::createLogFile(const std::string& fileName)
{
    m_logFilename = fileName;

    m_file.open(m_logFilename, std::ofstream::out);
    m_file.clear();
    m_file.close();
}

void CLogger::log(ELoggerType type, ELogOut out, const char* format, ...)
{
    char buffer[1024];

    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    std::string message;
    message.assign(buffer);

    log(message, type, out);
}

void CLogger::log(const std::string& message, ELoggerType type, ELogOut out)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    switch (out)
    {
    case ELogOut::eConsoleLog:
    {
        logToConsole(message, type);
        break;
    }

    case ELogOut::eFileLog:
    {
        logToFile(message, type);
        break;
    }
        
    case ELogOut::eBothLog:
    {
        logToConsole(message, type);
        logToFile(message, type);
        break;
    }

    default:
        break;
    }
}

void CLogger::destroyLogFile()
{
    if (m_file.is_open())
    {
        m_file.close();
    }

    std::remove(m_logFilename.c_str());
}

void CLogger::logToConsole(const std::string& message, ELoggerType type)
{
    if (type == ELoggerType::eLoggerDebug)
    {
#if (defined(_DEBUG) || defined(DEBUG)) && defined(USE_DEBUG_LOG)
#   if HIGHLIGHTING_LOGS
        std::cout << k_colorList[type] << k_loggerType[eLoggerDebug] << ": " << message << reset << std::endl;
#   else //HIGHLIGHTING_LOGS
        std::cout << k_loggerType[eLoggerDebug] << ": " << message << std::endl;
#   endif //HIGHLIGHTING_LOGS
#endif //_DEBUG
    }
    else
    {
#if HIGHLIGHTING_LOGS
        std::cout << k_colorList[type] << k_loggerType[type] << ": " << message << reset << std::endl;
#else //HIGHLIGHTING_LOGS
        std::cout << k_loggerType[type] << ": " << message << std::endl;
#endif //HIGHLIGHTING_LOGS
    }
}

void CLogger::logToFile(const std::string& message, ELoggerType type)
{
    m_file.open(m_logFilename, std::ofstream::out | std::ofstream::app);

    if (type == ELoggerType::eLoggerDebug)
    {
#if (defined(_DEBUG) || defined(DEBUG)) && defined(USE_DEBUG_LOG)
        m_file << k_loggerType[eLoggerDebug] << ": " << message << std::endl;
#endif
    }
    else
    {
        m_file << k_loggerType[type] << ": " << message << std::endl;
    }

    m_file.close();
}

} //namespace utils
} //namespace v3d
