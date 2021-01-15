////////////////////////////////////////////////////////////////////////////////////////////////
// Class for OpenXR Action Sets

#include "openxr/OpenXRApi.h"

using namespace godot;

ActionSet::ActionSet(const char *p_name, const char *p_localised_name, int p_priority) {
	set_name(p_name);
	set_localised_name(p_localised_name);
	priority = p_priority;
}

ActionSet::~ActionSet() {
	// loop through actions in actions set..
	while (!actions.empty()) {
		Action *action = actions.back();
		delete action;
		actions.pop_back();
	}

	if (handle != XR_NULL_HANDLE) {
		xrDestroyActionSet(handle);
	}
}

bool ActionSet::is_active() const {
	return active;
}

void ActionSet::set_active(bool p_is_active) {
	active = p_is_active;
}

char *ActionSet::get_name() const {
	return (char *)name;
}

bool ActionSet::set_name(const char *p_name) {
	if (handle != XR_NULL_HANDLE) {
		// already created, can't change it anymore
		return false;
	}

	strcpy(name, p_name);
	return true;
}

char *ActionSet::get_localised_name() const {
	return (char *)localised_name;
}

bool ActionSet::set_localised_name(const char *p_name) {
	if (handle != XR_NULL_HANDLE) {
		// already created, can't change it anymore
		return false;
	}

	strcpy(localised_name, p_name);
	return true;
}

uint32_t ActionSet::get_priority() const {
	return priority;
}

bool ActionSet::set_priority(const uint32_t p_priority) {
	if (handle != XR_NULL_HANDLE) {
		// already created, can't change it anymore
		return false;
	}

	priority = p_priority;
	return true;
}

Action *ActionSet::add_action(XrActionType p_type, const char *p_name, const char *p_localised_name) {
	if (handle != XR_NULL_HANDLE) {
		// already created, can't change it anymore
		return NULL;
	}

	// check if we already have this action
	for (uint64_t i = 0; i < actions.size(); i++) {
		if (strcmp(actions[i]->get_name(), p_name) == 0) {
			return actions[i];
		}
	}

	Action *new_action = new Action(p_type, p_name, p_localised_name);
	actions.push_back(new_action);
	return new_action;
}

Action *ActionSet::get_action(const char *p_name) {
	// find this action
	for (uint64_t i = 0; i < actions.size(); i++) {
		if (strcmp(actions[i]->get_name(), p_name) == 0) {
			return actions[i];
		}
	}

	return NULL;
}

XrActionSet ActionSet::get_action_set() {
	return handle;
}

bool ActionSet::create(OpenXRApi *p_api) {
	if (handle == XR_NULL_HANDLE) {
		// create our action set...
		XrActionSetCreateInfo actionSetInfo = {
			.type = XR_TYPE_ACTION_SET_CREATE_INFO,
			.next = NULL,
			.priority = priority
		};
		strcpy(actionSetInfo.actionSetName, name);
		strcpy(actionSetInfo.localizedActionSetName, localised_name);

		Godot::print("Creating action set {0} - {1} {2}", name, localised_name, priority);

		XrResult result = xrCreateActionSet(p_api->instance, &actionSetInfo, &handle);
		if (!p_api->xr_result(result, "failed to create actionset {0}", name)) {
			return false;
		}

		for (uint64_t i = 0; i < actions.size(); i++) {
			actions[i]->create(p_api, handle);
		}

		Godot::print("Done");
	}
	return true;
}

bool ActionSet::attach(OpenXRApi *p_api) {
	if (handle == XR_NULL_HANDLE) {
		return false;
	}

	// So according to the docs, once we attach our action set to our session it becomes read only..
	// https://www.khronos.org/registry/OpenXR/specs/1.0/man/html/xrAttachSessionActionSets.html
	XrSessionActionSetsAttachInfo attachInfo = {
		.type = XR_TYPE_SESSION_ACTION_SETS_ATTACH_INFO,
		.next = NULL,
		.countActionSets = 1,
		.actionSets = &handle
	};

	XrResult result = xrAttachSessionActionSets(p_api->session, &attachInfo);
	if (!p_api->xr_result(result, "failed to attach action set")) {
		return false;
	}

	return true;
}

void ActionSet::destroy() {
	// loop through the actions and destroy them, don't delete them
	for (uint64_t i = 0; i < actions.size(); i++) {
		actions[i]->destroy();
	}

	if (handle != XR_NULL_HANDLE) {
		xrDestroyActionSet(handle);
		handle = XR_NULL_HANDLE;
	}
}

void ActionSet::print() {
	Godot::print("ActionSet {0} - {1} ({2})", name, localised_name, (uint64_t)handle);

	for (uint64_t i = 0; i < actions.size(); i++) {
		actions[i]->print();
	}
}