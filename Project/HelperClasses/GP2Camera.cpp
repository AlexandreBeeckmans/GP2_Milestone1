#include"GP2Camera.h"
#include"vulkanbase/VulkanBase.h"
#include"glfw/glfw3.h"

#include <chrono>

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

GP2Camera::GP2Camera() :
	GP2Camera({ 0,0,-50 }, 0, 0, 0)
{
}

GP2Camera::GP2Camera(const glm::vec3& _origin, const float _fovAngle, const float width, const float height) :
	m_Origin{ _origin },
	m_FovAngle{ _fovAngle },
	m_AspectRatio{ width / height }


{
	m_PreviousTime = std::chrono::high_resolution_clock::now();
	Initialize();
}

void GP2Camera::Initialize()
{
	m_Fov = std::tanf((m_FovAngle * TO_RADIANS) / 2.f);

	CalculateViewMatrix();
	CalculateProjectionMatrix();
}

void GP2Camera::Update()
{
	//Update time
	m_CurrentTime = std::chrono::high_resolution_clock::now();
	float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(m_CurrentTime - m_PreviousTime).count();
	m_PreviousTime = m_CurrentTime;

	//Camera Update Logic
	constexpr float cameraSpeed{ 25.0f };
	constexpr float rotationSpeed{ 25.0f };

	//Translate camera
	glm::vec3 directionVector{ ManageTranslateInput() };
	m_Origin += directionVector * cameraSpeed * deltaTime;

	//Rotate camera
	if(m_IsRotating)
	{
		m_TotalYaw += m_RotationDirection.y * rotationSpeed * deltaTime;
		m_TotalPitch += m_RotationDirection.x * rotationSpeed * deltaTime;
	}

	//Update Matrices
	if (m_IsMoving || m_IsRotating)
	{
		CalculateViewMatrix();
		CalculateProjectionMatrix();
	}

}

glm::mat4 GP2Camera::GetViewMatrix() const
{
	return m_ViewMatrix;
};
glm::mat4 GP2Camera::GetProjectionMatrix() const
{
	return m_ProjectionMatrix;
}

void GP2Camera::SetTranslation(glm::vec2 translation)
{
	m_IsMoving = translation.length() != 0;
	m_MoveDirection = glm::normalize(translation);
}

void GP2Camera::KeyEvent(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		SetTranslation({ 0,1 });
		return;
	}
	if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		SetTranslation({ 0,-1 });
		return;
	}

	if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		SetTranslation({ -1,0 });
		return;
	}
	if (key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		SetTranslation({ 1,0 });
		return;
	}
	SetTranslation({ 0,0 });
}
void GP2Camera::MouseMove(GLFWwindow* window, double xPos, double yPos)
{
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	if (state == GLFW_PRESS)
	{
		float dx = static_cast<float>(xPos) - m_DragStart.x;
		if (dx < 0) 
		{
			m_RotationDirection.y = 1;
		}
		else if(dx > 0)
		{
			m_RotationDirection.y = -1;
		}
		else
		{
			m_RotationDirection.y = 0;
		}

		float dy = static_cast<float>(yPos) - m_DragStart.y;
		if (dy > 0)
		{
			m_RotationDirection.x = 1;
		}
		else if (dy < 0)
		{
			m_RotationDirection.x = -1;
		}
		else
		{
			m_RotationDirection.x = 0;
		}
	}
}
void GP2Camera::MouseEvent(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		double xPos;
		double yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		m_DragStart.x = static_cast<float>(xPos);
		m_DragStart.y = static_cast<float>(yPos);
		m_IsRotating = true;
		return;
	}

	m_IsRotating = false;
}

void GP2Camera::CalculateViewMatrix()
{
	glm::mat4 finalRotation = glm::rotate(glm::mat4x4(1.f), glm::radians(m_TotalYaw), {0,1,0});
	finalRotation = glm::rotate(finalRotation, glm::radians(m_TotalPitch), glm::vec3{ 1,0,0 });

	m_Forward = glm::normalize(finalRotation[2]);

	m_Right = glm::normalize(glm::cross(m_Forward, {0,1,0}));
	m_ViewMatrix = glm::lookAt(m_Origin, m_Origin + m_Forward, {0,1,0});
}
void GP2Camera::CalculateProjectionMatrix()
{
	m_ProjectionMatrix = glm::perspective(m_Fov, m_AspectRatio, m_NearPlane, m_FarPlane);
}

glm::vec3 GP2Camera::ManageTranslateInput() const
{
	if (m_MoveDirection.x > 0)
		return m_Right;
	if (m_MoveDirection.x < 0)
		return -m_Right;
	if (m_MoveDirection.y > 0)
		return m_Forward;
	if (m_MoveDirection.y < 0)
		return -m_Forward;
	return{};
}
