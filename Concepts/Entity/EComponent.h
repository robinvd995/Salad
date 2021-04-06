#pragma once

#include "Entity.h"

enum class CmpTp {
	None = 0,
	Test
};

class Ent;

class EComponent {

public:
	virtual CmpTp getComponentType() { return CmpTp::None; };

	virtual void onAttached() {};
	virtual void onUpdate() {};
	virtual void onDetached() {};

private:
	Ent m_Entity;
};
