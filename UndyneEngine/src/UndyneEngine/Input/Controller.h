//#ifndef CONTROLLER_H 
//#define CONTROLLER_H
//
//#include "InputManager.h" //todo: just put the enums in this file instead of the inputmanager
//
//#include <SDL3/SDL_joystick.h>
//#include <glm/vec2.hpp>
//
//
////std
//#include <memory>
//
//namespace UndyneEngine
//{
//	class Controller final
//	{
//	public: 
//        Controller();
//        ~Controller();
//
//        Controller(const Controller&) = delete;
//        Controller(Controller&&) = delete;
//        Controller& operator=(const Controller&) = delete;
//        Controller& operator=(Controller&&) = delete;
//
//        void update(); 
//
//        bool isDown(GamepadButton button) const; 
//        bool isPressed(GamepadButton button) const; 
//        bool isReleased(GamepadButton button) const; 
//
//
//    private: 
//        class Impl; 
//        std::unique_ptr<Impl> m_pImpl; 
//
//    };
//}
//
//
//
//
//#endif