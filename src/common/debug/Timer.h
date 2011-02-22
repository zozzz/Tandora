/*
 * File:   Timer.h
 * Author: Zozzz
 *
 * Created on 2011. február 18., 1:10
 */

#ifndef TIMER_H
#define	TIMER_H

#include <ctime>
#include <map>
#include <iostream>

namespace common
{
	class Timer
	{
	public:
		Timer()
		{
			reset();
		}

		void reset(){ _start = std::clock(); _elapsed = 0; }
		double stop()
		{
			_elapsed = (double)(std::clock() - _start);
			_elapsed /= CLOCKS_PER_SEC;
			return _elapsed;
		}
		double last(){ return _elapsed; };

		static void newTimer(const char* name)
		{
			if( _timers.count(name) > 0 )
				_timers[name]->reset();
			else
				_timers.insert(std::pair<const char*, Timer*>(name, new Timer()));
		}

		static Timer* getTimer(const char* name)
		{
			Timer* t = _timers[name];
			if( t != NULL )
				return t;
			return NULL;
		}

		static void printResult(const char* name)
		{
			Timer* t = getTimer(name);
			if( t == NULL )
			{
				std::cout << "Timer undefined: " << name << std::endl;
			}
			else
			{
				std::cout << "Timer[" << name << "]: " << t->stop() << std::endl;
			}
		}

	private:
		static std::map<const char*, Timer*> _timers;
		std::clock_t _start;
		double _elapsed;
	};

	std::map<const char*, Timer*> Timer::_timers;
}

#endif	/* TIMER_H */

