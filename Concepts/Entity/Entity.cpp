#include "Entity.h"
#include "EComponent.h"

#include <iostream>

template<typename T>
std::shared_ptr<T> Ent::getComponent() {
	for (auto it = m_ComponentMap.begin(); it != m_ComponentMap.end(); it++) {
		if((*it)->getComponentType() == T::getStaticType()) {
			return *it;
		}
	}
}

void Ent::attachComponent(Ent& entity, std::shared_ptr<EComponent> component) {
	entity.m_ComponentMap.push_back(component);
	component->onAttached();
}

void Ent::update(){
	for(auto it = m_ComponentMap.begin(); it != m_ComponentMap.end(); it++){
		(*it)->onUpdate();
	}
}
