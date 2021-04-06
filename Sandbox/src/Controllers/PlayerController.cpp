#include "sldpch.h"
#include "PlayerController.h"

void PlayerController::start() {
	SLD_INFO("Entity Player Start!");
}

void PlayerController::update(Salad::Timestep ts) {
	float speed = m_MoveSpeed * ts;

	glm::vec3 moveDirection(0.0f);

	if (Salad::Input::isKeyPressed(SLD_KEY_LEFT)) {
		moveDirection.x -= 1;
	}

	if (Salad::Input::isKeyPressed(SLD_KEY_RIGHT)) {
		moveDirection.x += 1;
	}

	if (Salad::Input::isKeyPressed(SLD_KEY_UP) && !Salad::Input::isKeyPressed(SLD_KEY_SPACE)) {
		moveDirection.y += 1;
	}

	if (Salad::Input::isKeyPressed(SLD_KEY_DOWN) && !Salad::Input::isKeyPressed(SLD_KEY_SPACE)) {
		moveDirection.y -= 1;
	}

	if (Salad::Input::isKeyPressed(SLD_KEY_UP) && Salad::Input::isKeyPressed(SLD_KEY_SPACE)) {
		moveDirection.z -= 1;
	}

	if (Salad::Input::isKeyPressed(SLD_KEY_DOWN) && Salad::Input::isKeyPressed(SLD_KEY_SPACE)) {
		moveDirection.z += 1;
	}

	bool moving = moveDirection.x != 0.0f || moveDirection.y != 0.0f || moveDirection.z != 0;

	if (moving) {
		glm::vec3 normalizedVector = glm::normalize(moveDirection);
		m_Transform->translate(normalizedVector * speed);
	}
}

void PlayerController::end() {}