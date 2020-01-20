#include "Salad.h"

class Sandbox : public Salad::Application {
public:
	Sandbox() {}
	~Sandbox() {}
};

Salad::Application* Salad::createApplication() {
	return new Sandbox();
}