#pragma once
#include <chrono>
#include <iostream>

class _debug_timer_scope
{
public:
	std::chrono::system_clock::time_point startTimePoint;

	_debug_timer_scope(const char* functionName,int line) 
	{
		std::cout <<line << " : " <<functionName << std::endl;
		startTimePoint = std::chrono::system_clock::now();
	}

	~_debug_timer_scope()
	{
		std::chrono::system_clock::time_point endTimePoint;
		endTimePoint = std::chrono::system_clock::now();

		std::chrono::duration<double> sec = endTimePoint - startTimePoint;

		std::cout << "End Timer : " << sec.count() << std::endl;
	}
};

#define _CONCAT_0_(a, b) a##b
#define _CONCAT_(a, b) _CONCAT_0_(a, b)
#define _ANALYZE_() _debug_timer_scope _CONCAT_(__analyze, __COUNTER__)##(__FUNCTION__,__LINE__)