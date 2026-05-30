#ifndef BASE_COMPONENT_H
#define BASE_COMPONENT_H
#include "../Core/Core.h"

namespace UndyneEngine
{
	class GameObject;
	class UNDYNE_API BaseComponent
	{
		friend class GameObject;
	public:

		virtual ~BaseComponent() = default;
		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;

		virtual void update(float) {}
		virtual void fixedUpdate(float) {}
		virtual void render() const {}
		virtual void start() {}

		GameObject* getOwner() const { return m_Owner; }

	protected:
		BaseComponent() = default;

	private:
		GameObject* m_Owner = nullptr;



	};

}
#endif