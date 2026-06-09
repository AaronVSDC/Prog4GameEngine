#ifndef OBSERVER_H
#define OBSERVER_H
#include "../Core/Core.h"
#include "Event.h"

namespace UndyneEngine
{
	class GameObject;
	class Subject;

	class UNDYNE_API Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void onNotify(GameObject& subject, Event event) = 0;
		virtual void onSubjectDestroyed(Subject&) {}
	};
}
#endif
