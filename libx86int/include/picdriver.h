#ifndef PICDRIVER_H
#define PICDRIVER_H

class PICDriver
{
public:

	PICDriver();
	
	PICDriver(int offset);
	
	void update();

	int offset;
	
};

#endif
