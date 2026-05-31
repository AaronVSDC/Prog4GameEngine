#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include "../Core/Core.h"
#include "Command.h"


//std
#include <memory>
#include <utility>

namespace UndyneEngine
{

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

	enum class GamepadTrigger : std::uint8_t
	{
		Left,
		Right
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

		// "Controller" here means a gamepad slot. The keyboard is global and doesn't need one.
		UNDYNE_API ControllerID addController();
		UNDYNE_API void			removeController(ControllerID id);

		UNDYNE_API BindingID bindButtonCommand (                  KeyboardKey   key,     InputState state, std::unique_ptr<Command> command);
		UNDYNE_API BindingID bindButtonCommand (ControllerID id,  GamepadButton button,  InputState state, std::unique_ptr<Command> command);
		UNDYNE_API BindingID bindStickCommand  (ControllerID id,  GamepadStick   stick,   std::unique_ptr<StickCommand>   command, float deadzone = 0.15f);
		UNDYNE_API BindingID bindTriggerCommand(ControllerID id,  GamepadTrigger trigger, std::unique_ptr<TriggerCommand> command, float deadzone = 0.05f);

		template<std::derived_from<Command> T, typename... Args>
		BindingID bindButtonCommand(KeyboardKey key, InputState state, Args&&... args)
		{
			return bindButtonCommand(key, state, std::make_unique<T>(std::forward<Args>(args)...));
		}

		template<std::derived_from<Command> T, typename... Args>
		BindingID bindButtonCommand(ControllerID id, GamepadButton button, InputState state, Args&&... args)
		{
			return bindButtonCommand(id, button, state, std::make_unique<T>(std::forward<Args>(args)...));
		}

		template<std::derived_from<StickCommand> T, typename... Args>
		BindingID bindStickCommand(ControllerID id, GamepadStick stick, Args&&... args)
		{
			return bindStickCommand(id, stick, std::make_unique<T>(std::forward<Args>(args)...));
		}

		template<std::derived_from<TriggerCommand> T, typename... Args>
		BindingID bindTriggerCommand(ControllerID id, GamepadTrigger trigger, Args&&... args)
		{
			return bindTriggerCommand(id, trigger, std::make_unique<T>(std::forward<Args>(args)...));
		}


		UNDYNE_API void unbindCommand(BindingID binding);
		UNDYNE_API void clearBindings(ControllerID id);

	};
}




#endif
