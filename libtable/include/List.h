#ifndef __LIST_H
#define __LIST_H

#include "Iterator.h"
#include "Heap.h"

template <class T>
struct ListEntry
{
	T data;
	ListEntry* prev;
	ListEntry* next;
};

template <class T>
class List
{
public:

	const Iterator end = 0;
	
	List()
	{
		listSize = 0;
		first = (ListEntry<T>*) 0;
		last = (ListEntry<T>*) 0;
	}
	
	Iterator insert(Iterator position, T data)
	{
		ListEntry<T>* i = heap.allocate(sizeof(ListEntry<T>));
		if(i == (ListEntry<T>*) 0)
		{
			return end;
		}
		
		i->next = reinterpret_cast<ListEntry<T>*>(position);
		if(listSize == 0)
		{
			i->prev = end;
			i->next = end;
			first = i;
			last = i;
		}
		else if(listSize == 1)
		{
			if(position == end)
			{
				i->prev = first;
				first->next = i;
				last = i;
			}
			else
			{
				i->prev = end;
				first->prev = i;
				first = i;
			}
		}
		else
		{
			if(position != end && position != getFirst())
			{
				i->prev = reinterpret_cast<ListEntry<T>*>(position)->prev;
				reinterpret_cast<ListEntry<T>*>(position)->prev->next = i;
				reinterpret_cast<ListEntry<T>*>(position)->prev = i;
			}
			else if(position == end)
			{
				i->prev = last;
				last->next = i;
				last = i; 
			}
			else
			{
				i->prev = reinterpret_cast<ListEntry<T>*>(end);
				reinterpret_cast<ListEntry<T>*>(position)->prev = i;
				first = i;
			}
		}
		i->data = data;
		listSize++;
		return reinterpret_cast<Iterator>(i);
	}
	
	Iterator insertAtBeginning(T data)
	{
		return insert(getFirst(), data);
	}
	
	Iterator insertAtEnd(T data)
	{
		return insert(end, data);
	}
	
	void remove(Iterator i)
	{
		ListEntry<T>* entry = reinterpret_cast<ListEntry<T>*>(i);
		if(entry->prev != 0)
		{
			entry->prev->next = entry->next;
		}
		if(entry->next != 0)
		{
			entry->next->prev = entry->prev;
		}
		if(i == getFirst())
		{
			first = entry->next;
		}
		if(i == getLast())
		{
			last = entry->prev;
		}
		heap.free((void*) entry);
	}

	Iterator getFirst()
	{
		return reinterpret_cast<Iterator>(first);
	}
	
	Iterator getLast()
	{
		return reinterpret_cast<Iterator>(last);
	}
	
	Iterator next(Iterator i)
	{
		return reinterpret_cast<Iterator>(reinterpret_cast<ListEntry<T>*>(i)->next);
	}
	
	T& get(Iterator i)
	{
		ListEntry<T>* entry = reinterpret_cast<ListEntry<T>*>(i);
		return entry->data;
	}
	
	size_t getSize()
	{
		return listSize;
	}
	
private:

	size_t listSize;

	ListEntry<T>* first;
	
	ListEntry<T>* last;
	
};

#endif
