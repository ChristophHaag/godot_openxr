////////////////////////////////////////////////////////////////////////////////////////////////
// GodotCalls is a utility for storing shared data required to call back into
// Godot

#include "GodotCalls.h"

const godot_gdnative_core_api_struct *api = NULL;
const godot_gdnative_ext_arvr_api_struct *arvr_api = NULL;
const godot_gdnative_ext_nativescript_api_struct *nativescript_api = NULL;

void GDN_EXPORT
godot_openxr_gdnative_init(godot_gdnative_init_options *p_options)
{
	// get our main API struct
	api = p_options->api_struct;

	// now find our arvr extension
	for (int i = 0; i < api->num_extensions; i++) {
		// todo: add version checks
		switch (api->extensions[i]->type) {
		case GDNATIVE_EXT_ARVR: {
			if (api->extensions[i]->version.major > 1 ||
			    (api->extensions[i]->version.major == 1 &&
			     api->extensions[i]->version.minor >= 1)) {
				arvr_api =
				    (godot_gdnative_ext_arvr_api_struct *)
				        api->extensions[i];
			} else {
				printf(
				    "ARVR API version %i.%i isn't supported, "
				    "need version 1.1 or "
				    "higher\n",
				    api->extensions[i]->version.major,
				    api->extensions[i]->version.minor);
			}
		}; break;
		case GDNATIVE_EXT_NATIVESCRIPT: {
			if (api->extensions[i]->version.major > 1 ||
			    (api->extensions[i]->version.major == 1 &&
			     api->extensions[i]->version.minor >= 0)) {
				nativescript_api =
				    (godot_gdnative_ext_nativescript_api_struct
				         *)api->extensions[i];
			} else {
				printf(
				    "Native script API version %i.%i isn't "
				    "supported, need version 1.0 "
				    "or higher\n",
				    api->extensions[i]->version.major,
				    api->extensions[i]->version.minor);
			}
		}; break;
		default: break;
		};
	};
}

void GDN_EXPORT
godot_gdnative_terminate(godot_gdnative_terminate_options *p_options)
{
	api = NULL;
	nativescript_api = NULL;
	arvr_api = NULL;
}


// TODO: make this part of arvr api
extern "C" {
void *
openxr_ext_constructor(godot_object *p_instance, void *p_method_data);
void
openxr_ext_destructor(godot_object *p_instance,
                      void *p_method_data,
                      void *p_user_data);
godot_variant
openxr_ext_get_hand_tracking(godot_object *p_instance,
                             void *p_method_data,
                             void *p_user_data,
                             int p_num_args,
                             godot_variant **p_args);

char class_name[] = "OpenXRExt";
void GDN_EXPORT
godot_openxr_nativescript_init(void *p_handle)
{
	godot_instance_create_func create = {NULL, NULL, NULL};
	create.create_func = &openxr_ext_constructor;

	godot_instance_destroy_func destroy = {NULL, NULL, NULL};
	destroy.destroy_func = &openxr_ext_destructor;

	nativescript_api->godot_nativescript_register_class(
	    p_handle, class_name, "Reference", create, destroy);

	godot_instance_method get_hand_tracking = {NULL, NULL, NULL};
	get_hand_tracking.method = &openxr_ext_get_hand_tracking;

	godot_method_attributes attributes = {GODOT_METHOD_RPC_MODE_DISABLED};

	nativescript_api->godot_nativescript_register_method(
	    p_handle, class_name, "get_hand_tracking", attributes,
	    get_hand_tracking);
}
}
