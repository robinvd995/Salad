#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Salad {

	struct CameraProps {
		float fov;
		float aspectRatio;
		float nearPlane;
		float farPlane;

		CameraProps(float pFov = 65.0f, float pAspectRatio = (16.0f / 9.0f), float pNearPlane = 0.1f, float pFarPlane = 1000.0f) :
			fov(pFov), aspectRatio(pAspectRatio), nearPlane(pNearPlane), farPlane(pFarPlane)
		{}
	};

	class Camera {

	public:
		Camera(CameraProps properties);
		~Camera();

		void calculateViewMatrix();

		inline glm::mat4 getViewMatrix() const { return m_ViewMatrix; }
		inline glm::mat4 getProjectionMatrix() const { return m_ProjectionMatrix; }

	private:
		glm::vec3 m_Position;
		glm::quat m_Rotation;

		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;

		CameraProps m_Properties;
	};

}