/*
 * File:   PerformanceTest.h
 * Author: Zozzz
 *
 * Created on 2011. február 18., 17:18
 */

#ifndef PERFORMANCETEST_H
#define	PERFORMANCETEST_H

#include "Timer.h"

#define TIMER_START(name) { common::Timer::newTimer(#name);
#define TIMER_N_START(name, nTimes) common::Timer::newTimer(#name); for(int x_##name = 0 ; x_##name < (nTimes) ; x_##name ++ ) {
#define TIMER_END(name) } common::Timer::printResult(#name);

#endif	/* PERFORMANCETEST_H */

