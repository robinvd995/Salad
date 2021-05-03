#include <Salad.h>
#include <Salad/Core/EntryPoint.h>

#include "Core/Core.h"

#include "EditorLayer.h"
#include "TestLayer.hpp"
#include "Salad/Core/ResourceLayer.h"

#include <iostream>

namespace Salad {

	class SaladBowl : public Application {
	public:
		SaladBowl() :
			Application("SaladBowl - Salad Editor")
		{
			pushLayer(new ResourceLayer(SLD_BOWL_RESOURCE_OUTPUT_LOCATION));
			
			//pushLayer(new EditorLayer());
			pushLayer(new TestLayer());
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
