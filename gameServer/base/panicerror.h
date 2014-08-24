#pragma once

#include <string>

namespace mu
{
	class PanicError
	{
	public:
		PanicError(const std::string &);
		~PanicError();
		PanicError(const PanicError &);

	private:
		std::string m_panicerror;
	};
}