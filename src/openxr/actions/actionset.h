////////////////////////////////////////////////////////////////////////////////////////////////
// Class for OpenXR Action sets
// Note, included through OpenXRApi.h
//
// ActionSets combine a collection of actions that can be triggered or interacted with
// if the actionset is active.
// We can then use interaction profiles to bind these actions to inputs on various controllers.

#ifndef OPENXR_ACTIONSET_H
#define OPENXR_ACTIONSET_H

class ActionSet {
private:
	bool active = true;
	char name[XR_MAX_ACTION_SET_NAME_SIZE];
	char localised_name[XR_MAX_LOCALIZED_ACTION_SET_NAME_SIZE];
	uint32_t priority = 0;

	// vector with actions
	std::vector<Action *> actions;

	// Populated once OpenXR is initialised
	XrActionSet handle = XR_NULL_HANDLE;

public:
	ActionSet(const char *p_name, const char *p_localised_name, int p_priority = 0);
	~ActionSet();

	bool is_active() const;
	void set_active(bool p_is_active);

	char *get_name() const;
	bool set_name(const char *p_name);

	char *get_localised_name() const;
	bool set_localised_name(const char *p_name);

	uint32_t get_priority() const;
	bool set_priority(const uint32_t p_priority);

	Action *add_action(XrActionType p_type, const char *p_name, const char *p_localised_name);
	Action *get_action(const char *p_name);

	XrActionSet get_action_set();
	bool create(OpenXRApi *p_api); // Tell OpenXR all about our action set
	bool attach(OpenXRApi *p_api); // Do this after creating all the interaction profiles, it will lock the whole thing
	void destroy(); // Clear this action set from OpenXR.

	void print(); // for debugging output our action map
};

#endif /* !OPENXR_ACTIONSET_H */
