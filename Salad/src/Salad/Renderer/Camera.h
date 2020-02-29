#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Salad {

	class Camera {

	public:
		const virtual glm::mat4& getProjectionMatrix() const = 0;
		const virtual glm::mat4& getViewProjectionMatrix() const = 0;
		const virtual glm::mat4& getViewMatrix() const = 0;
	};

	struct PerspectiveCameraProps {
		float fov;
		float aspectRatio;
		float nearPlane;
		float farPlane;

		PerspectiveCameraProps(float pFov = 65.0f, float pAspectRatio = (16.0f / 9.0f), float pNearPlane = 0.1f, float pFarPlane = 1000.0f) :
			fov(pFov), aspectRatio(pAspectRatio), nearPlane(pNearPlane), farPlane(pFarPlane)
		{}
	};

	class PerspectiveCamera : public Camera {

	public:
		PerspectiveCamera(PerspectiveCameraProps properties);
		~PerspectiveCamera();

		const virtual glm::mat4& getProjectionMatrix() const override { return m_ProjectionMatrix; }
		const virtual glm::mat4& getViewProjectionMatrix() const override { return m_ViewProjectionMatrix; }
		const virtual glm::mat4& getViewMatrix() const override { return m_ViewMatrix; }

		void recalculateViewMatrix();

	private:
		PerspectiveCameraProps m_Properties;
		
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };

	};

	class OrthographicCamera {

	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void setProjection(float left, float right, float bottom, float top);

		const glm::vec3& getPosition() const { return m_Position; }
		float getRotation() const { return m_Rotation; }

		void setPosition(const glm::vec3& position) { m_Position = position; recalculateViewMatrix(); }
		void setRotation(float rotation) { m_Rotation = rotation; recalculateViewMatrix(); }

		const glm::mat4& getProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& getViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& getViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void recalculateViewMatrix();

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
	};
}