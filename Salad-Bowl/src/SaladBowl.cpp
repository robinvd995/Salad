#include <Salad.h>
#include <Salad/Core/EntryPoint.h>

#include "EditorLayer.h"
#include "TestLayer.hpp"
#include "Salad/Core/ResourceLayer.h"

namespace Salad {

	class SaladBowl : public Application {
	public:
		SaladBowl() :
			Application("SaladBowl - Salad Editor")
		{
			pushLayer(new ResourceLayer());
			
			pushLayer(new EditorLayer());
			//pushLayer(new TestLayer());
		}
		~SaladBowl() {}
	};

	Application* createApplication() {
		return new SaladBowl();
	}
}
