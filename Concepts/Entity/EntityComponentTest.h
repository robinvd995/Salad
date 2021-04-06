#pragma once

#include "EComponent.h"

class EntityComponentTest : public EComponent {

public:
	virtual void onAttached() override;
	virtual void onUpdate() override;
	virtual void onDetached() override;

	static CmpTp getStaticType() { return CmpTp::Test; }
	virtual CmpTp getComponentType() override { return getStaticType(); }

};
