////////////////////////////////////////////////////////////////////////////////////////////////
// Just exposing an interface to the OS functions reachable in GDNative
// Basically just ported some of the functions in the godot-cpp

#ifndef OS_H
#define OS_H

#include "GodotCalls.h"

class OSNative {
private:
	static OSNative *singleton;
	godot_object *_os_singleton = nullptr;

	godot_method_bind *mb_get_ticks_msec = nullptr;
	godot_method_bind *mb_get_screen_size = nullptr;
	godot_method_bind *mb_get_current_video_driver = nullptr;
	godot_method_bind *mb_get_native_handle = nullptr;

public:
	enum HandleType {
		APPLICATION_HANDLE, // HINSTANCE, NSApplication*, UIApplication*, JNIEnv* ...
		DISPLAY_HANDLE, // X11::Display* ...
		WINDOW_HANDLE, // HWND, X11::Window*, NSWindow*, UIWindow*, Android activity ...
		WINDOW_VIEW, // HDC, NSView*, UIView*, Android surface ...
		OPENGL_CONTEXT // HGLRC, X11::GLXContext, NSOpenGLContext*, EGLContext* ...
	};

	static OSNative *get_singleton();
	static void cleanup_singleton();

	OSNative();
	~OSNative();

	int64_t get_ticks_msec();
	godot_vector2 get_screen_size(const int64_t screen = -1);
	int64_t get_current_video_driver();
	void *get_native_handle(godot_int p_handle_type);
};

#endif /* !OS_H */
