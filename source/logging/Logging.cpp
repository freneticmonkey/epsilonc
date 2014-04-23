#include "logging/Logging.h"

#ifdef _MSC_VER 
	#include <intrin.h>
	#include <Windows.h>
#endif

namespace epsilon
{
	LogListener::LogListener()
	{
		logName = "";
	}

	LogListener::LogListener(std::string name)
	{
		logName = name;
	}

	std::string LogListener::GetLogName()
	{
		return logName;
	}

	void LogStream::Log(std::string content)
	{
		content = this->name + ": " + content + "\n";
		printf("%s",content.c_str());
		fflush(stdout);

#ifdef _DEBUG
#ifdef _MSC_VER
		// If using Visual Studio in Debug mode. also print to the Visual Studio Window.
		if (IsDebuggerPresent())
		{
			OutputDebugStringA(content.c_str());
		}
#endif
#endif

        std::for_each(listeners.begin(), listeners.end(), [&](LogListener::Ptr listener){
            listener->Log(content);
        });
	}

	LogStream::Ptr LogStream::Create(std::string logName)
	{
		return std::make_shared<LogStream>(private_struct(), logName);
	}

	void LogStream::AddListener(LogListener::Ptr newListener)
	{
		listeners.push_back(newListener);
	}

	void LogStream::RemoveListener(LogListener::Ptr rmListener)
	{
        listeners.erase(std::remove(listeners.begin(), listeners.end(), rmListener), listeners.end());
	}

	Logger::Logger() : logInit(true)
	{
		logs[std::string("default")] = LogStream::Create("default");
	}

	Logger::~Logger()
	{
        printf("Warning: Logger destroyed.\n");
		fflush(stdout);
	}

	void Logger::Log(std::string content)
	{
		Log(std::string("default"), content);
	}

	void Logger::Log(std::string logName, std::string content)
	{
		LogListener::Ptr listener;
		GetLog(logName)->Log(content);

		if ( Logger::getInstance().logInit )
		{
			Logger::getInstance().initLog.push_back(logName + ": " + content);
		}
        
        LogListenerList * listeners = &Logger::getInstance().listeners;
        
        std::for_each(listeners->begin(), listeners->end(), [&](LogListener::Ptr listener){
            listener->Log(logName, content);
        });
	}

	LogStream::Ptr Logger::GetLog(std::string loggerName)
	{
		LogStream::Ptr theLog;
	
		LogMapIterator foundLog = logs.find(loggerName);

		if ( foundLog != logs.end() )
		{
			theLog = foundLog->second;
		}
		else
		{
            // FIXME: This should be a separate CreateLog(xxx) function
			theLog = LogStream::Create(loggerName);
			logs[loggerName] = theLog;
		}
		return theLog;
	}

	void Logger::addListener(LogListener::Ptr newListener)
	{
		LogStream::Ptr log;
		
		if ( newListener->GetLogName() == "" )
		{
			listeners.push_back(newListener);
		}
		else
		{
			log = logs[newListener->GetLogName()];
			if (log)
			{
				log->AddListener(newListener);
			}
		}
	}

	void Logger::removeListener(LogListener::Ptr rmListener)
	{
        LogStream::Ptr log;

		if ( rmListener->GetLogName() == "" )
		{
            listeners.erase(std::remove(listeners.begin(), listeners.end(), rmListener), listeners.end());
//			listeners->remove(rmListener);
		}
		else
		{
			log = logs[rmListener->GetLogName()];
			if (log)
			{
				log->RemoveListener(rmListener);
			}
		}
	}

	LogList Logger::FlushInitLog()
	{
		// Disable further logging of the init
		Logger::getInstance().logInit = false;
		// Return the init log to the caller
		return Logger::getInstance().initLog;
	}

	// Namespace functions
	void Log(const char * content)
	{ 
		Logger::getInstance().Log(std::string(content));
	}

	void Log(const char* logName, const char* content)
	{ 
		Logger::getInstance().Log(std::string(logName), std::string(content));
	}

	void Log(std::string content)
	{
		Logger::getInstance().Log(content);
	}

	void Log(std::string logName, std::string content)
	{
		Logger::getInstance().Log(logName, content);
	}
}
