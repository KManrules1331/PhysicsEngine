#pragma once
class Observer
{
public:
	virtual ~Observer();
	virtual void onNotify(Collider* collider, Event e);
};

