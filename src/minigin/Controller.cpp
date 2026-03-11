#include "Controller.h"
#include <windows.h>
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")

namespace dae
{
    class Controller::ControllerImpl
    {
    public:
        explicit ControllerImpl(uint8_t index)
            : m_Index(index)
        {
            ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
            ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
        }

        void update()
        {
            m_PreviousState = m_CurrentState;
            ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
            XInputGetState(m_Index, &m_CurrentState);
        }

        bool isDown(ControllerButton button) const
        {
            return (m_CurrentState.Gamepad.wButtons & static_cast<unsigned short>(button)) != 0;
        }

        bool isPressed(ControllerButton button) const
        {
            return ((m_CurrentState.Gamepad.wButtons & static_cast<unsigned short>(button)) != 0) &&
                ((m_PreviousState.Gamepad.wButtons & static_cast<unsigned short>(button)) == 0);
        }

        bool isUp(ControllerButton button) const
        {
            return ((m_CurrentState.Gamepad.wButtons & static_cast<unsigned short>(button)) == 0) &&
                ((m_PreviousState.Gamepad.wButtons & static_cast<unsigned short>(button)) != 0);
        }
        glm::vec2 getLeftThumb() const
        {
            constexpr float maxThumb{ 32767.f };
            glm::vec2 result{};
            const float deadZone = static_cast<float>(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
            if (std::abs(m_CurrentState.Gamepad.sThumbLX) > deadZone || std::abs(m_CurrentState.Gamepad.sThumbLY) > deadZone)
            {
                result.x = m_CurrentState.Gamepad.sThumbLX / maxThumb;
                result.y = -m_CurrentState.Gamepad.sThumbLY / maxThumb;
            }
            return result;
        }

        glm::vec2 getRightThumb() const
        {
            constexpr float maxThumb{ 32767.f };
            glm::vec2 result{};
            const float deadZone = static_cast<float>(XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
            if (std::abs(m_CurrentState.Gamepad.sThumbRX) > deadZone || std::abs(m_CurrentState.Gamepad.sThumbRY) > deadZone)
            {
                result.x = m_CurrentState.Gamepad.sThumbRX / maxThumb;
                result.y = m_CurrentState.Gamepad.sThumbRY / maxThumb;
            }
            return result;
        }

    private:
        uint8_t m_Index{};
        XINPUT_STATE m_CurrentState{};
        XINPUT_STATE m_PreviousState{};
    };

    Controller::Controller(uint8_t controllerIndex)
        : m_pImpl(std::make_unique<ControllerImpl>(controllerIndex))
    {
    }

    Controller::~Controller() = default;

    bool Controller::isDown(ControllerButton button) const
    {
        return m_pImpl->isDown(button);
    }

    bool Controller::isPressed(ControllerButton button) const
    {
        return m_pImpl->isPressed(button);
    }

    bool Controller::isUp(ControllerButton button) const
    {
        return m_pImpl->isUp(button);
    }

    void Controller::update()
    {
        m_pImpl->update();
    }
    glm::vec2 Controller::getLeftThumb() const
    {
        return m_pImpl->getLeftThumb();
    }

    glm::vec2 Controller::getRightThumb() const
    {
        return m_pImpl->getRightThumb();
    }
}