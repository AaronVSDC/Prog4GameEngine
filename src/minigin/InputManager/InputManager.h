#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include <memory>
#include <vector>
#include <array> 
#include <SDL3/SDL.h>
#include "Singleton.h"
#include "Controller.h" 
#include "Command.h"

namespace dae
{
    class InputManager final : public Singleton<InputManager>
    {
    public:
        bool processInput();

        void addController(uint8_t index);
        void addControllerCommand(uint8_t controllerIdx, ControllerButton button, KeyState state, std::unique_ptr<Command> command);
        void addControllerStickCommand(uint8_t controllerIdx, Stick stick, float deadzone, bool normalize, std::unique_ptr<AnalogCommand> command);
        void addKeyboardCommand(SDL_Scancode key, KeyState state, std::unique_ptr<Command> command);

    private:
        struct ControllerBinding
        {
            uint8_t controllerIdx{};
            ControllerButton button{};
            KeyState state{};
            std::unique_ptr<Command> command{};
        };

        struct KeyboardBinding
        {
            SDL_Scancode key{};
            KeyState state{};
            std::unique_ptr<Command> command{};
        };
        struct StickBinding
        {
            uint8_t controllerIdx{};
            Stick stick{};
            float deadzone{ 0.2f };
            bool normalize{ true };
            std::unique_ptr<AnalogCommand> command{};
        };

        std::vector<ControllerBinding> m_ControllerCommands{};
        std::vector<StickBinding> m_StickCommands{};
        std::vector<KeyboardBinding> m_KeyboardCommands{};
        std::vector<std::unique_ptr<Controller>> m_Controllers{};
    };
}
#endif