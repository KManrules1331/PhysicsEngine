#include "Subject.h"

void Subject::addObserver(Observer* o)
{
	ObserverNode* newNode = new ObserverNode;
	newNode->observer = o;
		   
	newNode->next = head;
	head = newNode;
}

void Subject::removeObserver(Observer* o)
{
	for (ObserverNode* current = head, *previous = head;	//Init
		current != nullptr;									//Condition
		previous = current, current = current->next)		//Iteration
	{
		if (current->observer == o)
		{
			if (current == head)
			{
				head = current->next;
			}
			else
			{
				previous->next = current->next;
			}
			current->next = nullptr;
			current->observer = nullptr;
			delete current;
		}
	}
}

void Subject::notify(Collider* collider, Event e)
{
	for (ObserverNode* current = head; current != nullptr; current = current->next)
	{
		current->observer->onNotify(collider, e);
	}
}