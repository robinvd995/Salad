#pragma once

#include "EditorTexture.h"

namespace Salad::Asset {

	EditorTexture loadEditorTexture(std::string& filepath);
	void saveEditorTexture(EditorTexture& texture);

}