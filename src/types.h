#ifndef TYPES_H
#define TYPES_H

#include <utility>

namespace Gui
{

	typedef unsigned int	uint32;
	typedef unsigned short	uint16;
	typedef unsigned char	uint8;
	typedef signed int		sint32;
	typedef signed short	sint16;
	typedef signed char		sint8;
	typedef unsigned char	byte;
	typedef void (*CallbackFunction)(uint32);
	typedef std::pair<CallbackFunction, uint32> CallbackType;

#define NoCallback	std::make_pair<CallbackFunction, uint32>(NULL, 0)
#define MkCallback(fn, param) std::make_pair<CallbackFunction, uint32>(fn, param)

	template<class T>
	inline T clamp(const T &v, const T &m, const T &M)
	{
		return v < m ? m : v > M ? M : v;
	}
}

#endif // TYPES_H
