#ifndef STATE_H
#define STATE_H
#include <memory>

namespace UndyneEngine
{
	class GameObject;

	class State
	{
	public:
		virtual ~State() = default;
		State(const State&) = delete;
		State(State&&) = delete;
		State& operator=(const State&) = delete;
		State& operator=(State&&) = delete;

		virtual void onEnter(GameObject&) {}
		virtual void onExit(GameObject&) {}
		virtual std::unique_ptr<State> update(GameObject&, float) { return nullptr; }

	protected:
		State() = default;
	};
}
#endif
