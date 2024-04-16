#include"GP2Camera.h"
#include"vulkanbase/VulkanBase.h"
#include"glfw/glfw3.h"

#include <chrono>

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"


GP2Camera::GP2Camera()
{
	GP2Camera({ 0,0,-50 }, 0, 0, 0);
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
	//SetVectors();
}

void GP2Camera::Update()
{
	//static auto startTime = std::chrono::high_resolution_clock::now();

	m_CurrentTime = std::chrono::high_resolution_clock::now();
	float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(m_CurrentTime - m_PreviousTime).count();
	m_PreviousTime = m_CurrentTime;

	//Camera Update Logic

	constexpr float cameraSpeed{ 25.0f };
	constexpr float rotationSpeed{ 25.0f };


	//Translate camera
	glm::vec3 directionVector{ ManageTranslateInput() };

	m_Origin += directionVector * cameraSpeed * deltaTime;

	if(m_IsRotating)
	{
		m_TotalYaw += m_RotationDirection.y * rotationSpeed * deltaTime;
		m_TotalPitch += m_RotationDirection.x * rotationSpeed * deltaTime;


		//SetVectors();
	}

	


	//Update Matrices
	if (m_IsMoving)
	{
		CalculateViewMatrix();
		CalculateProjectionMatrix();
		CalculateViewProjectionMatrix();

		m_IsMoving = true;
	}

}

glm::mat4 GP2Camera::GetViewProjectionMatrix() const
{
	return m_ViewProjectionMatrix;
};
glm::mat4 GP2Camera::GetViewMatrix() const
{
	return m_ViewMatrix;
};
glm::mat4 GP2Camera::GetProjectionMatrix() const
{
	//return MatrixInverse(m_ProjectionMatrix);
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
	//////TODO W1
	//const glm::mat4 onb{ MatrixCreateRotation(m_TotalPitch, m_TotalYaw, 0) * MatrixCreateTranslation(m_Origin) };

	//////ONB => invViewMatrix
	//m_InvViewMatrix = onb;

	////Inverse(ONB) => ViewMatrix
	//m_ViewMatrix = MatrixInverse(onb);

	////ViewMatrix => Matrix::CreateLookAtLH(...) [not implemented yet]
	///
	glm::mat4 finalRotation = glm::rotate(glm::mat4x4(1.f), glm::radians(m_TotalYaw), {0,1,0});
	finalRotation = glm::rotate(finalRotation, glm::radians(m_TotalPitch), glm::vec3{ 1,0,0 });

	m_Forward = glm::normalize(finalRotation[2]);

	m_Right = glm::normalize(glm::cross(m_Forward, {0,1,0}));
	m_ViewMatrix = glm::lookAt(m_Origin, m_Origin + m_Forward, {0,1,0});
}
void GP2Camera::CalculateProjectionMatrix()
{
	//m_ProjectionMatrix = MatrixCreatePerspectiveFovLH(m_Fov, m_AspectRatio, m_NearPlane, m_FarPlane);
	m_ProjectionMatrix = glm::perspective(m_Fov, m_AspectRatio, m_NearPlane, m_FarPlane);
}
void GP2Camera::CalculateViewProjectionMatrix()
{
	m_ViewProjectionMatrix = m_ViewMatrix * m_ProjectionMatrix;
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


glm::mat4 GP2Camera::MatrixCreateTranslation(const glm::vec3& t) const
{
	return
	{
		{1,0,0,0},
		{0,1,0,0},
		{0,0,1,0},
		{t,1}
	};
}
glm::mat4 GP2Camera::MatrixCreateRotation(const float pitch, const float yaw, const float roll) const
{
	const glm::mat4 rotationX
	{
		{1, 0, 0, 0},
		{0, std::cosf(pitch), std::sinf(pitch), 0},
		{0, -std::sinf(pitch), std::cosf(pitch), 0},
		{0, 0, 0, 1}
	};

	const glm::mat4 rotationY
	{
			{std::cosf(yaw), 0, -std::sinf(yaw), 0},
			{0, 1, 0, 0},
			{std::sinf(yaw), 0, std::cosf(yaw), 0},
			{0, 0, 0, 1}
	};

	const glm::mat4 rotationZ
	{
			{std::cosf(roll), std::sinf(roll), 0, 0},
			{-std::sinf(roll), std::cosf(roll), 0, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 1}
	};

	return  rotationX * rotationY * rotationZ;
}
glm::mat4 GP2Camera::MatrixInverse(const glm::mat4& mat) const
{
	//Optimized Inverse as explained in FGED1 - used widely in other libraries too.
	const glm::vec3& a = mat[0];
	const glm::vec3& b = mat[1];
	const glm::vec3& c = mat[2];
	const glm::vec3& d = mat[3];

	const float x = mat[0][3];
	const float y = mat[1][3];
	const float z = mat[2][3];
	const float w = mat[3][3];

	glm::vec3 s = glm::cross(a, b);
	glm::vec3 t = glm::cross(c, d);
	glm::vec3 u = a * y - b * x;
	glm::vec3 v = c * w - d * z;

	const float det = glm::dot(s, v) + glm::dot(t, u);
	const float invDet = 1.f / det;

	s *= invDet; t *= invDet; u *= invDet; v *= invDet;

	const glm::vec3 r0 = glm::cross(b, v) + t * y;
	const glm::vec3 r1 = glm::cross(v, a) - t * x;
	const glm::vec3 r2 = glm::cross(d, u) + s * w;
	//Vector3 r3 = Vector3::Cross(u, c) - s * z;

	return
	{
		glm::vec<4, float>{ r0.x, r1.x, r2.x, 0.f },
		glm::vec<4, float>{ r0.y, r1.y, r2.y, 0.f },
		glm::vec<4, float>{ r0.z, r1.z, r2.z, 0.f },
		{ -glm::dot(b, t),glm::dot(a, t),-glm::dot(d, s),glm::dot(c, s) }
	};
}
glm::mat4 GP2Camera::MatrixCreatePerspectiveFovLH(float fov, float aspect, float zn, float zf) const
{
	//assert(false && "Not Implemented");
	const glm::vec4 v1{ 1 / (aspect * fov), 0, 0, 0 };
	const glm::vec4 v2{ 0, 1 / fov, 0, 0 };
	const glm::vec4 v3{ 0, 0, zf / (zf - zn), 1 };
	const glm::vec4 v4{ 0, 0, -(zf * zn) / (zf - zn), 0 };

	return { v1,v2,v3,v4 };
}
glm::vec3 GP2Camera::MatrixTransformVector(const glm::mat4& mat,float x, float y, float z) const
{
	return glm::vec3
	{
		mat[0].x * x + mat[1].x * y + mat[2].x * z,
		mat[0].y * x + mat[1].y * y + mat[2].y * z,
		mat[0].z * x + mat[1].z * y + mat[2].z * z
	};
}

void GP2Camera::SetVectors()
{
	//applyRotation 
	const glm::mat4 rotationMatrix{ MatrixCreateRotation(m_TotalPitch, m_TotalYaw, 0) };
	m_Forward = MatrixTransformVector(rotationMatrix, 0, 0, 1);


	m_Forward = glm::normalize(rotationMatrix[2]);
	m_Forward.x *= -1;

	m_Right = glm::cross(m_Forward, { 0,1,0 });
	m_Right = glm::normalize(m_Right);

	m_Up = glm::cross(m_Forward, m_Right);
	m_Up = glm::normalize(m_Up);
}
