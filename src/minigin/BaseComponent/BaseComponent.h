#ifndef BASE_COMPONENT_H
#define BASE_COMPONENT_H
#include <cassert>

namespace dae
{
	class GameObject;
	class BaseComponent
	{
		friend class GameObject;
	public:

		virtual ~BaseComponent() = default;
		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;

		virtual void Update(float) {}
		virtual void FixedUpdate(float) {}
		virtual void Render() const {}
		virtual void Start() {}

		virtual GameObject* GetOwner() const { return m_pOwner; }
		void SetOwner(GameObject* owner)
		{
			assert(owner != nullptr and "BaseComponent::setOwner: owner is nullptr");
			if (!owner)
			{
				return;
			}
			m_pOwner = owner;
			// Note: SetOwner is called by GameObject::AddComponent<T> after the component is created
		}
		void SetEnabled(bool enabled)
		{
			if (enabled and not m_Enabled)
			{
				m_EnabledIsDirty = true;
			}
			else if (not enabled and m_Enabled)
			{
				m_DisabledIsDirty = true;
			}
			m_Enabled = enabled;
		}
		bool m_PendingRemove = false;
	protected:
		//BaseComponent(GameObject& owner) { m_pOwner = &owner; }
		BaseComponent() = default;

	private:
		GameObject* m_pOwner = nullptr;
		bool m_Enabled = true;
		bool m_EnabledIsDirty = true;
		bool m_DisabledIsDirty = false;


	};
}
#endif