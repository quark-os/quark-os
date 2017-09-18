#include "math.h"

size_t pow(size_t base, size_t exponent)
{
	if(exponent < 0)
	{
		return 0;
	}
	else if(exponent == 0)
	{
		return 1;
	}
	else if(exponent == 1)
	{
		return base;
	}
	else
	{
		return base * pow(base, exponent - 1);
	}
}
