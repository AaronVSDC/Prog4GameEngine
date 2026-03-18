#ifndef OBSERVER_H
#define OBSERVER_H

namespace dae
{
    class GameObject;

    enum class Event
    {
        PLAYER_DIED,
        PLAYER_HIT,
        ENEMY_DIED,
        PICKUP_COLLECTED, 
    };

    class Observer
    {
    public:
        virtual ~Observer() = default;
        virtual void OnNotify(GameObject* gameObject, const Event& event) = 0;
    };
}
#endif