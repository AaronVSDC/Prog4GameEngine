#include "Subject.h"
#include "Observer.h"

#include <algorithm>

namespace UndyneEngine
{
	Subject::~Subject()
	{
		for (Observer* observer : m_Observers)
			observer->onSubjectDestroyed(*this);
	}

	void Subject::addObserver(Observer* observer)
	{
		if (observer)
			m_Observers.push_back(observer);
	}

	void Subject::removeObserver(Observer* observer)
	{
		std::erase(m_Observers, observer);
	}

	void Subject::notify(GameObject& subject, Event event)
	{
		for (Observer* observer : m_Observers)
			observer->onNotify(subject, event);
	}
}
