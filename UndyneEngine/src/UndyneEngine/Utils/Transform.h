#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/vec3.hpp>

namespace UndyneEngine
{
	class GameObject; 
    class Transform final
    {
        friend class GameObject;
    public:
        Transform(glm::vec3 localPosition = glm::vec3(0.0f));
        Transform(float localX, float localY, float localZ);

        const glm::vec3& getLocalPosition() const noexcept { return m_LocalPosition; }
        void setLocalPosition(const glm::vec3& localPosition) noexcept;
        void setLocalPosition(float localX, float localY, float localZ = 0.0f) noexcept;
        const glm::vec3& getWorldPosition();
        void setDirty() noexcept;
    private:
        GameObject* m_Owner = nullptr;
        glm::vec3 m_LocalPosition{ 0.0f };
        glm::vec3 m_WorldPosition{ 0.0f };
        bool m_IsDirty = true;
    }; 
}


#endif