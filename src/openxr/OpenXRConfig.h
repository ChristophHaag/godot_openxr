/////////////////////////////////////////////////////////////////////////////////////
// Our OpenXR config GDNative object

#ifndef OPENXR_CONFIG_H
#define OPENXR_CONFIG_H

#include "OpenXRApi.h"
#include <Node.hpp>

namespace godot {

class OpenXRConfig : public Node {
	GODOT_CLASS(OpenXRConfig, Node)

private:
	OpenXRApi *openxr_api;

public:
	static void _register_methods();

	void _init();

	OpenXRConfig();
	~OpenXRConfig();

	Array _get_property_list();
	Variant _get(String p_name);
	bool _set(String p_name, Variant p_value);

	int get_form_factor() const;
	void set_form_factor(int p_form_factor);
};

} // namespace godot

#endif // !OPENXR_CONFIG_H
