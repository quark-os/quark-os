#include <stdint.h>

#include "system.h"

extern "C"
{
	
	int main()
	{		
		System::initialize();
		System::run();
		
		return 0;
	}
}
