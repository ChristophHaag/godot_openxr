////////////////////////////////////////////////////////////////////////////////////////////////
// Class for OpenXR Interaction Profiles
// Note, included through OpenXRApi.h
//
// Interaction profiles bind actions to the input on a given controller type.
// They are suggested bindings meaning that if a controller of this type is found
// OpenXR will use these bindings by default.
//
// However the user has the ability to override these bindings in the OpenXR runtime
// or supply their own bindings for controllers that we didn't supply bindings for.

#ifndef OPENXR_INTERACTIONPROFILE_H
#define OPENXR_INTERACTIONPROFILE_H

class InteractionProfile {
private:
	Path name;

	struct Binding {
		Action *action;
		Path *path;
	};

	std::vector<Binding> bindings;

public:
	InteractionProfile(const char *p_name);
	~InteractionProfile();

	char *get_name() const;
	void set_name(const char *p_name);

	void add_binding(Action *p_action, Path *p_path);
	bool update(OpenXRApi *p_api);
};

#endif /* !OPENXR_INTERACTIONPROFILE_H */
