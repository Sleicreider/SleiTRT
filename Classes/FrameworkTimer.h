//
//  FrameworkTimer.h
//  Hazelnut
//
//  Created by Dominik Reisner on 13.11.14.
//
//

#ifndef __Hazelnut__FrameworkTimer__
#define __Hazelnut__FrameworkTimer__

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <time.h>
#include <chrono>

using milliseconds =  std::chrono::milliseconds;
using seconds = std::chrono::seconds;

class FrameworkTimer
{
    
public:
    /**
     * Get current time in seconds.
     */
    int32_t GetCurrentTimeInSeconds() const;
    
    /**
     * Get current time in milliseconds.
     * @return milliseconds Type std::chrono::milliseconds
     */
    milliseconds GetCurrentTimeInMilliseconds() const;


	template <typename T>
	inline
	auto GetCurrentEpochTime() const;
};

inline milliseconds FrameworkTimer::GetCurrentTimeInMilliseconds() const
{
    return std::chrono::duration_cast<milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
}

template <typename T>
inline
auto FrameworkTimer::GetCurrentEpochTime() const
{
	return std::chrono::duration_cast<T>(std::chrono::system_clock::now().time_since_epoch()).count();
}


#endif /* defined(__Hazelnut__FrameworkTimer__) */
