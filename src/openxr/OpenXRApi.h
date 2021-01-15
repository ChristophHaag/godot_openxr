////////////////////////////////////////////////////////////////////////////////////////////////
// Wrapper class for interacting with OpenXR
//
// Initial implementation thanks Christoph Haag

#ifndef OPENXR_API_H
#define OPENXR_API_H

#include <Godot.hpp>
#include <Transform.hpp>
#include <Vector2.hpp>

#ifdef WIN32
#include <windows.h>
#endif

#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include "xrmath.h"
#include <openxr/openxr.h>

#define XR_MND_BALL_ON_STICK_EXTENSION_NAME "XR_MNDX_ball_on_a_stick_controller"

#ifdef WIN32
#define XR_USE_PLATFORM_WIN32
#else
#define XR_USE_PLATFORM_XLIB
#endif
#define XR_USE_GRAPHICS_API_OPENGL

#ifdef WIN32
#include <glad/glad.h>
#else
// linux
#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>

#include <GL/glx.h>
#include <X11/Xlib.h>
#endif

// #include <gdnative/gdnative.h>
#include <openxr/openxr.h>
#include <openxr/openxr_platform.h>

// forward declare this
class OpenXRApi;

#include "openxr/actions/path.h"

#include "openxr/actions/action.h"
#include "openxr/actions/actionset.h"
#include "openxr/actions/interactionprofile.h"

class HandTracker {
public:
	bool is_initialised;

	XrHandTrackerEXT hand_tracker;
	XrHandJointLocationEXT joint_locations[XR_HAND_JOINT_COUNT_EXT];
	XrHandJointVelocityEXT joint_velocities[XR_HAND_JOINT_COUNT_EXT];

	XrHandJointVelocitiesEXT velocities;
	XrHandJointLocationsEXT locations;
};

class OpenXRApi {
	friend class Path;
	friend class Action;
	friend class ActionSet;
	friend class InteractionProfile;

public:
	// These are hardcoded for now and meant for our backwards compatibility layer
	// If not configured in our action sets they will be defunct

	// Inputs we track, this was Hands but renamed it in case we want to support additional tracked input devices
	enum UserInputs {
		USER_HAND_LEFT, // This should always map to ARVRController 1
		USER_HAND_RIGHT, // This should always map to ARVRController 2
		USER_INPUT_MAX
	};

	// Paths related to our inputs
	const char *UserInputPaths[USER_INPUT_MAX] = {
		"/user/hand/left",
		"/user/hand/right",
	};

	struct InputMap {
		Path *path;
		godot_int godot_controller;
		XrPath active_profile; // note, this can be a profile added in the OpenXR runtime unknown to our default mappings
	};

	// Default actions we support so we can mimic our old ARVRController handling
	enum Actions {
		// Poses
		ACTION_AIM_POSE,

		// Analog
		ACTION_FRONT_TRIGGER, // front trigger (Axis 2)
		ACTION_SIDE_TRIGGER, // side trigger/grip (Axis 4)
		ACTION_JOYSTICK, // primary joystick (Axis 0/1)

		// Buttons
		ACTION_AX_BUTTON, // A/X button (button 7)
		ACTION_BYM_BUTTON, // B/Y/Menu button (button 1)
		ACTION_FRONT_BUTTON, // front trigger as button (button 15)
		ACTION_SIDE_BUTTON, // side trigger/grip as button (button 2)

		ACTION_MAX
	};

	const char *ActionNames[ACTION_MAX] = {
		"aim_pose",
		"front_trigger",
		"side_trigger",
		"joystick",
		"ax_buttons",
		"bym_button",
		"front_button",
		"side_button",
	};

	const XrActionType ActionTypes[ACTION_MAX] = {
		XR_ACTION_TYPE_POSE_INPUT,
		XR_ACTION_TYPE_FLOAT_INPUT,
		XR_ACTION_TYPE_FLOAT_INPUT,
		XR_ACTION_TYPE_VECTOR2F_INPUT,
		XR_ACTION_TYPE_BOOLEAN_INPUT,
		XR_ACTION_TYPE_BOOLEAN_INPUT,
		XR_ACTION_TYPE_BOOLEAN_INPUT,
		XR_ACTION_TYPE_BOOLEAN_INPUT,
	};

private:
	static OpenXRApi *singleton;
	bool initialised = false;
	int use_count = 1;

	// extensions
	bool hand_tracking_ext = false;
	bool monado_stick_on_ball_ext = false;
	bool hand_tracking_supported = false;

	XrInstance instance = XR_NULL_HANDLE;
	XrSystemId systemId;
	XrSession session = XR_NULL_HANDLE;

	/* XR_REFERENCE_SPACE_TYPE_LOCAL: head pose on startup/recenter is coordinate system origin.
	 * XR_REFERENCE_SPACE_TYPE_STAGE: origin is externally calibrated to be on play space floor. */
	XrReferenceSpaceType play_space_type = XR_REFERENCE_SPACE_TYPE_STAGE;

	XrSpace play_space = XR_NULL_HANDLE;
	XrSpace view_space = XR_NULL_HANDLE;
#ifdef WIN32
	XrGraphicsBindingOpenGLWin32KHR graphics_binding_gl;
#else
	XrGraphicsBindingOpenGLXlibKHR graphics_binding_gl;
#endif
	XrSwapchainImageOpenGLKHR **images = NULL;
	XrSwapchain *swapchains = NULL;
	uint32_t view_count;
	XrViewConfigurationView *configuration_views = NULL;
	// GLuint** framebuffers;
	// GLuint depthbuffer;

	XrCompositionLayerProjection *projectionLayer = NULL;
	XrFrameState frameState = {};
	bool running;

	XrSessionState state = XR_SESSION_STATE_UNKNOWN;
	bool view_pose_valid = false;

	uint32_t *buffer_index = NULL;

	XrView *views = NULL;
	XrCompositionLayerProjectionView *projection_views = NULL;

	Action *actions[ACTION_MAX];
	InputMap inputmaps[USER_INPUT_MAX];
	HandTracker hand_trackers[2]; // Fixed for left and right hand

	// config
	XrFormFactor form_factor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;
	std::vector<Path *> paths;
	std::vector<ActionSet *> action_sets;
	std::vector<InteractionProfile *> interaction_profiles;

	template <class... Args>
	bool xr_result(XrResult result, const char *format, Args... values) {
		if (XR_SUCCEEDED(result))
			return true;

		char resultString[XR_MAX_RESULT_STRING_SIZE];
		xrResultToString(instance, result, resultString);

		godot::Godot::print_error(
				godot::String("OpenXR ") + godot::String(format).format(godot::Array::make(values...)) + godot::String(" [") + godot::String(resultString) + godot::String("]"),
				__FUNCTION__,
				__FILE__,
				__LINE__);

		return false;
	};

	bool isExtensionSupported(const char *extensionName, XrExtensionProperties *instanceExtensionProperties, uint32_t instanceExtensionCount);
	bool isViewConfigSupported(XrViewConfigurationType type, XrSystemId systemId);
	bool isReferenceSpaceSupported(XrReferenceSpaceType type);
	bool initialiseInstance();
	bool initialiseExtensions();
	bool initialiseSession();
	bool initialiseSpaces();
	bool initialiseSwapChains();
	bool initialiseActionSets();
	bool initialiseHandTracking();
	bool check_graphics_requirements_gl(XrSystemId system_id);
	XrResult acquire_image(int eye);
	void update_controllers();
	void update_handtracking();
	void transform_from_matrix(godot_transform *p_dest, XrMatrix4x4f *matrix, float p_world_scale);

public:
	static OpenXRApi *openxr_get_api();
	static void openxr_release_api();

	OpenXRApi();
	~OpenXRApi();

	bool is_initialised();
	bool initialize();
	void uninitialize();

	const HandTracker *get_hand_tracker(int p_hand) { return &hand_trackers[p_hand]; };

	// config
	XrFormFactor get_form_factor() const;
	void set_form_factor(XrFormFactor p_form_factor);

	// action system
	Path *register_path(const char *p_name);
	Path *get_path(const char *p_name);
	ActionSet *get_action_set(const char *p_name);
	ActionSet *add_action_set(const char *p_name, const char *p_localised_name, int p_priority = 0);
	Action *get_action(const char *p_name);
	void remove_action_set(ActionSet *p_action_set);
	bool parse_action_sets(const char *p_json);

	InteractionProfile *add_interaction_profile(const char *p_name);
	InteractionProfile *get_interaction_profile(const char *p_name);
	void remove_interaction_profile(InteractionProfile *p_profile);
	bool parse_interaction_profiles(const char *p_json);

	/* render_openxr() should be called once per eye.
	 *
	 * If has_external_texture_support it assumes godot has finished rendering into
	 * the external texture and ignores texid. If false, it copies content from
	 * texid to the OpenXR swapchain. Then the image is released.
	 * If eye == 1, ends the frame.
	 */
	void render_openxr(int eye, uint32_t texid, bool has_external_texture_support);

	// fill_projection_matrix() should be called after process_openxr()
	void fill_projection_matrix(int eye, godot_real p_z_near, godot_real p_z_far, godot_real *p_projection);

	// recommended_rendertarget_size() returns required size of our image buffers
	void recommended_rendertarget_size(uint32_t *width, uint32_t *height);

	// get_view_transform() should be called after fill_projection_matrix()
	bool get_view_transform(int eye, float world_scale, godot_transform *transform_for_eye);

	// get_head_center() can be called at any time after init
	bool get_head_center(float world_scale, godot_transform *transform);

	// get_external_texture_for_eye() acquires images and sets has_support to true
	int get_external_texture_for_eye(int eye, bool *has_support);

	// process_openxr() should be called FIRST in the frame loop
	void process_openxr();

	// helper method to get a transform from an openxr pose
	godot::Transform transform_from_pose(const XrPosef &p_pose, float p_world_scale);
};

#endif /* !OPENXR_API_H */
