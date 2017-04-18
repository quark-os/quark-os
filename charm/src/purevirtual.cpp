#include "system.h"

extern "C"
{
	void __cxa_pure_virtual()
	{
		System::panic();
	}
}
