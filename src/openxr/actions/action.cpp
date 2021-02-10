////////////////////////////////////////////////////////////////////////////////////////////////
// Class for OpenXR Actions

#include "openxr/OpenXRApi.h"

using namespace godot;

Action::Action(XrActionType p_type, const char *p_name, const char *p_localised_name) {
	type = p_type;
	set_name(p_name);
	set_localised_name(p_localised_name);
}

Action::~Action() {
	// no need to delete our paths, we don't own them

	if (handle != XR_NULL_HANDLE) {
		xrDestroyAction(handle);
	}
}

XrActionType Action::get_type() const {
	return type;
}

void Action::set_type(XrActionType p_type) {
	type = p_type;
}

char *Action::get_name() const {
	return (char *)name;
}

bool Action::set_name(const char *p_name) {
	if (handle != XR_NULL_HANDLE) {
		return false;
	}

	strcpy(name, p_name);
	return true;
}

char *Action::get_localised_name() const {
	return (char *)localised_name;
}

bool Action::set_localised_name(const char *p_name) {
	if (handle != XR_NULL_HANDLE) {
		return false;
	}

	strcpy(localised_name, p_name);
	return true;
}

bool Action::add_path(Path *p_path) {
	if (handle != XR_NULL_HANDLE) {
		return false;
	}

	// check if we already have it
	for (uint64_t i = 0; i < subaction_paths.size(); i++) {
		if (subaction_paths[i] == p_path) {
			// we already have it...
			return true;
		}
	}

	// add it
	subaction_paths.push_back(p_path);
	return true;
}

XrAction Action::get_action() const {
	return handle;
}

bool Action::get_as_bool(OpenXRApi *p_api, Path *p_path) {
	if (handle == XR_NULL_HANDLE || p_path == NULL) {
		// not initialised
		return false;
	} else if (type != XR_ACTION_TYPE_BOOLEAN_INPUT) {
		// wrong type
		return false;
	} else {
		XrActionStateGetInfo getInfo = {
			.type = XR_TYPE_ACTION_STATE_GET_INFO,
			.next = NULL,
			.action = handle,
			.subactionPath = p_path->get_path()
		};

		if (getInfo.subactionPath == XR_NULL_PATH) {
			// path not set
			return false;
		}

		XrActionStateBoolean resultState;
		resultState.type = XR_TYPE_ACTION_STATE_BOOLEAN,
		resultState.next = NULL;
		XrResult result = xrGetActionStateBoolean(p_api->session, &getInfo, &resultState);
		if (!p_api->xr_result(result, "failed to get boolean value")) {
			resultState.isActive = false;
		}

		// we should do something with resultState.isActive

		return resultState.currentState;
	}
}

float Action::get_as_float(OpenXRApi *p_api, Path *p_path) {
	if (handle == XR_NULL_HANDLE || p_path == NULL) {
		// not initialised
		return 0.0;
	} else if (type != XR_ACTION_TYPE_FLOAT_INPUT) {
		// wrong type
		return 0.0;
	} else {
		XrActionStateGetInfo getInfo = {
			.type = XR_TYPE_ACTION_STATE_GET_INFO,
			.next = NULL,
			.action = handle,
			.subactionPath = p_path->get_path()
		};

		if (getInfo.subactionPath == XR_NULL_PATH) {
			// path not set
			return 0.0;
		}

		XrActionStateFloat resultState;
		resultState.type = XR_TYPE_ACTION_STATE_FLOAT,
		resultState.next = NULL;
		XrResult result = xrGetActionStateFloat(p_api->session, &getInfo, &resultState);
		if (!p_api->xr_result(result, "failed to get float value")) {
			resultState.isActive = false;
		}

		// we should do something with resultState.isActive

		return resultState.currentState;
	}
}

Vector2 Action::get_as_vector(OpenXRApi *p_api, Path *p_path) {
	if (handle == XR_NULL_HANDLE || p_path == NULL) {
		// not initialised
		return Vector2();
	} else if (type != XR_ACTION_TYPE_VECTOR2F_INPUT) {
		// wrong type
		return Vector2();
	} else {
		XrActionStateGetInfo getInfo = {
			.type = XR_TYPE_ACTION_STATE_GET_INFO,
			.next = NULL,
			.action = handle,
			.subactionPath = p_path->get_path()
		};

		if (getInfo.subactionPath == XR_NULL_PATH) {
			// path not set
			return Vector2();
		}

		XrActionStateVector2f resultState;
		resultState.type = XR_TYPE_ACTION_STATE_VECTOR2F,
		resultState.next = NULL;
		XrResult result = xrGetActionStateVector2f(p_api->session, &getInfo, &resultState);
		if (!p_api->xr_result(result, "failed to get vector value")) {
			resultState.isActive = false;
		}

		// we should do something with resultState.isActive

		return Vector2(resultState.currentState.x, resultState.currentState.y);
	}
}

bool Action::is_pose_active(OpenXRApi *p_api, Path *p_path) {
	if (handle == XR_NULL_HANDLE || p_path == NULL) {
		// not initialised
		Godot::print("Pose not initialised");
		return false;
	} else if (type != XR_ACTION_TYPE_POSE_INPUT) {
		// wrong type
		Godot::print("Not a pose type");
		return false;
	} else {
		XrActionStateGetInfo getInfo = {
			.type = XR_TYPE_ACTION_STATE_GET_INFO,
			.next = NULL,
			.action = handle,
			.subactionPath = p_path->get_path()
		};

		if (getInfo.subactionPath == XR_NULL_PATH) {
			// path not set
			Godot::print("Path not registered");
			return false;
		}

		XrActionStatePose resultState;
		resultState.type = XR_TYPE_ACTION_STATE_POSE,
		resultState.next = NULL;
		XrResult result = xrGetActionStatePose(p_api->session, &getInfo, &resultState);
		if (!p_api->xr_result(result, "failed to get pose state")) {
			resultState.isActive = false;
		}

		/*
		if (resultState.isActive) {
			Godot::print("{0}/{1} is active", name, p_path->get_name());
		} else {
			Godot::print("{0}/{1} is inactive", name, p_path->get_name());
		}
		*/

		return resultState.isActive;
	}
}

Transform Action::get_as_pose(OpenXRApi *p_api, Path *p_path, float p_world_scale) {
	if (handle == XR_NULL_HANDLE || p_path == NULL || spaces.size() != subaction_paths.size()) {
		// not initialised or setup fully
		return Transform();
	} else if (type != XR_ACTION_TYPE_POSE_INPUT) {
		// wrong type
		return Transform();
	} else {
		// find out the index for our path, note, thanks to register_path we can use our pointers here
		uint64_t index = 0xFFFFFFFF;
		for (uint64_t i = 0; i < subaction_paths.size() && index == 0xFFFFFFFF; i++) {
			if (subaction_paths[i] == p_path) {
				index = i;
			}
		}

		if (index == 0xFFFFFFFF) {
			// couldn't find it?
			return Transform();
		}

		XrSpaceLocation location;

		location.type = XR_TYPE_SPACE_LOCATION;
		location.next = NULL;

		XrResult result = xrLocateSpace(spaces[index], p_api->play_space, p_api->frameState.predictedDisplayTime, &location);
		if (!p_api->xr_result(result, "failed to locate space!")) {
			return Transform();
		}

		bool spaceLocationValid =
				//(location.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT) != 0 &&
				(location.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT) != 0;

		if (!spaceLocationValid) {
			Godot::print_warning(String("OpenXR Space location not valid for hand ") + String::num_int64(index), __FUNCTION__, __FILE__, __LINE__);
			return Transform();
		} else {
			// TODOneed to get worldscale
			return p_api->transform_from_pose(location.pose, p_world_scale);
		}
	}
}

bool Action::create(OpenXRApi *p_api, XrActionSet p_action_set) {
	if (p_action_set == XR_NULL_HANDLE) {
		Godot::print_error("Can't create action when action set was not created", __FUNCTION__, __FILE__, __LINE__);
		return false;
	}

	if (handle == XR_NULL_HANDLE) {
		uint32_t num_of_paths = 0;
		XrPath paths[16]; // We really don't need more then 2 or 3 max

		for (uint64_t i = 0; i < subaction_paths.size() && i < 16; i++) {
			Path *path = subaction_paths[i];
			paths[num_of_paths] = path->get_path();
			if (paths[num_of_paths] != XR_NULL_PATH) {
				num_of_paths++;
			}
		}

		XrActionCreateInfo actionInfo = {
			.type = XR_TYPE_ACTION_CREATE_INFO,
			.next = NULL,
			.actionType = type,
			.countSubactionPaths = num_of_paths,
			.subactionPaths = paths
		};

		strcpy(actionInfo.actionName, name);
		strcpy(actionInfo.localizedActionName, localised_name);

#ifdef DEBUG
		Godot::print("Created action {0} {1} {2}", actionInfo.actionName, actionInfo.localizedActionName, actionInfo.countSubactionPaths);
#endif

		XrResult result = xrCreateAction(p_action_set, &actionInfo, &handle);
		if (!p_api->xr_result(result, "failed to create {0} action", name)) {
			return false;
		}
	}

	if (type == XR_ACTION_TYPE_POSE_INPUT) {
		// if this is a pose we need to define spaces

		XrActionSpaceCreateInfo actionSpaceInfo = {
			.type = XR_TYPE_ACTION_SPACE_CREATE_INFO,
			.next = NULL,
			.action = handle,
			// .subactionPath = handPaths[0],
			// seriously MS, you can't support this either?!?!
			// .poseInActionSpace.orientation.w = 1.f,
			.poseInActionSpace = {
					.orientation = {
							.w = 1.f } },
		};

		for (int s = spaces.size(); s < subaction_paths.size(); s++) {
			XrSpace space;

			actionSpaceInfo.subactionPath = subaction_paths[s]->get_path();

			XrResult result = xrCreateActionSpace(p_api->session, &actionSpaceInfo, &space);
			if (!p_api->xr_result(result, "failed to create pose space")) {
				return false;
			}

#ifdef DEBUG
			Godot::print("Created space for {0}/{1} ({2})", name, subaction_paths[s]->get_name(), (uint64_t)space);
#endif

			spaces.push_back(space);
		}
	}

	return true;
}

void Action::destroy() {
#ifdef DEBUG
	Godot::print("Destroying OpenXR objects related to action {0}", name);
#endif

	while (!spaces.empty()) {
		XrSpace space = spaces.back();
		xrDestroySpace(space);
		spaces.pop_back();
	}

	if (handle != XR_NULL_HANDLE) {
		xrDestroyAction(handle);
		handle = XR_NULL_HANDLE;
	}
}

void Action::print() {
	Godot::print(" - Action {0} - {1} ({2})", name, localised_name, (uint64_t)handle);

	for (uint64_t i = 0; i < subaction_paths.size(); i++) {
		Godot::print("    - Path {0} ({1})", subaction_paths[i]->get_name(), (uint64_t)subaction_paths[i]->get_path());
	}
}
