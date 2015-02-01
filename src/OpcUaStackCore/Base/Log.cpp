#include "OpcUaStackCore/Base/Log.h"
#include <iostream>

namespace OpcUaStackCore
{

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// LogHandle
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	LogHandle::LogHandle(void)
	: logHandleName_("")
	{
	}

	LogHandle::LogHandle(const std::string& logHandleName)
	: logHandleName_(logHandleName)
	{
	}

	LogHandle::~LogHandle(void)
	{
	}

	void
	LogHandle::logHandleName(const std::string& logHandleName)
	{
		logHandleName_ = logHandleName;
	}

	std::string
	LogHandle::logHandleName(void)
	{
		return logHandleName_;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// Log
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	LogIf* Log::logIf_ = nullptr;
	
	void 
	Log::logIf(LogIf* logIf)
	{
		logIf_ = logIf;
	}

	LogIf*
	Log::logIf(void)
	{
		return logIf_;
	}
	
	bool 
	Log::logout(LogLevel logLevel, const std::string& message)
	{
		if (logIf_ == nullptr) return false;
		return logIf_->logout(logLevel, message);
	}

	Log::Log(LogLevel logLevel, const std::string& message)
	: logLevel_(logLevel)
	, message_(message)
	, parameter_("")
	, logHandle_(nullptr)
	{
	}

	Log::~Log(void)
	{
		if (logIf() != nullptr) {
			std::string message = message_;
			if (parameter_ != "") {
				message += " : " + parameter_;
			}
			if (logIf_->logout(logLevel_, message)) {
				return;
			}
		}

		std::cout << logLevel() << " " << message_;
		if  (parameter_ != "") {
			std::cout << ": " << parameter_;
		}
		std::cout << std::endl; 
	}

	std::string 
	Log::logLevel(void)
	{
		switch (logLevel_)
		{
			case Error:		return "ERR";
			case Warning:	return "WRN";
			case Info:		return "INF";
			case Debug:		return "DBG";
			case Trace:		return "TRC";
			default:		return "UKN";
		}
		return "";
	}

	void 
	Log::format(const std::string& parameterName, const std::string& parameterValue)
	{
		if (parameter_ != "") {
			parameter_.append(", ");
		}
		parameter_.append(parameterName);
		parameter_.append("=<");
		parameter_.append(parameterValue);
		parameter_.append(">");
	}

}
