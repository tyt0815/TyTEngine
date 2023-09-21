#pragma once

#define SINGLETON(type) static type* GetInstance()\
	{\
		static type mgr;\
		return &mgr;\
	}