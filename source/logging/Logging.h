#pragma once

#include "EpsilonCore.h"

namespace epsilon
{
	using namespace std;

	class LogListener
	{
	public:
		typedef shared_ptr<LogListener> Ptr;

		// These will be implemented by derived classes
		//static Ptr Create();
		//static Ptr Create(string name);
		
		string GetLogName();

		// Functions that must be overridden by derived classes
		virtual void Log(string content) = 0 ;
		virtual void Log(string logName, string content) = 0;
	protected:
		LogListener();
		LogListener(string name);
	private:
		string logName;
	};

	typedef std::list<LogListener::Ptr> LogListenerList;
	typedef LogListenerList::iterator LogListenerIterator;

	class LogStream
	{
	private:
		struct private_struct {};

	public:
		typedef shared_ptr<LogStream> Ptr;

		static Ptr Create(string logName);

		void AddListener(LogListener::Ptr newListener);
		void RemoveListener(LogListener::Ptr rmListener);

		void Log(string content);

		LogStream(const private_struct &, string logName)
		{
			name = logName;
		}

	private:		
		string name;
		LogListenerList listeners;
	};

	typedef map<string, LogStream::Ptr> LogStreamMap;
	typedef LogStreamMap::iterator LogMapIterator;
	typedef std::list<std::string> LogList;

	class Logger
	{
	public:
		static void Log(string content);
		static void Log(string logName, string content);
		
		static void addListener(LogListener::Ptr newListener);
		static void removeListener(LogListener::Ptr rmListener);

		// Used by the UI to ensure in-engine console is accurate
		static LogList FlushInitLog();

	private:
		Logger();
		~Logger();
		static LogStream::Ptr GetLog(string loggerName);

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

	void Log(string content);
	void Log(string logName, string content);
}