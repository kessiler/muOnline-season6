#pragma

#include "lock.h"

namespace mu
{
	class Noncopyable
	{
	protected:
		Noncopyable() {};
		~Noncopyable() {};
	};
}