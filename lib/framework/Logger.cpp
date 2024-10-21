#include <cstdlib>
#include <iostream>
#include <sstream>
#include <cmath>

#include <stdarg.h>
#include <memory.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "Logger.h"

const char *
LogSink::GetHeader()
{
	static char buffer[64];

	struct timespec specific_time;
	struct tm *now;
	int millsec;

	clock_gettime(CLOCK_REALTIME, &specific_time);
	now = localtime(&specific_time.tv_sec);
	millsec = (int)floor(specific_time.tv_nsec / 1.0e6);

	memset(buffer, 0x00, sizeof(buffer));
	sprintf(buffer, "[%6d] %04d/%02d/%02d %02d:%02d:%02d.%03d ", getpid(), now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec, millsec);
	return buffer;
}

const char *
LogSink::GetCurrentDay()
{
	static char buffer[64];

	time_t nowTime;
	struct tm *now;

	time(&nowTime);
	now = localtime(&nowTime);

	memset(buffer, 0x00, sizeof(buffer));
	sprintf(buffer, "%04d%02d%02d", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday);

	return buffer;
}

void LogSink::DumpBytes(std::ostream &out, const char *p, int len, bool isUse)
{
	if (false == isUse)
		return;

	char temp[512];

	int coff, roff;
	int off;
	char szOutput[258];
	char szString[128];
	int i = 0;
	int j = 0;
	int base = 16;

	for (roff = 0; roff < (len + base - 1) / base; roff++)
	{
		i = 0;
		j = 0;
		memset(szOutput, 0x20, sizeof(szOutput));
		memset(szString, 0x20, sizeof(szString));

		sprintf(&szOutput[i], "[%04X] :", roff * base);
		i = i + 8;
		for (coff = 0; coff < base; coff++)
		{
			off = roff * base + coff;
			if (off < len)
			{
				if (base <= 10)
					sprintf(&szOutput[i], "%02d  ", (unsigned char)p[off]);
				else
					sprintf(&szOutput[i], "%02X  ", (unsigned char)p[off]);

				// Added by bklee
				if ((unsigned char)p[off] < 0x20)
					szString[j] = (unsigned char)'.';
				else
					szString[j] = (unsigned char)p[off];

				// i = i + 3 ;
				//  modified by bklee 2000.02.07
				if ((coff % 4) == 3)
					i = i + 4;
				else
					i = i + 3;

				j = j + 1;
			}
		}

		szOutput[i] = (unsigned char)0x20;
		szOutput[60] = (unsigned char)'\0';
		szString[j] = (unsigned char)'\0';

		sprintf(temp, "%-60s  %s \n", szOutput, szString);
		out << temp;
	}
}

LogSinkConsole
	LogSinkConsole::instance_ = LogSinkConsole();

void LogSinkConsole::Write(const std::string &str)
{
	buffer_ += str;
	Flush();
}

void LogSinkConsole::Write(int c)
{
	buffer_ += (char)c;
	if (c == '\n')
		Flush();
}

void LogSinkConsole::Flush()
{
	if (0 == buffer_.size())
		return;
#if 1
    buffer_ = this->GetHeader() + buffer_;
#else
	std::string temp = this->GetHeader() + buffer_;
	buffer_ = temp;
#endif

	if (buffer_[buffer_.length() - 1] != '\n')
		buffer_ += '\n';

	if (NULL != parent)
		parent->Display(buffer_);

	buffer_.clear();
}

void LogSinkConsole::Display(const std::string &str)
{
	std::cout << str;
}

LogSinkFile
	LogSinkFile::instance_ = LogSinkFile();

void LogSinkFile::Display(const std::string &str)
{
	// 쓰고 있던 LogFile이 날짜가 지났으면 로그 파일을 다시 연다.
	if (strncmp(currentDay_.c_str(), this->GetCurrentDay(), 12))
	{
		this->openIndex_ = 0; // 날짜 지났으면 인덱스 무효화
		this->OpenFile();
	}

	for (int i = 0; i < 10; ++i) // 무한루프를 방지하기 위해 10번만 돌자
	{
		struct stat fStat;
		int res = fstat(this->fd_, &fStat);
		if ((-1 == res) || (0 == fStat.st_nlink)) // 열려진 링크가 0이라면 파일이 삭제됐음
		{
			this->openIndex_ = 0; // 링크가 없어졌어도 무효화
			this->OpenFile();
		}
		else if (2 * 1000 * 1000 * 1000 < fStat.st_size) // 2007.07.24 천희완 : 파일 크기가 2G를 넘길 수 없다.
		{
			++this->openIndex_; // 현재 인덱스 증가
			this->OpenFile();
		}
		else
			break;
	}

	::write(fd_, str.c_str(), str.size());
}

bool LogSinkFile::OpenFile()
{
	this->CloseFile();

	currentDay_ = this->GetCurrentDay();
	std::string logFilename = fileName_ + "_" + currentDay_;

	if (this->openIndex_ > 0) // openIndex_가 0이상이면 파일 뒤에 인덱스를 붙인다
	{
		char temp[16];
		memset(temp, 0x00, sizeof(temp));
		sprintf(temp, ".%04d", this->openIndex_);

		logFilename += temp;
	}
	logFilename += ".log";

	fd_ = ::open(logFilename.c_str(), O_CREAT | O_RDWR | O_APPEND, 0644);
	if (-1 == fd_)
	{
		std::cerr << "Fail to open log fd!! [" << logFilename << "]" << std::endl;
		return false;
	}

	return true;
}

void LogSinkFile::CloseFile()
{
	if (-1 != fd_)
	{
		::close(fd_);
		fd_ = -1;
	}
}

////////////////////////////////////////////////////////////////////////////////
//
//  전역 로그 객체 정의
//
/*
LogSinks gLogSinks(&LogSinkConsole::instance_);
LogBuffer<char> gLogBuf(&gLogSinks);
std::ostream gLog(&gLogBuf);
*/

// TMutex	gLogMutex;

/**
 * C 형태의 함수로 로그를 출력합니다.
 * NSLog()형태로 define되어서 사용하며 __FILE__, __LINE__을 추가하여 로그를 출력하기 위함입니다.
 * hooper.h에서 정의되어 사용합니다.
 *
 * @param fmt
 * @param ...
 */
/**
extern "C" void _nslog(char *fmt, ...)
{
	va_list list;
	static char buffer[4096];

	// 가변 파라미터 메시지 처리

	memset(buffer, 0x00, sizeof(buffer));

	va_start(list, fmt);
	vsnprintf(buffer, sizeof(buffer) - 1, fmt, list);
	va_end(list);

	gLog << buffer << std::endl;
}
*/