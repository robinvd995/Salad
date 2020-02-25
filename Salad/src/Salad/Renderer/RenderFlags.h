#pragma once

namespace Salad {

	class RenderFlags {
	
	public:
		virtual unsigned int getFlagStaticDraw() const = 0;
		virtual unsigned int getFlagDynamicDraw() const = 0;
	};

}