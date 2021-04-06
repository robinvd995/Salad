#include "EntityComponentTest.h"

#include <iostream>

void EntityComponentTest::onAttached() {
	std::cout << "Attached!" << std::endl;
}

void EntityComponentTest::onUpdate() {
	std::cout << "On update" << std::endl;
}

void EntityComponentTest::onDetached() {
	std::cout << "Detached!" << std::endl;
}

extern template std::shared_ptr<EntityComponentTest> Ent::getComponent();