#include "logging/Logging.h"

namespace epsilon
{
	LogListener::LogListener()
	{
		logName = "";
	}

	LogListener::LogListener(string name)
	{
		logName = name;
	}

	string LogListener::GetLogName()
	{
		return logName;
	}

	void LogStream::Log(string content)
	{
		content = this->name + ": " + content + "\n";
		printf(content.c_str());

		if ( !listeners.empty() )
		{
			for (LogListenerIterator i = listeners.begin(); i != listeners.end(); i++ )
			{
				(*i)->Log(content);
			}
		}
	}

	LogStream::Ptr LogStream::Create(string logName)
	{
		return make_shared<LogStream>(private_struct(), logName);
	}

	void LogStream::AddListener(LogListener::Ptr newListener)
	{
		listeners.push_back(newListener);
	}

	void LogStream::RemoveListener(LogListener::Ptr rmListener)
	{
		listeners.remove(rmListener);
	}

	Logger::Logger()
	{
		logs[string("default")] = LogStream::Create("default");
		listeners = new LogListenerList;
	}

	Logger::~Logger()
	{
		delete listeners;
	}

	void Logger::Log(string content)
	{
		Logger::Log(string("default"), content);
	}

	void Logger::Log(string logName, string content)
	{
		LogListener::Ptr listener;
		Logger::GetLog(logName)->Log(content);

		LogListenerList * listeners = Logger::getInstance().listeners;
		if ( !listeners->empty() )
		{
			for (LogListenerIterator i = listeners->begin(); i != listeners->end(); i++ )
			{
				listener = (*i);
				listener->Log(logName, content);
			}
		}
	}

	LogStream::Ptr Logger::GetLog(string loggerName)
	{
		LogStreamMap logs = Logger::getInstance().logs;
		LogStream::Ptr theLog;
	
		LogMapIterator foundLog = logs.find(loggerName);

		if ( foundLog != logs.end() )
		{
			theLog = foundLog->second;
		}
		else
		{
			theLog = LogStream::Create(loggerName);
			logs[loggerName] = theLog;
		}
		return theLog;
	}

	void Logger::addListener(LogListener::Ptr newListener)
	{
		LogListenerList * listeners = Logger::getInstance().listeners;
		LogStreamMap logs = Logger::getInstance().logs;
		LogStream::Ptr log;
		
		if ( newListener->GetLogName() == "" )
		{
			listeners->push_back(newListener);
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
		LogListenerList * listeners = Logger::getInstance().listeners;
		LogStreamMap logs = Logger::getInstance().logs;
		LogStream::Ptr log;

		if ( rmListener->GetLogName() == "" )
		{
			listeners->remove(rmListener);
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

	// Namespace functions
	void Log(const char * content)
	{ 
		Logger::Log(string(content)); 
	}

	void Log(const char* logName, const char* content)
	{ 
		Logger::Log(string(logName), string(content)); 
	}

	void Log(string content)
	{
		Logger::Log(content);
	}

	void Log(string logName, string content)
	{
		Logger::Log(logName, content);
	}
}
