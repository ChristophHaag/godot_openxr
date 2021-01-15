////////////////////////////////////////////////////////////////////////////////////////////////
// Class for OpenXR Interaction Profiles

#include "openxr/OpenXRApi.h"

using namespace godot;

InteractionProfile::InteractionProfile(const char *p_name) {
	name.set_name(p_name);
}

InteractionProfile::~InteractionProfile() {
	// nothing yet to do here
}

char *InteractionProfile::get_name() const {
	return name.get_name();
}

void InteractionProfile::set_name(const char *p_name) {
	name.set_name(p_name);
}

void InteractionProfile::add_binding(Action *p_action, Path *p_path) {
	Binding binding = {
		.action = p_action,
		.path = p_path
	};
	bindings.push_back(binding);
}

bool InteractionProfile::update(OpenXRApi *p_api) {
	uint32_t num_bindings = bindings.size();

	// make sure we create this, if it has already been created it'll just skip
	name.create(p_api);

	// allocate a buffer for our bindings that we can send to OpenXR
	XrActionSuggestedBinding *xr_bindings = (XrActionSuggestedBinding *)malloc(sizeof(XrActionSuggestedBinding) * num_bindings);
	if (xr_bindings == NULL) {
		Godot::print_error("OpenXR couldn't allocate memory for bindings", __FUNCTION__, __FILE__, __LINE__);
		return false;
	}

	// copy our bindings data
	for (uint64_t b = 0; b < num_bindings; b++) {
		xr_bindings[b].action = bindings[b].action->get_action();
		xr_bindings[b].binding = bindings[b].path->get_path();

		Godot::print("Binding {0} - {1} => {2}", b, (int64_t)xr_bindings[b].action, (int64_t)xr_bindings[b].binding);
	}

	// update our profile
	const XrInteractionProfileSuggestedBinding suggestedBindings = {
		.type = XR_TYPE_INTERACTION_PROFILE_SUGGESTED_BINDING,
		.next = NULL,
		.interactionProfile = name.get_path(),
		.countSuggestedBindings = num_bindings,
		.suggestedBindings = xr_bindings
	};

	if (suggestedBindings.interactionProfile == XR_NULL_PATH) {
		Godot::print("No path set for interaction profile {0}", name.get_name());
		free(xr_bindings);
		return false;
	}

	XrResult result = xrSuggestInteractionProfileBindings(p_api->instance, &suggestedBindings);
	if (!p_api->xr_result(result, "failed to suggest bindings for {0}", name.get_name())) {
		free(xr_bindings);
		return false;
	}

	free(xr_bindings);

	return true;
}
