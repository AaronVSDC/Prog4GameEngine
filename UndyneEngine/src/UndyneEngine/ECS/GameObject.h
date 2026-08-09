#ifndef GAME_OBJECT_H 
#define GAME_OBJECT_H

#include "BaseComponent.h"
#include "../Log/Log.h"
#include "../Utils/Transform.h"

//std
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace UndyneEngine
{
	class Scene;

	class GameObject final
	{
		friend class Scene;
	public:
		GameObject(std::optional<std::string> name = std::nullopt);
		~GameObject() = default;
		GameObject(const GameObject& other) = delete;

		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void start();
		void update(float deltaTime);
		void fixedUpdate(float fixedTimeStep);
		void render() const; 

		void markForRemoval() noexcept; 

		//---------------------
		//getters and setters
		//---------------------
		const std::string& getName() const noexcept { return m_Name;  }
		void rename(const std::string& newName) noexcept { m_Name = newName;  }
		[[nodiscard]] unsigned int getID() const noexcept { return m_ID;  }
		GameObject* getParent() const noexcept { return m_Parent;  }
		const std::vector<GameObject*>& getChildren() const noexcept { return m_Children;  }
		Transform& getTransform() noexcept { return m_Transform;  }
		Scene* getScene() const noexcept { return m_Scene;  }
	    bool getMarkedForRemoval() const noexcept { return m_MarkedForRemoval;  }

		//--------------------
		//parent child logic
		//--------------------
		void setParent(GameObject* newParent, bool keepWorldPosition = false); 
		void markChildrenTransformDirty();

		//----------------
		//component logic
		//----------------
		template<std::derived_from<BaseComponent> T, typename... Args> 
		T* addComponent(Args&&... args)
		{
			auto component = std::make_unique<T>(std::forward<Args>(args)...);  
			component->m_Owner = this; 
			T* raw = component.get(); 
			m_Components.emplace_back(std::move(component)); 
			return raw; 
		}

		template<std::derived_from<BaseComponent> T>
		void removeComponent()
		{
			std::erase_if(m_Components,
				[](const std::unique_ptr<BaseComponent>& component)
				{ return dynamic_cast<T*>(component.get()) != nullptr; }); 
		}

		template<std::derived_from<BaseComponent> T>
		T* getComponent() const
		{
			for (const auto& component : m_Components)
			{
				if (T* casted = dynamic_cast<T*>(component.get()))
					return casted; 
			}
			return nullptr; 
		}

		template<std::derived_from<BaseComponent> T>
		bool hasComponent() const
		{
			return getComponent<T>() != nullptr; 
		}
	private:
		const unsigned int m_ID;
		std::string m_Name;
		static unsigned int s_NextID;
		std::vector<std::unique_ptr<BaseComponent>> m_Components; 
		std::vector<GameObject*> m_Children;
		GameObject* m_Parent = nullptr;
		Transform m_Transform;
		bool m_MarkedForRemoval = false;
		Scene* m_Scene = nullptr;



	};


}



#endif