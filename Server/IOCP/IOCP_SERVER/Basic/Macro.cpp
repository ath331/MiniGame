#include "Macro.h"

bool IsNullPtr(void* ptr)
{
	if (ptr == nullptr)
		return true;
	else
		return false;
}

void DeleteSafePtr(void* ptr)
{
	if (!IsNullPtr(ptr))
		delete ptr;
}