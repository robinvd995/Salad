#include <Salad.h>
#include <Salad/Core/EntryPoint.h>
#include "Salad/Core/ResourceLayer.h"

#include "SaladBowl/Core/Core.h"
#include "SaladBowl/EditorLayer.h"
#include "SaladBowl/TestLayer.hpp"

#include <iostream>

namespace Salad {

	class SaladBowl : public Application {
	public:
		SaladBowl() :
			Application("SaladBowl - Salad Editor") 		{
			pushLayer(new ResourceLayer(SLD_BOWL_RESOURCE_OUTPUT_LOCATION));

			//bool test = false;
			/*if (!test) */pushLayer(new EditorLayer());
			//else pushLayer(new TestLayer());
		}
		~SaladBowl() {}
	};

	Application* createApplication() {
		return new SaladBowl();
	}

	ImGuiLayer* createImGuiLayer() {
		return new ImGuiLayer(".editor/imgui.ini");
	}
}
