extends Spatial


onready var openxr_ext = preload("res://addons/godot-openxr/godot_openxr.gdns").new()
# Called when the node enters the scene tree for the first time.
func _ready():
	print("hand tracking loaded")
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


func finger(cid, skeleton_finger, openxr_finger):
	var finger_transform = openxr_ext.get_hand_tracking(cid, openxr_finger)
	
	if not finger_transform:
		#print("No finger location for " + str(cid))
		return
	
	var skel = self.get_node("RootNode/Skeleton")

	var hand_trans = self.get_node("../").transform
	if not hand_trans:
		#print("No hand transform")
		#return
		pass
		
	#var inv_hand = hand_trans.affine_inverse()
	#if not inv_hand:
	#	print("Could not invert hand transform")
	
	#finger_transform = finger_transform
	skel.set_bone_custom_pose(skeleton_finger, finger_transform)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	var cid = -1
	if self.name == "openxr_left_hand":
		cid = 1
	elif self.name == "openxr_right_hand":
		cid = 2
	else:
		print("Script must be attached to openxr_left_hand or openxr_right_hand")
		print("Currently attached to " + self.name)
		
	finger(cid, 9, XR_JOINT_INDEX.XR_HAND_JOINT_INDEX_TIP_EXT)
	finger(cid, 14, XR_JOINT_INDEX.XR_HAND_JOINT_MIDDLE_TIP_EXT)
	finger(cid, 19, XR_JOINT_INDEX.XR_HAND_JOINT_RING_TIP_EXT)
	finger(cid, 24, XR_JOINT_INDEX.XR_HAND_JOINT_LITTLE_TIP_EXT)
	
