#pragma once

#include <glm/ext/matrix_transform.hpp>

class Camera
{
	glm::vec3 position, front, up, right, worldUp;
	float yaw, pitch;

public:
	explicit Camera(glm::vec3 position, glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = -90.0f, float pitch = 0.0f);

	inline glm::mat4 GetView() const
	{
		return glm::lookAt(position, position + front, up);
	}

	inline void Move(glm::vec3 velocity)
	{
		position += velocity.x * right + velocity.y * up + velocity.z * front;
	}

	void Rotate(glm::vec3 rotation);

	inline glm::vec3 GetPosition() const { return position; }

private:
	void UpdateVectors();
};
