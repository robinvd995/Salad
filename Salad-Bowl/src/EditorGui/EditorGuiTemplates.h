#pragma once

#include <functional>
#include <string>

namespace Salad::EditorGui {

	struct ContextItemData {
		bool hasPrefixIcon = false;
		float iconU0 = 0.0f, iconV0 = 0.0f;
		float iconU1 = 0.0f, iconV1 = 0.0f;
		float iconW = 0.0f, iconH = 0.0f;
		bool hasAffix = false;
		const char* affix = nullptr;

		ContextItemData() = default;
		~ContextItemData() = default;

		ContextItemData& setPrefix(float u0, float v0, float u1, float v1, float w, float h) {
			iconU0 = u0; iconV0 = v0;
			iconU1 = u1; iconV1 = v1;
			iconW = w; iconH = h;
			hasPrefixIcon = true;
			return *this;
		}

		ContextItemData& setAffix(const char* a) {
			hasAffix = true;
			affix = a;
			return *this;
		}
	};

	void templateContextItem(const std::string& label, const char* name, ContextItemData& data, std::function<void()> selectedFunc);
}