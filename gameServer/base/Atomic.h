#pragma once

namespace mu
{
	template<typename _A>
	class Atomic
	{
		_A volatile m_a;
	public:
		Atomic(const _A& a) { InterlockedExchange((volatile _A *)(&m_a),a); };
		~Atomic() {};
		_A Get() {
			_A a = m_a;
			InterlockedExchange((volatile _A *)(&m_a),a);
			return a;
		}
		
		_A& operator= (const _A& a)	{
			InterlockedExchange((volatile _A *)(&m_a),a);
			return (_A&)(m_a);
		}
	};
}