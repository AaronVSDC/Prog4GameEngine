#ifndef SCORE_COMPONENT_H
#define SCORE_COMPONENT_H

#include "BaseComponent.h"
#include "Subject.h"

namespace dae
{
    class ScoreComponent final : public BaseComponent, public Subject
    {
    public:
        ScoreComponent() = default;
        ~ScoreComponent() override = default;

        ScoreComponent(const ScoreComponent&) = delete;
        ScoreComponent(ScoreComponent&&) = delete;
        ScoreComponent& operator=(const ScoreComponent&) = delete;
        ScoreComponent& operator=(ScoreComponent&&) = delete;

        void Start()                 override {}
        void Update(float)           override {}
        void Render()          const override {}

        void AddPoints(int amount, Event cause);
        int  GetScore() const { return m_Score; }

    private:
        int m_Score{ 0 };
    };
}
#endif