#pragma once

#include <glm/glm.hpp>
#include <chrono>

constexpr float PI{ 3.14159265f };
constexpr float TO_RADIANS{ PI / 180.0f };

struct GLFWwindow;
class GP2Camera final
{
public:
	GP2Camera();
	GP2Camera(const glm::vec3& origin, float fovAngle, const float width, const float height);

	void Update();


	//Simple Getters
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;

	glm::vec3 GetForward()const { return m_Forward; }
	glm::vec3 GetPosition() const { return m_Origin; }

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

	constexpr static glm::vec3 m_UnitX{1,0,0};
	constexpr static glm::vec3 m_UnitY{ 0,1,0 };
	constexpr static glm::vec3 m_UnitZ{ 0,0,1 };

	float m_TotalPitch{ 0.0f };
	float m_TotalYaw{ 3.14f };

	glm::mat4 m_ViewMatrix{};
	glm::mat4 m_ProjectionMatrix{};

	float m_NearPlane{ 0.1f };
	float m_FarPlane{ 500.0f };

	bool m_IsMoving{ true };
	glm::vec2 m_MoveDirection{};

	bool m_IsRotating{ false };
	glm::vec2 m_RotationDirection{0,0};
	glm::vec2 m_DragStart{};
	

	std::chrono::time_point<std::chrono::high_resolution_clock> m_PreviousTime{};
	std::chrono::time_point<std::chrono::high_resolution_clock> m_CurrentTime{};


	//Helpers functions
	void Initialize();

	void SetTranslation(glm::vec2 translation);

	void CalculateViewMatrix();
	void CalculateProjectionMatrix();

	glm::vec3 ManageTranslateInput() const;
};