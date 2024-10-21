#ifndef __CONNECT_H__
#define __CONNECT_H__

class Connect
{
public:
public:
	Connect();
	virtual ~Connect();
public:
	Connect(int fd);

private:
	Connect(const Connect& rhs) {};
	Connect& operator=(const Connect& rhs) { return *this; }
};

#endif