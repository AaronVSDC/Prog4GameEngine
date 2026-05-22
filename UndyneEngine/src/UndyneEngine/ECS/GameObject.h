#ifndef GAME_OBJECT_H 
#define GAME_OBJECT_H

#include "BaseComponent.h"
#include "..\Core\Core.h"

//std
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace UndyneEngine
{

	class UNDYNE_API GameObject final
	{
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


		//---------------------
		//getters and setters
		//---------------------
		const std::string& getName() { return m_Name;  }
		void rename(const std::string& newName) { m_Name = newName;  }
		[[nodiscard]] const unsigned int getID() const { return m_ID;  }

		//----------------
		//component logic
		//----------------
		template<std::derived_from<BaseComponent> T, typename... Args> //cool cpp20 thing: "constrained template" 
		T* addComponent(Args&&... args)
		{
			auto component = std::make_unique<T>(std::forward<Args>(args)...);  
			component->m_pOwner = this; 
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
		std::vector<std::unique_ptr<BaseComponent>> m_Components; 
		static unsigned int s_NextID;

	};


}



#endif