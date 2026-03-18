#ifndef ADD_POINTS_COMMAND_H
#define ADD_POINTS_COMMAND_H

#include "Command.h"
#include "ScoreComponent.h"
#include "Observer.h"

namespace dae
{
    class AddPointsCommand final : public Command 
    {
    public:
        AddPointsCommand(ScoreComponent* score, int amount, Event cause)
            : m_pScore(score)
            , m_Amount(amount)
            , m_Cause(cause)
        {
        }

        void execute() override
        {
            if (m_pScore)
                m_pScore->AddPoints(m_Amount, m_Cause);
        }

    private:
        ScoreComponent* m_pScore;
        int             m_Amount;
        Event           m_Cause;
    };
}
#endif