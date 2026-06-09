#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
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

	struct ControllerID
	{
		std::uint8_t value = 0;
		bool operator==(const ControllerID&) const = default;
	};

	struct BindingID
	{
		std::uint64_t value = 0;
		bool operator==(const BindingID&) const = default;
	};

	namespace InputManager
	{
		void init();
		void destroy();

		bool processInput();

		ControllerID addController();
		void			removeController(ControllerID id);

		BindingID bindButtonCommand (                  KeyboardKey   key,     InputState state, std::unique_ptr<Command> command);
		BindingID bindButtonCommand (ControllerID id,  GamepadButton button,  InputState state, std::unique_ptr<Command> command);
		BindingID bindStickCommand  (ControllerID id,  GamepadStick   stick,   std::unique_ptr<StickCommand>   command, float deadzone = 0.15f);
		BindingID bindTriggerCommand(ControllerID id,  GamepadTrigger trigger, std::unique_ptr<TriggerCommand> command, float deadzone = 0.05f);


		void unbindCommand(BindingID binding);
		void clearBindings(ControllerID id);

	};
}




#endif
