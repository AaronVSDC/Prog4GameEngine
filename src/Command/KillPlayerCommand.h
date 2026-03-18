#ifndef KILL_PLAYER_COMMAND_H
#define KILL_PLAYER_COMMAND_H

#include "Command.h" 
#include "HealthComponent.h" 

namespace dae
{
    class KillPlayerCommand final : public Command
    {
    public:
        explicit KillPlayerCommand(HealthComponent* health)
            : m_pHealth(health) {
        }

        void execute() override 
        {
            if (m_pHealth)
                m_pHealth->Die();
        }

    private:
        HealthComponent* m_pHealth;
    };
}
#endif