#include"GP2Camera.h"
#include"vulkanbase/VulkanBase.h"
#include"glfw/glfw3.h"

#include <chrono>


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
}

void GP2Camera::Update()
{
	//static auto startTime = std::chrono::high_resolution_clock::now();

	m_CurrentTime = std::chrono::high_resolution_clock::now();
	float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(m_CurrentTime - m_PreviousTime).count();
	m_PreviousTime = m_CurrentTime;

	//Camera Update Logic

	constexpr float cameraSpeed{ 5.0f };
	constexpr float rotationSpeed{ 5.0f };


	//Translate camera
	glm::mat4 translateMatrix{};
	translateMatrix = MatrixCreateTranslation({ 1, 1, 1 });

	glm::vec3 directionVector{ ManageTranslateInput() };
	//if (pKeyboardState[SDL_SCANCODE_LSHIFT]) directionVector *= m_Boost;

	m_Origin += directionVector * cameraSpeed * deltaTime;

	//Rotation
	//if ((mouseState & SDL_BUTTON(1)) || (mouseState & SDL_BUTTON(3)))
	//{
	//	m_IsMoving = true;

	//	//avoid double press
	//	if (!((mouseState & SDL_BUTTON(1)) && (mouseState & SDL_BUTTON(3))))
	//	{
	//		//Rotate around Y axis
	//		m_TotalYaw += static_cast<float>(mouseX) * TO_RADIANS;

	//		if (mouseState & SDL_BUTTON(3))
	//		{
	//			//Rotate around X axis
	//			m_TotalPitch -= static_cast<float>(mouseY) * TO_RADIANS;
	//		}

	//		//applyRotation 
	//		const Matrix rotationMatrix{ Matrix::CreateRotation(m_TotalPitch, m_TotalYaw, 0) };
	//		m_Forward = rotationMatrix.TransformVector(Vector3::UnitZ) * deltaTime * rotationSpeed;
	//		m_Forward.Normalize();

	//		m_Right = Vector3::Cross(Vector3::UnitY, m_Forward);
	//		m_Right.Normalize();

	//		m_Up = Vector3::Cross(m_Forward, m_Right);
	//		m_Up.Normalize();
	//	}
	//}


	//Update Matrices
	if (m_IsMoving)
	{
		CalculateViewMatrix();
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
	return m_InvViewMatrix;
};
glm::mat4 GP2Camera::GetProjectionMatrix() const
{
	return m_ProjectionMatrix;
}

void GP2Camera::SetTranslation(glm::vec2 translation)
{
	m_IsMoving = translation.length() != 0;
	m_MoveDirection = glm::normalize(translation);
};

void GP2Camera::CalculateViewMatrix()
{
	////TODO W1
	const glm::mat4 onb{ MatrixCreateRotation(m_TotalPitch, m_TotalYaw, 0) * MatrixCreateTranslation(m_Origin) };

	////ONB => invViewMatrix
	m_InvViewMatrix = onb;

	//Inverse(ONB) => ViewMatrix
	m_ViewMatrix = MatrixInverse(onb);

	//ViewMatrix => Matrix::CreateLookAtLH(...) [not implemented yet]
	m_Forward = MatrixTransformVector(onb, 0, 0, 1);
	m_Forward = glm::normalize(m_Forward);
}
void GP2Camera::CalculateProjectionMatrix()
{
	m_ProjectionMatrix = MatrixCreatePerspectiveFovLH(m_Fov, m_AspectRatio, m_NearPlane, m_FarPlane);
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
		{0, std::cosf(pitch), -std::sinf(pitch), 0},
		{0, std::sinf(pitch), std::cosf(pitch), 0},
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

	return rotationX * rotationY * rotationZ;
}
glm::mat4 GP2Camera::MatrixInverse(const glm::mat4& mat)
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
