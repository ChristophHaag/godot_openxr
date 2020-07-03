extends ARVRController


onready var openxr_ext = preload("res://addons/godot-openxr/godot_openxr.gdns").new()
# Called when the node enters the scene tree for the first time.
func _ready():
	pass

# see https://www.khronos.org/registry/OpenXR/specs/1.0/html/xrspec.html#XR_EXT_hand_tracking
enum XR_JOINT_INDEX {
	XR_HAND_JOINT_PALM_EXT = 0,
	XR_HAND_JOINT_WRIST_EXT = 1,
	XR_HAND_JOINT_THUMB_METACARPAL_EXT = 2,
	XR_HAND_JOINT_THUMB_PROXIMAL_EXT = 3,
	XR_HAND_JOINT_THUMB_DISTAL_EXT = 4,
	XR_HAND_JOINT_THUMB_TIP_EXT = 5,
	XR_HAND_JOINT_INDEX_METACARPAL_EXT = 6,
	XR_HAND_JOINT_INDEX_PROXIMAL_EXT = 7,
	XR_HAND_JOINT_INDEX_INTERMEDIATE_EXT = 8,
	XR_HAND_JOINT_INDEX_DISTAL_EXT = 9,
	XR_HAND_JOINT_INDEX_TIP_EXT = 10,
	XR_HAND_JOINT_MIDDLE_METACARPAL_EXT = 11,
	XR_HAND_JOINT_MIDDLE_PROXIMAL_EXT = 12,
	XR_HAND_JOINT_MIDDLE_INTERMEDIATE_EXT = 13,
	XR_HAND_JOINT_MIDDLE_DISTAL_EXT = 14,
	XR_HAND_JOINT_MIDDLE_TIP_EXT = 15,
	XR_HAND_JOINT_RING_METACARPAL_EXT = 16,
	XR_HAND_JOINT_RING_PROXIMAL_EXT = 17,
	XR_HAND_JOINT_RING_INTERMEDIATE_EXT = 18,
	XR_HAND_JOINT_RING_DISTAL_EXT = 19,
	XR_HAND_JOINT_RING_TIP_EXT = 20,
	XR_HAND_JOINT_LITTLE_METACARPAL_EXT = 21,
	XR_HAND_JOINT_LITTLE_PROXIMAL_EXT = 22,
	XR_HAND_JOINT_LITTLE_INTERMEDIATE_EXT = 23,
	XR_HAND_JOINT_LITTLE_DISTAL_EXT = 24,
	XR_HAND_JOINT_LITTLE_TIP_EXT = 25,
	XR_HAND_JOINT_MAX_ENUM_EXT = 0x7FFFFFFF
}
	
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	var cid = get("controller_id")
	
	var tip_location = openxr_ext.get_hand_tracking(cid, XR_JOINT_INDEX.XR_HAND_JOINT_INDEX_TIP_EXT)
	
	if not tip_location:
		return
	
	print("Tip location " + str(tip_location.x) + str(tip_location.y) + str(tip_location.z))
