////////////////////////////////////////////////////////////////////////////////////////////////
// Class for OpenXR Actions
// Note, included through OpenXRApi.h
//
// Our action object encapsulates all data and logic related to interaction with actions
// in OpenXR.

#ifndef OPENXR_ACTION_H
#define OPENXR_ACTION_H

class Action {
private:
	XrActionType type = XR_ACTION_TYPE_BOOLEAN_INPUT;
	char name[XR_MAX_ACTION_NAME_SIZE];
	char localised_name[XR_MAX_LOCALIZED_ACTION_NAME_SIZE];

	std::vector<Path *> subaction_paths;

	// Populated once OpenXR is initialised
	XrAction handle = XR_NULL_HANDLE;
	std::vector<XrSpace> spaces;

public:
	Action(XrActionType p_type, const char *p_name, const char *p_localised_name);
	~Action();

	XrActionType get_type() const;
	void set_type(XrActionType p_type);

	char *get_name() const;
	bool set_name(const char *p_name);

	char *get_localised_name() const;
	bool set_localised_name(const char *p_name);

	bool add_path(Path *p_path);

	XrAction get_action() const;
	bool get_as_bool(OpenXRApi *p_api, Path *p_path);
	float get_as_float(OpenXRApi *p_api, Path *p_path);
	godot::Vector2 get_as_vector(OpenXRApi *p_api, Path *p_path);
	bool is_pose_active(OpenXRApi *p_api, Path *p_path);
	godot::Transform get_as_pose(OpenXRApi *p_api, Path *p_path, float p_world_scale);

	bool create(OpenXRApi *p_api, XrActionSet p_action_set);
	void destroy();

	void print(); // for debugging output our action map
};

#endif /* !OPENXR_ACTION_H */
