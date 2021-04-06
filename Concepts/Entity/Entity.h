#pragma once

#include <vector>
#include <memory>

class EComponent;

class Ent {

public:

	template<class T>
	std::shared_ptr<T> getComponent();

	static void attachComponent(Ent& entity, std::shared_ptr<EComponent> component);
	void update();

private:
	std::vector<std::shared_ptr<EComponent>> m_ComponentMap;
};

class EntityComponentTest;
template std::shared_ptr<EntityComponentTest> Ent::getComponent();
