#include <Salad.h>
#include <Salad/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Salad {

	class SaladBowl : public Application {
	public:
		SaladBowl() :
			Application("SaladBowl - Salad Editor")
		{
			pushLayer(new EditorLayer());
		}
		~SaladBowl() {}
	};

	Application* createApplication() {
		return new SaladBowl();
	}

}


