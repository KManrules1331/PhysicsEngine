#pragma once

#include "GameObject.h"
#include "Events.h"
#include "Observer.h"

class Subject
{
public:
	void addObserver(Observer* observer);
	void removeObserver(Observer* observer);
	void notify(Collider* collider, Event e);
protected:
private:
	struct ObserverNode
	{
		ObserverNode* next;
		Observer* observer;
	};
	ObserverNode* head;

};

