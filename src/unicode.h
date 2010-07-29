#ifndef UNICODE_H
#define UNICODE_H

#include <string>

namespace Gui
{
	std::string toUtf8(const std::wstring &wstr);
	inline const std::string &toUtf8(const std::string &str)	{	return str;	}
	std::wstring fromUtf8(const std::string &str);
	inline const std::wstring &fromUtf8(const std::wstring &wstr)	{	return wstr;	}

	class gwstring : public std::wstring
	{
	public:
		gwstring()	{}
		gwstring(const std::string &s) : std::wstring(fromUtf8(s))	{}
		gwstring(const char *s) : std::wstring(fromUtf8(s))	{}
		gwstring(const std::wstring &s) : std::wstring(s)	{}
		gwstring(const wchar_t *s) : std::wstring(s)	{}

		operator std::string() const {	return toUtf8(*this);	}
		operator std::wstring() const {	return *this;	}
	};
}

#endif // UNICODE_H
