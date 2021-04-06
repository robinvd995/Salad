#pragma once

#include <Salad.h>

class PlayerController : public Salad::EntityBehaviour {

public:
	virtual void start() override;
	virtual void update(Salad::Timestep ts) override;
	virtual void end() override;

private:
	float m_MoveSpeed = 3.0f;

};
