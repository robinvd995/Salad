#pragma once

#include <cstdint>
#include <vector>

namespace Salad {

	enum class EditorEventType {
		ViewportResized,
		StateChanged
	};

	struct EditorEvent {
		EditorEventType type;

		EditorEvent() = delete;
		EditorEvent(const EditorEvent&) = delete;
		EditorEvent(EditorEventType p_type) : type(p_type) {}
	};

	struct EditorEventViewportResized : public EditorEvent {

		uint32_t width = 0, height = 0;

		EditorEventViewportResized() = delete;
		EditorEventViewportResized(const EditorEventViewportResized&) = delete;
		EditorEventViewportResized(uint32_t p_width, uint32_t p_height) : EditorEvent(EditorEventType::ViewportResized),
			width(p_width), height(p_height)
		{}
	};

	struct EditorEventStateChanged : public EditorEvent {
	
		uint32_t state;

		EditorEventStateChanged() = delete;
		EditorEventStateChanged(const EditorEventStateChanged&) = delete;
		EditorEventStateChanged(uint32_t p_state) : EditorEvent(EditorEventType::StateChanged),
			state(p_state)
		{}
	};

	class EditorEventListener {
	
	public:
		virtual void onEditorEvent(const EditorEvent& evnt) = 0;
	};

	class EditorEventManager {
	
	public:
		EditorEventManager() {}
		~EditorEventManager() {}
	
		static void registerListener(EditorEventListener* listener) { s_Instance->m_EventListeners.push_back(listener); }

		static void submitEvent(EditorEvent& evnt) { for (EditorEventListener* listener : s_Instance->m_EventListeners) listener->onEditorEvent(evnt); }

	private:
		inline static EditorEventManager* s_Instance;

		friend class EditorLayer;

	private:
		std::vector<EditorEventListener*> m_EventListeners;
	};
}
