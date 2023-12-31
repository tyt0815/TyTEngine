#include "pch.h"
#include "GameTimer.h"
#include <Windows.h>

TGameTimer::TGameTimer():
	mSecondsPerCount(0.f),
	mDeltaTime(-1.f),
	mBaseTime(0),
	mPausedTime(0),
	mPrevTime(0),
	mCurrTime(0),
	mStopTime(0),
	mStopped(false)
{
	__int64 CountsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&CountsPerSec);	// 현재 시간 value를 반환
	mSecondsPerCount = 1.0 / (double)CountsPerSec;
}

float TGameTimer::TotalTime() const
{
	if (mStopped)
	{
		return (float)(((mStopTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
	}
	else
	{
		return (float)(((mCurrTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
	}
}

float TGameTimer::DeltaTime() const
{
	return (float)mDeltaTime;
}

void TGameTimer::Reset()
{
	__int64 CurrTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&CurrTime);
	mBaseTime = CurrTime;
	mPrevTime = CurrTime;
	mStopTime = 0;
	mStopped = false;
}

void TGameTimer::Start()
{
	__int64 StartTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&StartTime);

	if (mStopped)
	{
		mPausedTime += (StartTime - mStopTime);
		mPrevTime = StartTime;
		mStopTime = 0;
		mStopped = false;
	}
}

void TGameTimer::Stop()
{
	if (!mStopped)
	{
		__int64 CurrTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&CurrTime);
		mStopTime = CurrTime;
		mStopped = true;
	}
}

void TGameTimer::Tick()
{
	if (mStopped)
	{
		mDeltaTime = 0.f;
		return;
	}
	__int64 CurrTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&CurrTime);
	mCurrTime = CurrTime;

	mDeltaTime = (mCurrTime - mPrevTime) * mSecondsPerCount;

	mPrevTime = mCurrTime;

	if (mDeltaTime < 0.f)
	{
		mDeltaTime = 0.f;
	}
}
