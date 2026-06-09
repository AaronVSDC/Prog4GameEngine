#ifndef SUBJECT_H
#define SUBJECT_H
#include "../Core/Core.h"
#include "Event.h"

#include <vector>

namespace UndyneEngine
{
	class GameObject;
	class Observer;

	class UNDYNE_API Subject
	{
	public:
		Subject() = default;
		~Subject();
		Subject(const Subject&) = delete;
		Subject(Subject&&) = delete;
		Subject& operator=(const Subject&) = delete;
		Subject& operator=(Subject&&) = delete;

		void addObserver(Observer* observer);
		void removeObserver(Observer* observer);

	protected:
		void notify(GameObject& subject, Event event);

	private:
		std::vector<Observer*> m_Observers;
	};
}
#endif
