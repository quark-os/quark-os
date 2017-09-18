#ifndef __ELF_H
#define __ELF_H

class ELF
{
public:

	ELF();
	
	ELF(void* file);
	
	bool load();
	
	void* getEntryPoint();
	
private:

	void* file;
	
	void* entry;
	
};

#endif
