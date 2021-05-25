#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch)
	: position(position), worldUp(worldUp), yaw(yaw), pitch(pitch)
{
	UpdateVectors();
}

void Camera::Rotate(glm::vec3 rotation)
{
	yaw += rotation.y;
	pitch = glm::clamp(pitch - rotation.x, -89.0f, 89.0f);
	UpdateVectors();
}

void Camera::UpdateVectors()
{
	front.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	front.y = glm::sin(glm::radians(pitch));
	front.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}
