#pragma once

#include "Salad/Scene/Entity.h"

namespace Salad {

	class EditorSelectionContext {

	public:

		EditorSelectionContext() = default;
		~EditorSelectionContext() {}

		Entity getSelectionContext() { return m_SelectedEntity; }
		bool hasSelectionContext() { return m_SelectedEntity.isValid(); }
		void setSelectionContext(Entity entity) { m_SelectedEntity = entity; }
		bool isEntitySelected(Entity entity) { return m_SelectedEntity == entity; }

	private:
		Entity m_SelectedEntity;
	};

}


