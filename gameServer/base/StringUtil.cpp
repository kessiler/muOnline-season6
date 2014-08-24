#include "StdAfx.h"
#include <string>
#include <vector>

// strange npos compilation ? ... 

namespace mu
{
namespace StringUtil
{

static void Split(std::string& s,std::string& delimeter,std::vector<std::string>& rv)
{
	std::string l(s);

	unsigned int pos = l.find_first_of(delimeter,0);

	while( pos != std::string::npos )
	{
		rv.push_back(l.substr(0,pos));
		l = l.substr(pos+1,std::string::npos);
		pos = l.find_first_of(delimeter,0);
	}

	if( l.length() > 0 )
		rv.push_back(l);
}

static void Trim(std::string& s,std::string& r)
{
	unsigned int first = s.find_first_not_of("\t\r\n");
	unsigned int last = s.find_last_not_of("\t\r\n");
	r = s.substr(first,last-first+1);
}

static void Trim(std::string& s)
{
	unsigned int first = s.find_first_not_of("\t\r\n");
	unsigned int last = s.find_last_not_of("\t\r\n");

	if( first != 0 )
		s.erase(0,first);

	if( last != std::string::npos )
		s.erase(last+1,std::string::npos);
}

static int ToInteger(std::string& s)
{
	return atoi(s.c_str());
}

static const std::string FromInteger(int v)
{
	char c[128];
	memset(c,0,sizeof(c));
	itoa(v,c,10);
	std::string o;
	o.append(c);
	return std::string(o);
}

static double ToDouble(std::string& s)
{
	return atof(s.c_str());
}

static void ToLower(std::string& s,std::string& out)
{
	char c = 0;
	const char* cs = s.c_str();

	for(int i = 0; i < s.length(); i++)	{
		c = (const char)tolower(cs[i]);
		out.append(&c);
	}
}

static void ToUpper(std::string& s,std::string& out)
{
	char c = 0;
	const char* cs = s.c_str();

	for(int i = 0; i < s.length(); i++)	{
		c = (const char)toupper(cs[i]);
		out.append(&c);
	}
}

static void ConvertToSystemCode(std::string& s,std::string& out)
{
	out = "";
	out.append(s);
}

static void ConvertToMBCS(std::string& s,std::string& out)
{
	out = "";
	out.append(s);
}

static const std::string ToString(int v)
{
	char buf[128];
	memset(buf,0,sizeof(buf));
	sprintf(buf,"%d",v);
	std::string s;
	s.append(buf);
	return std::string(s);
}

static const std::string GetIp(std::string& s)
{
	std::vector<std::string> v;
	std::string d(":");
	mu::StringUtil::Split(s,d,v);

	if( v.size() < 2 )
		return std::string("null");

return v[0];
}

static int GetPort(std::string& s)
{
	std::vector<std::string> v;
	std::string d(":");
	mu::StringUtil::Split(s,d,v);

	if( v.size() < 2 )
		return 0;

	return atoi(v[1].c_str());
}


}
}

void testString()
{
	std::string s;
	mu::StringUtil::GetPort(s);
}