#include "InputManager.h"
#include "../Log/Log.h"

#include <SDL3/SDL.h>

//std
#include <algorithm>
#include <array>
#include <cmath>
#include <unordered_map>
#include <utility>
#include <vector>

namespace UndyneEngine
{
	namespace
	{
        SDL_Scancode toSDL(KeyboardKey key) noexcept
        {
            switch (key)
            {
            case KeyboardKey::A: return SDL_SCANCODE_A;
            case KeyboardKey::B: return SDL_SCANCODE_B;
            case KeyboardKey::C: return SDL_SCANCODE_C;
            case KeyboardKey::D: return SDL_SCANCODE_D;
            case KeyboardKey::E: return SDL_SCANCODE_E;
            case KeyboardKey::F: return SDL_SCANCODE_F;
            case KeyboardKey::G: return SDL_SCANCODE_G;
            case KeyboardKey::H: return SDL_SCANCODE_H;
            case KeyboardKey::I: return SDL_SCANCODE_I;
            case KeyboardKey::J: return SDL_SCANCODE_J;
            case KeyboardKey::K: return SDL_SCANCODE_K;
            case KeyboardKey::L: return SDL_SCANCODE_L;
            case KeyboardKey::M: return SDL_SCANCODE_M;
            case KeyboardKey::N: return SDL_SCANCODE_N;
            case KeyboardKey::O: return SDL_SCANCODE_O;
            case KeyboardKey::P: return SDL_SCANCODE_P;
            case KeyboardKey::Q: return SDL_SCANCODE_Q;
            case KeyboardKey::R: return SDL_SCANCODE_R;
            case KeyboardKey::S: return SDL_SCANCODE_S;
            case KeyboardKey::T: return SDL_SCANCODE_T;
            case KeyboardKey::U: return SDL_SCANCODE_U;
            case KeyboardKey::V: return SDL_SCANCODE_V;
            case KeyboardKey::W: return SDL_SCANCODE_W;
            case KeyboardKey::X: return SDL_SCANCODE_X;
            case KeyboardKey::Y: return SDL_SCANCODE_Y;
            case KeyboardKey::Z: return SDL_SCANCODE_Z;
            case KeyboardKey::Num0: return SDL_SCANCODE_0;
            case KeyboardKey::Num1: return SDL_SCANCODE_1;
            case KeyboardKey::Num2: return SDL_SCANCODE_2;
            case KeyboardKey::Num3: return SDL_SCANCODE_3;
            case KeyboardKey::Num4: return SDL_SCANCODE_4;
            case KeyboardKey::Num5: return SDL_SCANCODE_5;
            case KeyboardKey::Num6: return SDL_SCANCODE_6;
            case KeyboardKey::Num7: return SDL_SCANCODE_7;
            case KeyboardKey::Num8: return SDL_SCANCODE_8;
            case KeyboardKey::Num9: return SDL_SCANCODE_9;
            case KeyboardKey::Space:      return SDL_SCANCODE_SPACE;
            case KeyboardKey::Enter:      return SDL_SCANCODE_RETURN;
            case KeyboardKey::Escape:     return SDL_SCANCODE_ESCAPE;
            case KeyboardKey::Tab:        return SDL_SCANCODE_TAB;
            case KeyboardKey::LeftShift:  return SDL_SCANCODE_LSHIFT;
            case KeyboardKey::RightShift: return SDL_SCANCODE_RSHIFT;
            case KeyboardKey::LeftCtrl:   return SDL_SCANCODE_LCTRL;
            case KeyboardKey::RightCtrl:  return SDL_SCANCODE_RCTRL;
            case KeyboardKey::LeftAlt:    return SDL_SCANCODE_LALT;
            case KeyboardKey::RightAlt:   return SDL_SCANCODE_RALT;
            case KeyboardKey::Up:    return SDL_SCANCODE_UP;
            case KeyboardKey::Down:  return SDL_SCANCODE_DOWN;
            case KeyboardKey::Left:  return SDL_SCANCODE_LEFT;
            case KeyboardKey::Right: return SDL_SCANCODE_RIGHT;
            case KeyboardKey::F1:  return SDL_SCANCODE_F1;
            case KeyboardKey::F2:  return SDL_SCANCODE_F2;
            case KeyboardKey::F3:  return SDL_SCANCODE_F3;
            case KeyboardKey::F4:  return SDL_SCANCODE_F4;
            case KeyboardKey::F5:  return SDL_SCANCODE_F5;
            case KeyboardKey::F6:  return SDL_SCANCODE_F6;
            case KeyboardKey::F7:  return SDL_SCANCODE_F7;
            case KeyboardKey::F8:  return SDL_SCANCODE_F8;
            case KeyboardKey::F9:  return SDL_SCANCODE_F9;
            case KeyboardKey::F10: return SDL_SCANCODE_F10;
            case KeyboardKey::F11: return SDL_SCANCODE_F11;
            case KeyboardKey::F12: return SDL_SCANCODE_F12;
            }
            return SDL_SCANCODE_UNKNOWN;
        }
        SDL_GamepadButton toSDL(GamepadButton button) noexcept
        {
            switch (button)
            {
            case GamepadButton::A: return SDL_GAMEPAD_BUTTON_SOUTH;
            case GamepadButton::B: return SDL_GAMEPAD_BUTTON_EAST;
            case GamepadButton::X: return SDL_GAMEPAD_BUTTON_WEST;
            case GamepadButton::Y: return SDL_GAMEPAD_BUTTON_NORTH;
            case GamepadButton::DPadUp:    return SDL_GAMEPAD_BUTTON_DPAD_UP;
            case GamepadButton::DPadDown:  return SDL_GAMEPAD_BUTTON_DPAD_DOWN;
            case GamepadButton::DPadLeft:  return SDL_GAMEPAD_BUTTON_DPAD_LEFT;
            case GamepadButton::DPadRight: return SDL_GAMEPAD_BUTTON_DPAD_RIGHT;
            case GamepadButton::LeftBumper:  return SDL_GAMEPAD_BUTTON_LEFT_SHOULDER;
            case GamepadButton::RightBumper: return SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER;
            case GamepadButton::LeftStick:   return SDL_GAMEPAD_BUTTON_LEFT_STICK;
            case GamepadButton::RightStick:  return SDL_GAMEPAD_BUTTON_RIGHT_STICK;
            case GamepadButton::Start: return SDL_GAMEPAD_BUTTON_START;
            case GamepadButton::Back:  return SDL_GAMEPAD_BUTTON_BACK;
            case GamepadButton::Guide: return SDL_GAMEPAD_BUTTON_GUIDE;
            }
            return SDL_GAMEPAD_BUTTON_INVALID;
        }

        std::pair<SDL_GamepadAxis, SDL_GamepadAxis> toSDL(GamepadStick stick) noexcept
        {
            switch (stick)
            {
            case GamepadStick::Left:  return { SDL_GAMEPAD_AXIS_LEFTX,  SDL_GAMEPAD_AXIS_LEFTY  };
            case GamepadStick::Right: return { SDL_GAMEPAD_AXIS_RIGHTX, SDL_GAMEPAD_AXIS_RIGHTY };
            }
            return { SDL_GAMEPAD_AXIS_INVALID, SDL_GAMEPAD_AXIS_INVALID };
        }

        SDL_GamepadAxis toSDL(GamepadTrigger trigger) noexcept
        {
            switch (trigger)
            {
            case GamepadTrigger::Left:  return SDL_GAMEPAD_AXIS_LEFT_TRIGGER;
            case GamepadTrigger::Right: return SDL_GAMEPAD_AXIS_RIGHT_TRIGGER;
            }
            return SDL_GAMEPAD_AXIS_INVALID;
        }

        float normalizeStickAxis(Sint16 raw) noexcept
        {
            return (raw >= 0) ? raw / 32767.0f : raw / 32768.0f;
        }

        struct KeyboardButtonBinding
        {
            BindingID    id;
            InputState   state;
            KeyboardKey  key;
            std::unique_ptr<Command> command;
        };

        struct GamepadButtonBinding
        {
            BindingID     id;
            ControllerID  controllerID;
            InputState    state;
            GamepadButton button;
            std::unique_ptr<Command> command;
        };

        struct StickBinding
        {
            BindingID id;
            ControllerID controllerID;
            GamepadStick stick;
            float deadzone;
            std::unique_ptr<StickCommand> command;
        };

        struct TriggerBinding
        {
            BindingID id;
            ControllerID controllerID;
            GamepadTrigger trigger;
            float deadzone;
            std::unique_ptr<TriggerCommand> command;
        };

        struct ControllerData
        {
            ControllerID id;
            SDL_Gamepad* gamepad = nullptr;
        };

        std::vector<ControllerData>          s_Controllers;
        std::vector<KeyboardButtonBinding>   s_KeyboardButtonBindings;
        std::vector<GamepadButtonBinding>    s_GamepadButtonBindings;
        std::vector<StickBinding>            s_StickBindings;
        std::vector<TriggerBinding>          s_TriggerBindings;
        std::uint32_t                        s_NextControllerID = 1;
        std::uint64_t                        s_NextBindingID = 1;

        std::vector<bool>           s_PreviousKeys;
        std::vector<bool>           s_CurrentKeys;

        std::unordered_map<SDL_JoystickID, std::array<bool, SDL_GAMEPAD_BUTTON_COUNT>> s_PreviousGamepadButtons;

        ControllerData* findController(ControllerID id)
        {
            auto it = std::ranges::find_if(s_Controllers, [&](const ControllerData& controller) {return controller.id == id;  });
            return it == s_Controllers.end() ? nullptr : &*it;
        }

        void assignNewGamepad(SDL_JoystickID which)
        {
            for (auto& controller : s_Controllers)
            {
                if (controller.gamepad == nullptr)
                {
                    controller.gamepad = SDL_OpenGamepad(which);
                    if (controller.gamepad)
                    {
                        s_PreviousGamepadButtons[which].fill(false);
                        UDE_CORE_INFO("Gamepad attached to controller {}", controller.id.value);
                    }
                    return;
                }
            }
        }

        void releaseGamepad(SDL_JoystickID which)
        {
            for (auto& controller : s_Controllers)
            {
                if (controller.gamepad and SDL_GetGamepadID(controller.gamepad) == which)
                {
                    SDL_CloseGamepad(controller.gamepad);
                    controller.gamepad = nullptr;
                    s_PreviousGamepadButtons.erase(which);
                    UDE_CORE_INFO("Gamepad detached from controller {}", controller.id.value);
                    return;
                }
            }
        }

        bool evaluateEdge(bool now, bool previous, InputState state)
        {
            switch (state)
            {
                case InputState::Down: return now;
                case InputState::Pressed: return now and !previous;
                case InputState::Released: return !now and previous;
            }
            return false;
        }
	}




namespace InputManager
{
    void init()
    {
        SDL_InitSubSystem(SDL_INIT_GAMEPAD);
    }
    void destroy()
    {
        for (auto& controller : s_Controllers)
            if (controller.gamepad) SDL_CloseGamepad(controller.gamepad);

        s_KeyboardButtonBindings.clear();
        s_GamepadButtonBindings.clear();
        s_StickBindings.clear();
        s_TriggerBindings.clear();
        s_Controllers.clear();
        s_PreviousGamepadButtons.clear();
        s_PreviousKeys.clear();
        s_CurrentKeys.clear();

        SDL_QuitSubSystem(SDL_INIT_GAMEPAD);
    }
    bool processInput()
    {
        SDL_Event sdlEvent;

        while (SDL_PollEvent(&sdlEvent))
        {
            if (sdlEvent.type == SDL_EVENT_QUIT) return false;

            if (sdlEvent.type == SDL_EVENT_GAMEPAD_ADDED)
                assignNewGamepad(sdlEvent.gdevice.which);
            else if (sdlEvent.type == SDL_EVENT_GAMEPAD_REMOVED)
                releaseGamepad(sdlEvent.gdevice.which);
        }

        int numberKeys = 0;
        const bool* sdlKeys = SDL_GetKeyboardState(&numberKeys);
        if (static_cast<int>(s_CurrentKeys.size()) != numberKeys)
        {
            s_CurrentKeys.assign(numberKeys, false);
            s_PreviousKeys.assign(numberKeys, false);
        }

        for (int i = 0; i < numberKeys; ++i)
            s_CurrentKeys[i] = sdlKeys[i];

        //now evaluate all bindings
        for (auto& binding : s_KeyboardButtonBindings)
        {
            const SDL_Scancode scancode = toSDL(binding.key);
            if (scancode == SDL_SCANCODE_UNKNOWN) continue;

            const bool now      = s_CurrentKeys[scancode];
            const bool previous = s_PreviousKeys[scancode];

            if (evaluateEdge(now, previous, binding.state) and binding.command)
                binding.command->execute();
        }

        for (auto& binding : s_GamepadButtonBindings)
        {
            ControllerData* controller = findController(binding.controllerID);
            if (!controller or !controller->gamepad) continue;

            const SDL_GamepadButton sdlButton = toSDL(binding.button);
            if (sdlButton == SDL_GAMEPAD_BUTTON_INVALID) continue;

            const SDL_JoystickID joystickID = SDL_GetGamepadID(controller->gamepad);

            const bool now      = SDL_GetGamepadButton(controller->gamepad, sdlButton);
            const bool previous = s_PreviousGamepadButtons[joystickID][sdlButton];

            if (evaluateEdge(now, previous, binding.state) and binding.command)
                binding.command->execute();
        }

        for (auto& stickBinding : s_StickBindings)
        {
            ControllerData* controller = findController(stickBinding.controllerID);
            if (!controller or !controller->gamepad) continue;

            const auto [sdlAxisX, sdlAxisY] = toSDL(stickBinding.stick);
            if (sdlAxisX == SDL_GAMEPAD_AXIS_INVALID) continue;

            const float stickX = normalizeStickAxis(SDL_GetGamepadAxis(controller->gamepad, sdlAxisX));
            const float stickY = normalizeStickAxis(SDL_GetGamepadAxis(controller->gamepad, sdlAxisY));

            const float magnitude = std::sqrt(stickX * stickX + stickY * stickY);

            if (magnitude < stickBinding.deadzone)
            {
                if (stickBinding.command) stickBinding.command->execute(glm::vec2{ 0.0f });
                continue;
            }

            const float clampedMagnitude = std::min(magnitude, 1.0f);
            const float scaledMagnitude  = (clampedMagnitude - stickBinding.deadzone)
                                         / (1.0f - stickBinding.deadzone);

            const glm::vec2 direction{ stickX / magnitude, stickY / magnitude };

            if (stickBinding.command) stickBinding.command->execute(direction * scaledMagnitude);
        }

        for (auto& triggerBinding : s_TriggerBindings)
        {
            ControllerData* controller = findController(triggerBinding.controllerID);
            if (!controller or !controller->gamepad) continue;

            const SDL_GamepadAxis sdlAxis = toSDL(triggerBinding.trigger);
            if (sdlAxis == SDL_GAMEPAD_AXIS_INVALID) continue;

            const Sint16 raw = SDL_GetGamepadAxis(controller->gamepad, sdlAxis);
            const float triggerValue = static_cast<float>(raw) / 32767.0f; 

            if (triggerValue < triggerBinding.deadzone) continue;

            const float scaledValue = (triggerValue - triggerBinding.deadzone)
                                    / (1.0f - triggerBinding.deadzone);

            if (triggerBinding.command) triggerBinding.command->execute(scaledValue);
        }


        //save previous keys for next frame
        s_PreviousKeys = s_CurrentKeys;
        for (auto& controller : s_Controllers)
        {
            if (!controller.gamepad) continue;
            const SDL_JoystickID joystickID = SDL_GetGamepadID(controller.gamepad);
            auto& previous = s_PreviousGamepadButtons[joystickID];
            for (int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; ++i)
                previous[i] = SDL_GetGamepadButton(controller.gamepad, static_cast<SDL_GamepadButton>(i));
        }

        return true;
    }

    ControllerID addController()
    {
        ControllerID newControllerID{ static_cast<std::uint8_t>(s_NextControllerID++) };
        s_Controllers.push_back({ newControllerID, nullptr });

        int count = 0;
        SDL_JoystickID* joystickIDs = SDL_GetGamepads(&count);

        if (joystickIDs)
        {
            for (int i = 0; i < count; ++i)
            {
                bool alreadyBound = false;
                for (auto& controller : s_Controllers)
                {
                    if (controller.gamepad and SDL_GetGamepadID(controller.gamepad) == joystickIDs[i])
                    {
                        alreadyBound = true;
                        break;
                    }
                }

                if (!alreadyBound)
                {
                    assignNewGamepad(joystickIDs[i]);
                    break;
                }
            }
            SDL_free(joystickIDs);
        }

        return newControllerID;
    }


    void removeController(ControllerID id)
    {
        clearBindings(id);
        std::erase_if(s_Controllers, [&](ControllerData& controller)
        {
            if (controller.id == id)
            {
                if (controller.gamepad) SDL_CloseGamepad(controller.gamepad);
                return true;
            }
            return false;
        });
    }

    BindingID bindButtonCommand(KeyboardKey key, InputState state, std::unique_ptr<Command> command)
    {
        BindingID newBindingID{ s_NextBindingID++ };
        s_KeyboardButtonBindings.push_back(KeyboardButtonBinding{ newBindingID, state, key, std::move(command) });
        return newBindingID;
    }


    BindingID bindButtonCommand(ControllerID id, GamepadButton button, InputState state, std::unique_ptr<Command> command)
    {
        BindingID newBindingID{ s_NextBindingID++ };
        s_GamepadButtonBindings.push_back(GamepadButtonBinding{ newBindingID, id, state, button, std::move(command) });
        return newBindingID;
    }

    BindingID bindStickCommand(ControllerID id, GamepadStick stick, std::unique_ptr<StickCommand> command, float deadzone)
    {
        BindingID newBindingID{ s_NextBindingID++ };
        s_StickBindings.push_back(StickBinding{ newBindingID, id, stick, deadzone, std::move(command) });
        return newBindingID;
    }

    BindingID bindTriggerCommand(ControllerID id, GamepadTrigger trigger, std::unique_ptr<TriggerCommand> command, float deadzone)
    {
        BindingID newBindingID{ s_NextBindingID++ };
        s_TriggerBindings.push_back(TriggerBinding{ newBindingID, id, trigger, deadzone, std::move(command) });
        return newBindingID;
    }

    void unbindCommand(BindingID id)
    {
        auto matchesID = [&](const auto& binding) { return binding.id == id; };
        std::erase_if(s_KeyboardButtonBindings, matchesID);
        std::erase_if(s_GamepadButtonBindings,  matchesID);
        std::erase_if(s_StickBindings,          matchesID);
        std::erase_if(s_TriggerBindings,        matchesID);
    }

    void clearBindings(ControllerID id)
    {
        auto matchesController = [&](const auto& binding) { return binding.controllerID == id; };
        std::erase_if(s_GamepadButtonBindings, matchesController);
        std::erase_if(s_StickBindings,         matchesController);
        std::erase_if(s_TriggerBindings,       matchesController);
    }

}
}
