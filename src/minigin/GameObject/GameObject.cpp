#include "GameObject.h"
#include "ResourceManager.h"


namespace dae
{
	GameObject::~GameObject()
	{
	} 
	void GameObject::Update(float deltaTime)
	{
		if (m_PositionIsDirty) UpdateWorldPosition(); //todo: maybe move this to a late update instead.
		if (m_RotationIsDirty) UpdateWorldRotation();

		for (auto& component : m_Components)
		{
			component->Update(deltaTime);
		}

		auto iterator = std::remove_if(m_Components.begin(), m_Components.end(),
			[](const std::unique_ptr<BaseComponent>& component) { return component->m_PendingRemove; });
		m_Components.erase(iterator, m_Components.end());
	}

	void GameObject::FixedUpdate(float fixedTimeStep)
	{
		for (auto& component : m_Components)
		{
			if (component->m_Enabled) component->FixedUpdate(fixedTimeStep);
		}
	}

	void GameObject::Render() const
	{
		for (auto& component : m_Components)
		{
			if (component->m_Enabled) component->Render();
		}
	}

	void GameObject::Start()
	{
		for (auto& component : m_Components)
		{
			if (component->m_Enabled) component->Start();
		}
	}

	//void GameObject::onEnable()
	//{
	//	for (auto& component : m_Components)
	//	{
	//		if (component->m_EnabledIsDirty)
	//		{
	//			component->onEnable();
	//			component->m_EnabledIsDirty = false;
	//		}
	//	}
	//}

	//void GameObject::onDisable()
	//{
	//	for (auto& component : m_Components)
	//	{
	//		if (component->m_DisabledIsDirty)
	//		{
	//			component->onDisable();
	//			component->m_DisabledIsDirty = false;
	//		}
	//	}
	//}

	void GameObject::SetActive(bool active)
	{
		if (active and not m_IsActive)
		{
			for (auto& component : m_Components)
			{
				if (component->m_Enabled) component->m_EnabledIsDirty = true;
			}
		}
		if (not active and m_IsActive)
		{
			for (auto const& component : m_Components)
			{
				if (component->m_Enabled) component->m_DisabledIsDirty = true;
			}
		}
		for (auto const& child : m_Children)
		{
			child->SetActive(active);
		}
		m_IsActive = active;
	}

	GameObject* GameObject::GetParent() const
	{
		return m_pParent;
	}
	void GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
	{
		//Check if the new parent is valid (not itself or one of its children)
		if (IsChild(parent) || parent == this || m_pParent == parent)
			return;

		//Update position, rotation and scale
		if (parent == nullptr)
			SetLocalPosition(GetWorldPosition());
		else
		{
			if (keepWorldPosition)
				SetLocalPosition(GetWorldPosition() - parent->GetWorldPosition());
			SetPositionDirty();
		}

		//Remove itself from the previous parent
		if (m_pParent)
			m_pParent->m_Children.erase(
				std::remove(m_pParent->m_Children.begin(), m_pParent->m_Children.end(), this),
				m_pParent->m_Children.end());

		//Set the given parent on itself.
		m_pParent = parent;

		//Add itself as a child to the given parent
		if (m_pParent)
			m_pParent->m_Children.push_back(this);
	}
	size_t GameObject::GetChildCount() const
	{
		return m_Children.size();
	}
	bool GameObject::IsChild(GameObject* object) const
	{
		for (GameObject* child : m_Children)
		{
			if (child == object) return true;
		}

		return false;
	}

	GameObject* GameObject::GetChildAt(unsigned int index) const
	{
		return m_Children[index];
	}

	void GameObject::SetLocalPosition(const glm::vec3& pos)
	{
		m_LocalPosition = pos;
		SetPositionDirty();
	}

	void GameObject::SetPositionDirty()
	{
		m_PositionIsDirty = true;
		for (auto& child : m_Children)
			child->SetPositionDirty();
	}

	const glm::vec3& GameObject::GetWorldPosition()
	{
		if (m_PositionIsDirty) UpdateWorldPosition();
		return m_WorldPosition;
	}

	const glm::vec3& GameObject::GetLocalPosition() const
	{
		return  m_LocalPosition;
	}
	void GameObject::UpdateWorldPosition()
	{
		if (m_pParent == nullptr)
			m_WorldPosition = m_LocalPosition;
		else
			m_WorldPosition = m_pParent->GetWorldPosition() + m_LocalPosition;

		m_Transform.SetPosition(m_WorldPosition.x, m_WorldPosition.y, m_WorldPosition.z);
		m_PositionIsDirty = false;
	}

	void GameObject::SetLocalRotation(const float angleDegrees)
	{
		m_LocalRotation = glm::vec3(0.0f, 0.0f, glm::radians(angleDegrees));
		SetRotationDirty();
	}

	void GameObject::SetRotationDirty()
	{
		m_RotationIsDirty = true;
		for (auto& child : m_Children) child->SetRotationDirty();
	}

	const glm::vec3& GameObject::GetWorldRotation()
	{
		if (m_RotationIsDirty) UpdateWorldRotation();
		return m_WorldRotation;
	}

	const glm::vec3& GameObject::GetLocalRotation() const
	{
		return m_LocalRotation;
	}

	void GameObject::UpdateWorldRotation()
	{
		if (m_pParent == nullptr)
			m_WorldRotation = m_LocalRotation;
		else
			m_WorldRotation = m_pParent->GetWorldRotation() + m_LocalRotation;

		m_Transform.SetRotationEuler(m_WorldRotation.x, m_WorldRotation.y, m_WorldRotation.z);
		m_RotationIsDirty = false; 
	}

	void GameObject::AddTag(const std::string& tag)
	{
		m_Tag = tag;
	}

	bool GameObject::CompareTag(const std::string& tag) const
	{
		if (tag == m_Tag)
			return true;
		return false;
	}
	void GameObject::MarkForRemoval()
	{
		if (m_pendingRemoval) return;
		m_pendingRemoval = true;

		// detach from parent
		if (m_pParent) {
			auto& s = m_pParent->m_Children;
			s.erase(std::remove(s.begin(), s.end(), this), s.end());
			m_pParent = nullptr;
		}
		// detach children (or reparent to null)
		for (auto* c : m_Children) if (c) c->m_pParent = nullptr;
		m_Children.clear();
	}
}