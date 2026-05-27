#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include "..\Core\Core.h"
#include "Command.h"


//std
#include <memory>
#include <utility>

namespace UndyneEngine
{

	enum class ControllerType : std::uint8_t
	{
		Keyboard,
		Gamepad
	};

	enum class InputState : std::uint8_t
	{
		Pressed, 
		Released, 
		Down
	};

	enum class KeyboardKey : std::uint8_t
	{
		A, B, C, D, E, F, G, H, I, J, K, L, M,
		N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
		Space, Enter, Escape, Tab,
		LeftShift, RightShift, LeftCtrl, RightCtrl, LeftAlt, RightAlt,
		Up, Down, Left, Right,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12
	};

	enum class GamepadButton : std::uint8_t
	{
		A, B, X, Y,
		DPadUp, DPadDown, DPadLeft, DPadRight,
		LeftBumper, RightBumper,
		LeftStick, RightStick,
		Start, Back, Guide
	};

	enum class GamepadStick : std::uint8_t
	{ 
		Left, 
		Right 
	};
	enum class GamepadAxis : std::uint8_t
	{
		LeftStickX, LeftStickY,
		RightStickX, RightStickY, 
		LeftTrigger, RightTrigger
	}; 

	struct UNDYNE_API ControllerID
	{
		std::uint8_t value = 0; 
		bool operator==(const ControllerID&) const = default; 
	};

	struct UNDYNE_API BindingID
	{
		std::uint64_t value = 0; 
		bool operator==(const BindingID&) const = default; 
	};

	namespace InputManager
	{
		UNDYNE_API void init(); 
		UNDYNE_API void destroy(); 

		UNDYNE_API bool processInput();

		UNDYNE_API ControllerID addController(ControllerType type); 
		UNDYNE_API void			removeController(ControllerID id); 

		UNDYNE_API BindingID bindCommand(ControllerID id, KeyboardKey key, InputState state, std::unique_ptr<Command> command); 
		UNDYNE_API BindingID bindCommand(ControllerID id, GamepadButton button, InputState state, std::unique_ptr<Command> command); 
		UNDYNE_API BindingID bindAxis(ControllerID id, GamepadAxis axis, std::unique_ptr<Command> command, float deadzone = 0.15f); 

		template<std::derived_from<Command> T, typename... Args>
		BindingID bindCommand(ControllerID id, KeyboardKey key, InputState state, Args&&... args)
		{
			return bindCommand(id, key, state, std::make_unique<T>(std::forward<Args>(args)...)); 
		}

		template<std::derived_from<Command> T, typename... Args>
		BindingID bindCommand(ControllerID id, GamepadButton button, InputState state, Args&&... args)
		{
			return bindCommand(id, button, state, std::make_unique<T>(std::forward<Args>(args)...));
		}

		template<std::derived_from<Command> T, typename... Args>
		BindingID bindAxis(ControllerID id, GamepadAxis axis, Args&&... args)
		{
			return bindAxis(id, axis, std::make_unique<T>(std::forward<Args>(args)...));
		}


		UNDYNE_API void unbindCommand(BindingID binding); 
		UNDYNE_API void clearBindings(ControllerID id); 

	};
}




#endif