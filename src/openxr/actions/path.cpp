////////////////////////////////////////////////////////////////////////////////////////////////
// Class for OpenXR Paths

#include "openxr/OpenXRApi.h"

using namespace godot;

Path::Path() {
	set_name("");
}

Path::Path(const char *p_name) {
	set_name(p_name);
}

Path::~Path() {
	// we can't destroy paths?
}

char *Path::get_name() const {
	return (char *)name;
}

bool Path::set_name(const char *p_name) {
	if (path != XR_NULL_PATH) {
		// we already created this path
		Godot::print("OpenXR: You can't set the name of a path after it has been created");
		return false;
	} else if (strlen(p_name) > XR_MAX_PATH_LENGTH) {
		memcpy(name, p_name, XR_MAX_PATH_LENGTH - 1);
		name[XR_MAX_PATH_LENGTH - 1] = '\0';
	} else {
		strcpy(name, p_name);
	}
	return true;
}

XrPath Path::get_path() const {
	return path;
}

bool Path::create(OpenXRApi *p_api) {
	if (path != XR_NULL_PATH) {
		return true;
	}

	// TODO We should find out what value path gets on failure (if any)

	XrResult result = xrStringToPath(p_api->instance, name, &path);
	if (!p_api->xr_result(result, "failed to create {0} path", name)) {
		return false;
	}

	Godot::print("Registered path {0} ({1})", name, (uint64_t)path);

	return true;
}

void Path::destroy() {
	// Strangely enough we can't destroy paths but they are cleaned up when the XR instance dies
	// I'm assuming if we re-register this path before its cleaned up, OpenXR will reuse the entry
	// It's even possible its a lookup into a table in OpenXR of available paths
	// So we just clear them.
	if (path != XR_NULL_PATH) {
		path = XR_NULL_PATH;

		Godot::print("Cleared path {0}", name);
	}
}
