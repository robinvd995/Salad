#pragma once

#include <glm/glm.hpp>

namespace Salad {

	struct PerspectiveCameraProperties {
	
		float Fov, AspectRatio, NearClip, FarClip;

		PerspectiveCameraProperties()
			: Fov(65.0f), AspectRatio(16.0f / 9.0f), NearClip(0.1f), FarClip(1000.0f)
		{}
	};

	struct OrthographicCameraProperties {
	
		float Left, Right, Top, Bottom, ZNear, ZFar;

		OrthographicCameraProperties()
			: Left(-16.0f), Right(16.0f), Top(9.0f), Bottom(-9.0f), ZNear(-1.0f), ZFar(1.0f)
		{}
	};

	class Camera {

	public:
		enum class CameraType {
			Perspective = 0, Orthographic = 1
		};

	public:
		Camera();
		Camera(CameraType projectionType, const glm::mat4& projection);
		Camera(PerspectiveCameraProperties props);
		Camera(OrthographicCameraProperties props);
		~Camera();

		inline const glm::mat4& getProjection() const { return m_Projection; }

		inline const bool isPrimaryCamera() const { return m_IsPrimaryCamera; }
		inline void setPrimaryCamera(bool primary) { m_IsPrimaryCamera = primary; }

		inline CameraType getCameraProjectionType() { return m_CameraProjectionType; }
		void setProjectionType(CameraType type) { m_CameraProjectionType = type; }

		void setPerspectiveProjection(PerspectiveCameraProperties props) { m_PerspectiveProperties = props; }
		void setOrthographicProjection(OrthographicCameraProperties props) { m_OrthographicProperties = props; }

		PerspectiveCameraProperties& getPerspectiveProperties() { return m_PerspectiveProperties; }
		OrthographicCameraProperties& getOrthographicProperties() { return m_OrthographicProperties; }

		// Rename to recalculate projection matrix
		void recalculateViewMatrix();

	private:
		bool m_IsPrimaryCamera{ true };
		bool m_FixedAspectRatio{ false };
		CameraType m_CameraProjectionType{ CameraType::Perspective };
		glm::mat4 m_Projection{ 1.0f };

		PerspectiveCameraProperties m_PerspectiveProperties;
		OrthographicCameraProperties m_OrthographicProperties;

	};

}