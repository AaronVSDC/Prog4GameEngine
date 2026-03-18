#ifndef SUBJECT_H
#define SUBJECT_H
#include <vector>
#include "Observer.h"

namespace dae
{
    class GameObject;

    class Subject
    {
    public:
        virtual ~Subject() = default;

        void AddObserver(Observer* observer)
        {
            m_Observers.push_back(observer);
        }

        void RemoveObserver(Observer* observer)
        {
            m_Observers.erase(
                std::remove(m_Observers.begin(), m_Observers.end(), observer),
                m_Observers.end()
            );
        }

    protected:
        void Notify(GameObject* gameObject, const Event& event)
        {
            for (auto* observer : m_Observers)
                observer->OnNotify(gameObject, event);
        }

    private:
        std::vector<Observer*> m_Observers;
    };
}
#endif