#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <glm/glm.hpp>
namespace dae
{
	class Transform final
	{
	public:
		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y, float z);
		void SetPosition(const glm::vec3& position);
		const glm::vec3& GetRotationEuler() const { return m_RotationEuler; }
		void SetRotationEuler(float x, float y, float z) { m_RotationEuler = { x, y, z }; }

	private:
		glm::vec3 m_Position;
		glm::vec3 m_RotationEuler{ 0.0f };

	};
}
#endif