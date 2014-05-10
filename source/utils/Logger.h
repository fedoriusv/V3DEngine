#ifndef _F3D_LOGGER_H_
#define _F3D_LOGGER_H_

#include "common.h"
#include "Singleton.h"

namespace f3d
{
namespace utils
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	enum ELoggerType
	{
		eLoggerDebug,
		eLoggerInfo,
		eLoggerWarning,
		eLoggerError,

		eLoggerCount
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	enum class ELogOut
	{
		eConsoleLog,
		eFileLog,
		eBothLog
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	class CLogger : public Singleton<CLogger>
	{
	public:
							CLogger();
		virtual				~CLogger();

		void				createLogFile (const std::string& filename);
		
		void				log(const std::string& message, ELoggerType type = ELoggerType::eLoggerInfo, ELogOut out = ELogOut::eConsoleLog);
		void				log(ELoggerType type, ELogOut out, const char* format, ...);

		void				destroyLogFile();
		
	protected:

		void				logToConsole(const std::string& message, ELoggerType type);
		void				logToFile(const std::string& message, ELoggerType type);

		std::string			m_logFilename;
		std::ofstream		m_file;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////

#if USE_LOGGER
#	define LOG_GEBUG(messages, ...) f3d::utils::CLogger::getInstance()->log(f3d::utils::ELoggerType::eLoggerDebug, f3d::utils::ELogOut::eConsoleLog, messages, __VA_ARGS__);
#	define LOG_INFO(messages, ...) f3d::utils::CLogger::getInstance()->log(f3d::utils::ELoggerType::eLoggerInfo, f3d::utils::ELogOut::eConsoleLog, messages, __VA_ARGS__);
#	define LOG_WARRNING(messages, ...) f3d::utils::CLogger::getInstance()->log(f3d::utils::ELoggerType::eLoggerWarning, f3d::utils::ELogOut::eConsoleLog, messages, __VA_ARGS__);
#	define LOG_ERROR(messages, ...) f3d::utils::CLogger::getInstance()->log(f3d::utils::ELoggerType::eLoggerError, f3d::utils::ELogOut::eConsoleLog, messages, __VA_ARGS__);
#else
#	define LOG_GEBUG(messages, ...)
#	define LOG_INFO(messages, ...)
#	define LOG_WARRNING(messages, ...)
#	define LOG_ERROR(messages, ...)
#endif

	/////////////////////////////////////////////////////////////////////////////////////////////////////
}
}
#endif //_F3D_LOGGER_H_