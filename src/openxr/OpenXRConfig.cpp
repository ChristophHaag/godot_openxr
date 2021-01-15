/////////////////////////////////////////////////////////////////////////////////////
// Our OpenXR config GDNative object

#include "OpenXRConfig.h"
#include <Dictionary.hpp>
#include <GlobalConstants.hpp>

using namespace godot;

void OpenXRConfig::_register_methods() {
	register_method("get_form_factor", &OpenXRConfig::get_form_factor);
	register_method("set_form_factor", &OpenXRConfig::set_form_factor);
	register_property<OpenXRConfig, int>("form_factor", &OpenXRConfig::set_form_factor, &OpenXRConfig::get_form_factor, 1, GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_ENUM, "Not set,HMD,Hand Held");
}

OpenXRConfig::OpenXRConfig() {
	openxr_api = OpenXRApi::openxr_get_api();
}

OpenXRConfig::~OpenXRConfig() {
	if (openxr_api != NULL) {
		OpenXRApi::openxr_release_api();
	}
}

void OpenXRConfig::_init() {
	// nothing to do here
}

int OpenXRConfig::get_form_factor() const {
	if (openxr_api == NULL) {
		return 0;
	} else {
		return (int)openxr_api->get_form_factor();
	}
}

void OpenXRConfig::set_form_factor(int p_form_factor) {
	if (openxr_api == NULL) {
		Godot::print("OpenXR object wasn't constructed.");
	} else {
		openxr_api->set_form_factor((XrFormFactor)p_form_factor);
	}
}
