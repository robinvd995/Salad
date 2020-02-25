#pragma once

#include <Salad.h>
#include <Salad/OrthographicCameraController.h>

class Sandbox2D : public Salad::Layer {

public: 
	Sandbox2D();
	virtual void onAttach() override;
	virtual void onDetach() override;

	virtual void onUpdate(Salad::Timestep ts) override;
	virtual void onImGuiRender() override;
	virtual void onEvent(Salad::Event& e) override;

private:
	Salad::OrthographicCameraFollower m_CameraController;
	
	// Temporary!!!
	Salad::Ref<Salad::VertexArray> m_VertexArray;
	Salad::Ref<Salad::Shader> m_Shader;

	Salad::Ref<Salad::Texture2D> m_Texture;

	Salad::Ref<Salad::Entity> m_Entity;

	Salad::Ref<Salad::Sprite> m_Sprite;

	glm::vec3 m_SqColor = { 0.2f, 0.3f, 0.8f };

	Salad::Ref<Salad::TileMap> m_TileMap;
};