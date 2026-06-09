#ifndef OBSERVER_H
#define OBSERVER_H
#include "Event.h"

namespace UndyneEngine
{
	class GameObject;
	class Subject;

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void onNotify(GameObject& subject, Event event) = 0;
		virtual void onSubjectDestroyed(Subject&) {}
	};
}
#endif
