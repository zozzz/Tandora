/*
 * File:   Timer.h
 * Author: Zozzz
 *
 * Created on 2011. február 18., 1:10
 */

#ifndef TIMER_H
#define	TIMER_H

#include <ctime>

namespace common
{
	class Timer
	{
	public:
		Timer()
		{
			reset();
		}

		void reset(){ _start = std::clock(); _elapsed = 0; std::cout << "T started" << std::endl; }
		double stop()
		{
			_elapsed = (double)(std::clock() - _start);
			_elapsed /= CLOCKS_PER_SEC;
			return _elapsed;
		}
		double last(){ return _elapsed; };

	private:
		std::clock_t _start;
		double _elapsed;
	};

}

#endif	/* TIMER_H */

