#ifndef GAME_OBJECT_H 
#define GAME_OBJECT_H
#include <memory>
#include <string>
#include <vector>
#include "BaseComponent.h"
#include "Transform.h"
namespace dae
{
	class Texture2D;

	class GameObject final
	{
	public:
		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Update(float deltaTime);
		void FixedUpdate(float fixedTimeStep);
		void Render() const;

		void Start();
		void SetActive(bool active);


		Transform GetTransform() const { return m_Transform; }
		bool GetIsActive() const { return m_IsActive; }

		//component stuff
		void AddComponent(std::unique_ptr<BaseComponent> component)
		{
			component->SetOwner(this);
			m_Components.emplace_back(std::move(component));
		}
		template <typename T> void RemoveComponent()
		{

			if (m_Components.empty())
				return;

			for (auto& component : m_Components)
			{
				if (auto* castedComponent = dynamic_cast<T*>(component.get()))
				{
					castedComponent->m_PendingRemove = true;
				}
			}
		}

		template <typename T> T* GetComponent()
		{

			if (m_Components.empty())
				return nullptr;

			for (const auto& component : m_Components)
			{
				if (auto* castedComponent = dynamic_cast<T*>(component.get()))
				{
					return castedComponent;
				}
			}

			return nullptr;
		}
		template <typename T> std::vector<T*> GetComponents()
		{
			std::vector<T*> components;
			for (auto& component : m_Components) {
				if (auto casted = dynamic_cast<T*>(component.get())) {
					components.push_back(casted);
				}
			}
			return components;
		}
		template <typename T> bool HasComponent() const
		{
			for (const auto& component : m_Components)
			{
				if (auto* castedComponent = dynamic_cast<T*>(component.get()))
				{
					return true;
				}
			}

			return false;
		}

		// PARENT-CHILD 
		void SetParent(GameObject* parent, bool keepWorldPosition);
		GameObject* GetParent() const;
		size_t GetChildCount() const;
		GameObject* GetChildAt(unsigned int index) const;
		bool IsChild(GameObject* object) const;

		// TRANSFORM
		void SetPositionDirty();
		void SetLocalPosition(const glm::vec3& pos);
		const glm::vec3& GetLocalPosition() const;
		const glm::vec3& GetWorldPosition();
		void UpdateWorldPosition();
		void SetRotationDirty();
		void SetLocalRotation(const float angleDegrees);
		const glm::vec3& GetLocalRotation() const;
		const glm::vec3& GetWorldRotation();
		void UpdateWorldRotation();


		//TAGS
		void AddTag(const std::string& tag);
		bool CompareTag(const std::string& tag) const;
		void MarkForRemoval();
		bool IsPendingRemoval() const noexcept { return m_pendingRemoval; }



	private:
		bool m_pendingRemoval = false;

		Transform m_Transform = {};
		std::vector<std::unique_ptr<BaseComponent>> m_Components = {};

		GameObject* m_pParent = nullptr;
		std::vector<GameObject*> m_Children = {};

		bool m_PositionIsDirty = false;
		bool m_RotationIsDirty = false;

		bool m_IsActive = true;
		glm::vec3 m_LocalPosition = {};
		glm::vec3 m_WorldPosition = {};
		glm::vec3 m_LocalRotation = {};
		glm::vec3 m_WorldRotation = {};

		std::string m_Tag = {};

	};
}
#endif