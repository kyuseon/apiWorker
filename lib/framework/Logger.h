#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include <vector>
#include <memory>
#include <sstream>

class LogSinks;
class LogSink
{
public:
	LogSink(LogSinks *arg = NULL)
	{
		this->parent = arg;
	};
	virtual ~LogSink() {};

	virtual void Write(const std::string &str) = 0;
	virtual void Write(int c) = 0;
	virtual void Display(const std::string &str) = 0;
	virtual void Flush() {}
	static void DumpBytes(std::ostream &out, const char *p, int len, bool isUse = true);

public:
	void SetParent(LogSinks *arg)
	{
		this->parent = arg;
	};

protected:
	const char *GetHeader();
	const char *GetCurrentDay();

	LogSinks *parent; //!< 부모 sink
};

/// 로그를 Console에 뿌려주는 Sink
class LogSinkConsole : public LogSink
{
public:
	static LogSinkConsole instance_;

	virtual void Write(const std::string &str);
	virtual void Write(int c);
	virtual void Flush();

	inline static LogSinkConsole *GetDefaultSink() { return &instance_; }

protected:
	virtual void Display(const std::string &str);

	LogSinkConsole() {};
	virtual ~LogSinkConsole() {};

	std::string buffer_;
};

class LogSinkFile : public LogSinkConsole
{
public:
	static LogSinkFile instance_;

public:
	LogSinkFile(const std::string &fileName) : fd_(-1), openIndex_(0)
	{
		setFileName(fileName);
	};
	virtual ~LogSinkFile()
	{
		CloseFile();
	};

public:
	void setFileName(const std::string &fileName)
	{
		fileName_ = fileName;
		OpenFile();
	}

protected:
	virtual void Display(const std::string &str);

	LogSinkFile() : fd_(-1), openIndex_(0) {} // outFile_(NULL) {}

	bool OpenFile();
	void CloseFile();

protected:
	int fd_;
	int openIndex_; // 2007.07.24 천희완 : 파일 크기가 2G이상이면 openIndex_를 증가시켜 새 파일을 연다.

	std::string fileName_;
	std::string currentDay_;
};

class LogSinks
{
public:
	LogSinks(LogSink *defaultSink = NULL)
	{
		this->addLogSink(defaultSink);
	};
	virtual ~LogSinks()
	{
		std::vector<LogSink *>::iterator it = logSinks_.begin();
		if (it != logSinks_.end())
		{
			(*it)->Flush();
		}

		for (; it != logSinks_.end(); ++it)
		{
			if (*it == &LogSinkConsole::instance_)
				continue;
			if (*it == &LogSinkFile::instance_)
				continue;

			try
			{
				std::unique_ptr<LogSink> mustDelete(*it);
			}
			catch (...)
			{
			}
			*it = NULL;
		}
	}
	void Write(int c)
	{
		std::vector<LogSink *>::const_iterator it = logSinks_.begin();
		if (it != logSinks_.end())
		{
			(*it)->Write(c);
		}
	}
	void Display(const std::string &str)
	{
		std::vector<LogSink *>::const_iterator it = logSinks_.begin();
		for (; it != logSinks_.end(); ++it)
		{
			(*it)->Display(str);
		}
	}
	void addLogSink(LogSink *arg)
	{
		if (NULL == arg)
			return;

		if (0 == logSinks_.size())
		{
			arg->SetParent(this); // 맨 처음 sink는 Default
		}
		logSinks_.push_back(arg);
	}
	std::size_t size() { return logSinks_.size(); }

private:
	std::vector<LogSink *> logSinks_;
};

template <class T, class Tr = std::char_traits<T>, class A = std::allocator<T>>
class LogBuffer : public std::basic_stringbuf<T, Tr, A>
{
public:
	LogBuffer(LogSinks *arg = NULL)
	{
		logSinks_ = arg;
	};
	virtual ~LogBuffer() {};

	typedef typename std::basic_stringbuf<T, Tr, A>::int_type int_type;
	int_type overflow(int_type c)
	{
		if (c != std::string::traits_type::eof())
		{
			if (NULL != logSinks_)
				logSinks_->Write(c);
		}
		return std::string::traits_type::not_eof(c);
	}
	void setLogSinks(LogSinks *arg)
	{
		logSinks_ = arg;
	}

private:
	LogSinks *logSinks_;
};

/**
////////////////////////////////////////////////////////////////////////////////
//
//  전역 로그 객체 선언
//

#define _LOG(x)                               \
	{                                         \
		TPThread::TAutoLock lock(&gLogMutex); \
		x;                                    \
	}

class TMutex; //!	로그 출력을 위한 mutex

extern LogSinks gLogSinks;			//!	전역 로그 싱크 콜렉션
extern LogBuffer<char> gLogBuf; //! 전역 버퍼
extern std::ostream gLog;			//! 전역 스트림

// extern TMutex gLogMutex;

// 2016.10.14 ssh 로그 출력 함수
extern "C" void _nslog(char *fmt, ...);
*/

#endif // __LOGGER_H__
