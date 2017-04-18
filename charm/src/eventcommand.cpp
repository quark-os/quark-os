#include "eventcommand.h"

EventCommand::EventCommand()
	: Event()
{
	id = EVENT_COMMAND;
	command[0] = '\0';
}

EventCommand::EventCommand(char* command)
{
	id = EVENT_COMMAND;
	for(int i = 0; command[i] != '\n'; i++)
	{
		this->command[i] = command[i];
	}
}

char* EventCommand::getCommand()
{
	return command;
}
