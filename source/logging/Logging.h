#pragma once

#include "EpsilonCore.h"

namespace epsilon
{
	class LogListener
	{
	public:
		typedef std::shared_ptr<LogListener> Ptr;

		// These will be implemented by derived classes
		//static Ptr Create();
		//static Ptr Create(string name);
		
		std::string GetLogName();

		// Functions that must be overridden by derived classes
		virtual void Log(std::string content) = 0 ;
		virtual void Log(std::string logName, std::string content) = 0;
	protected:
		LogListener();
		LogListener(std::string name);
	private:
		std::string logName;
	};

	typedef std::list<LogListener::Ptr> LogListenerList;
	typedef LogListenerList::iterator LogListenerIterator;

	class LogStream
	{
	private:
		struct private_struct {};

	public:
		typedef std::shared_ptr<LogStream> Ptr;

		static Ptr Create(std::string logName);

		void AddListener(LogListener::Ptr newListener);
		void RemoveListener(LogListener::Ptr rmListener);

		void Log(std::string content);

		LogStream(const private_struct &, std::string logName)
		{
			name = logName;
		}

	private:		
		std::string name;
		LogListenerList listeners;
	};

	typedef std::map<std::string, LogStream::Ptr> LogStreamMap;
	typedef LogStreamMap::iterator LogMapIterator;
	typedef std::list<std::string> LogList;

	class Logger
	{
	public:
		static void Log(std::string content);
		static void Log(std::string logName, std::string content);
		
		static void addListener(LogListener::Ptr newListener);
		static void removeListener(LogListener::Ptr rmListener);

		// Used by the UI to ensure in-engine console is accurate
		static LogList FlushInitLog();

	private:
		Logger();
		~Logger();
		static LogStream::Ptr GetLog(std::string loggerName);

		static Logger& getInstance(void)
		{
			static Logger instance;
			return instance;
		}

		// Do not implement these functions to prevent access outside 
		// of getInstance
		Logger(Logger const&);
		void operator=(Logger const&);

		LogStreamMap logs;
		LogListenerList * listeners;

		LogList initLog;
		bool logInit;
	};

	// Declaring log function for epsilon classes
	void Log(const char* content);
	void Log(const char* logName, const char* content);

	void Log(std::string content);
	void Log(std::string logName, std::string content);
}