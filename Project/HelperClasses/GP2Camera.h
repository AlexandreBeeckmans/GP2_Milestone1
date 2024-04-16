#pragma once

#include <glm/glm.hpp>
#include <chrono>
//#include <vulkan/vulkan_core.h>

//#include "GLFW/glfw3.h"

constexpr float PI{ 3.14159265f };
constexpr float TO_RADIANS{ PI / 180.0f };

struct GLFWwindow;
class GP2Camera final
{
public:
	GP2Camera();
	GP2Camera(const glm::vec3& _origin, float _fovAngle, const float width, const float height);

	void Update();


	//Simple Getters
	glm::mat4 GetViewProjectionMatrix() const;
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;

	void KeyEvent(int key, int scancode, int action, int mods);
	void MouseMove(GLFWwindow* window, double xPos, double yPos);
	void MouseEvent(GLFWwindow* window, int button, int action, int mods);


private:
	glm::vec3 m_Origin{};
	float m_FovAngle{ 45.f };
	float m_Fov{ tanf((m_FovAngle * TO_RADIANS) / 2.f) };
	float m_AspectRatio{};

	glm::vec3 m_Forward{ glm::vec3{0,0,1 } };
	glm::vec3 m_Up{ glm::vec3{0,1,0 } };
	glm::vec3 m_Right{ glm::vec3{1,0,0} };

	float m_TotalPitch{ 0.0f };
	float m_TotalYaw{ 3.14f };

	glm::mat4 m_InvViewMatrix{};
	glm::mat4 m_ViewMatrix{};
	glm::mat4 m_ProjectionMatrix{};
	glm::mat4 m_ViewProjectionMatrix{};

	float m_NearPlane{ 0.1f };
	float m_FarPlane{ 500.0f };

	bool m_IsMoving{ true };

	glm::vec2 m_MoveDirection{};
	glm::vec2 m_RotationDirection{0,0};
	glm::vec2 m_DragStart{};
	bool m_IsRotating{ false };

	std::chrono::time_point<std::chrono::high_resolution_clock> m_PreviousTime{};
	std::chrono::time_point<std::chrono::high_resolution_clock> m_CurrentTime{};


	//Helpers functions
	void Initialize();

	void SetTranslation(glm::vec2 translation);

	void CalculateViewMatrix();
	void CalculateProjectionMatrix();
	void CalculateViewProjectionMatrix();

	glm::vec3 ManageTranslateInput() const;


	glm::mat4 MatrixCreateTranslation(const glm::vec3& t) const;
	glm::mat4 MatrixCreateRotation(const float pitch, const float yaw, const float roll) const;
	glm::mat4 MatrixInverse(const glm::mat4& mat) const;
	glm::mat4 MatrixCreatePerspectiveFovLH(float fov, float aspect, float zn, float zf) const;
	glm::vec3 MatrixTransformVector(const glm::mat4& mat, float x, float y, float z) const;

	void SetVectors();
};