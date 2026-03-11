#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <memory>
#include <glm/vec2.hpp>

namespace dae
{
    enum class ControllerButton : unsigned short
    {
        DPadUp = 0x0001,
        DPadDown = 0x0002,
        DPadLeft = 0x0004,
        DPadRight = 0x0008,
        Start = 0x0010,
        Back = 0x0020,
        LeftThumb = 0x0040,
        RightThumb = 0x0080,
        LeftShoulder = 0x0100,
        RightShoulder = 0x0200,
        A = 0x1000,
        B = 0x2000,
        X = 0x4000,
        Y = 0x8000
    };

    enum class KeyState
    {
        Up,
        Down,
        Pressed
    };
    enum class Stick
    {
        Left,
        Right
    };
    class Controller
    {
    public:
        explicit Controller(uint8_t controllerIndex);
        ~Controller();

        Controller(const Controller&) = delete;
        Controller& operator=(const Controller&) = delete;
        Controller(Controller&&) = delete;
        Controller& operator=(Controller&&) = delete;

        bool isDown(ControllerButton button) const;
        bool isPressed(ControllerButton button) const;
        bool isUp(ControllerButton button) const;
        void update();

        glm::vec2 getLeftThumb() const;
        glm::vec2 getRightThumb() const;
    private:
        class ControllerImpl;
        std::unique_ptr<ControllerImpl> m_pImpl; 
    };
}

#endif