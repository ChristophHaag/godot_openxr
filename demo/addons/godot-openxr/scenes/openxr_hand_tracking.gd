extends Spatial


onready var openxr_ext = preload("res://addons/godot-openxr/godot_openxr.gdns").new()

var cid = -1
func hand_suffix():
	if (cid == 1):
		return "_l"
	else:
		return "_r"

# Called when the node enters the scene tree for the first time.
func _ready():
	if self.name == "openxr_left_hand":
		cid = 1
	elif self.name == "openxr_right_hand":
		cid = 2
	else:
		print("Script must be attached to openxr_left_hand or openxr_right_hand")
		print("Currently attached to " + self.name)
	print("hand tracking loaded for " + str(cid))
	var skel = self.get_node("RootNode/Skeleton")
	print("Bones " + str(skel.get_bone_count()))
	
	var suff = hand_suffix()
	var wrist_name_hand = "wrist" + suff
	var wrist_index = skel.find_bone(wrist_name_hand)
	if wrist_index >= 0:
		skel.set_bone_parent(skel.find_bone("finger_thumb_0" + suff), wrist_index)
		skel.set_bone_parent(skel.find_bone("finger_thumb_1" + suff), skel.find_bone("finger_thumb_0" + suff))
		skel.set_bone_parent(skel.find_bone("finger_thumb" + suff + "_end"), skel.find_bone("finger_thumb_2" + suff))

		
		skel.set_bone_parent(skel.find_bone("finger_index_0" + suff), wrist_index)
		skel.set_bone_parent(skel.find_bone("finger_index_1" + suff), skel.find_bone("finger_index_0" + suff))
		skel.set_bone_parent(skel.find_bone("finger_index_2" + suff), skel.find_bone("finger_index_1" + suff))
		skel.set_bone_parent(skel.find_bone("finger_index" + suff + "_end"), skel.find_bone("finger_index_2" + suff))
		
		skel.set_bone_parent(skel.find_bone("finger_middle_0" + suff), wrist_index)
		skel.set_bone_parent(skel.find_bone("finger_middle_1" + suff), skel.find_bone("finger_middle_0" + suff))
		skel.set_bone_parent(skel.find_bone("finger_middle_2" + suff), skel.find_bone("finger_middle_1" + suff))
		skel.set_bone_parent(skel.find_bone("finger_middle" + suff + "_end"), skel.find_bone("finger_middle_2" + suff))
		
		skel.set_bone_parent(skel.find_bone("finger_ring_0" + suff), wrist_index)
		skel.set_bone_parent(skel.find_bone("finger_ring_1" + suff), skel.find_bone("finger_ring_0" + suff))
		skel.set_bone_parent(skel.find_bone("finger_ring_2" + suff), skel.find_bone("finger_ring_1" + suff))
		skel.set_bone_parent(skel.find_bone("finger_ring" + suff + "_end"), skel.find_bone("finger_ring_2" + suff))
		
		
		skel.set_bone_parent(skel.find_bone("finger_little_0" + suff), wrist_index)
		skel.set_bone_parent(skel.find_bone("finger_little_0" + suff), skel.find_bone("finger_little_0" + suff))
		skel.set_bone_parent(skel.find_bone("finger_little_0" + suff), skel.find_bone("finger_little_1" + suff))
		skel.set_bone_parent(skel.find_bone("finger_little" + suff + "_end"), skel.find_bone("finger_little_2" + suff))
		
	else:
		print("No wrist found for " + wrist_name_hand)

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

var mapping = {
	"root": XR_JOINT_INDEX.XR_HAND_JOINT_WRIST_EXT,
	"wrist": XR_JOINT_INDEX.XR_HAND_JOINT_WRIST_EXT,
	
	"finger_thumb_0": XR_JOINT_INDEX.XR_HAND_JOINT_THUMB_PROXIMAL_EXT,
	"finger_thumb_1": XR_JOINT_INDEX.XR_HAND_JOINT_THUMB_DISTAL_EXT,
	"finger_thumb": XR_JOINT_INDEX.XR_HAND_JOINT_THUMB_TIP_EXT,
		
	"finger_index_0": XR_JOINT_INDEX.XR_HAND_JOINT_INDEX_PROXIMAL_EXT,
	"finger_index_1": XR_JOINT_INDEX.XR_HAND_JOINT_INDEX_INTERMEDIATE_EXT,
	"finger_index_2": XR_JOINT_INDEX.XR_HAND_JOINT_INDEX_DISTAL_EXT,
	"finger_index": XR_JOINT_INDEX.XR_HAND_JOINT_INDEX_TIP_EXT,
	
	"finger_middle_0": XR_JOINT_INDEX.XR_HAND_JOINT_MIDDLE_PROXIMAL_EXT,
	"finger_middle_1": XR_JOINT_INDEX.XR_HAND_JOINT_MIDDLE_INTERMEDIATE_EXT,
	"finger_middle_2": XR_JOINT_INDEX.XR_HAND_JOINT_MIDDLE_DISTAL_EXT,
	"finger_middle": XR_JOINT_INDEX.XR_HAND_JOINT_MIDDLE_TIP_EXT,
	
	"finger_ring_0": XR_JOINT_INDEX.XR_HAND_JOINT_RING_PROXIMAL_EXT,
	"finger_ring_1": XR_JOINT_INDEX.XR_HAND_JOINT_RING_INTERMEDIATE_EXT,
	"finger_ring_2": XR_JOINT_INDEX.XR_HAND_JOINT_RING_DISTAL_EXT,
	"finger_ring": XR_JOINT_INDEX.XR_HAND_JOINT_RING_TIP_EXT,
	
	"finger_pinky_0": XR_JOINT_INDEX.XR_HAND_JOINT_LITTLE_PROXIMAL_EXT,
	"finger_pinky_1": XR_JOINT_INDEX.XR_HAND_JOINT_LITTLE_INTERMEDIATE_EXT,
	"finger_pinky_2": XR_JOINT_INDEX.XR_HAND_JOINT_LITTLE_DISTAL_EXT,
	"finger_pinky": XR_JOINT_INDEX.XR_HAND_JOINT_LITTLE_TIP_EXT
}

var root_transform
var wrist_transform

func finger(cid, bone_name, suffix, prev_bone_name, prev_suffix):
	var skel = self.get_node("RootNode/Skeleton")

	var bone_name_hand = bone_name + suffix
	var bone_index = skel.find_bone(bone_name_hand)
	if bone_index < 0:
		print("No bone found for " + bone_name_hand)
	else:
		#print("bone " + bone_name + ": " + str(bone_index))
		pass
	
	var openxr_bone = mapping[bone_name]
	var finger_transform = openxr_ext.get_hand_tracking(cid, openxr_bone)

	if not finger_transform:
		#print("No finger location for " + str(cid) + ", " + bone_name)
		return

	if bone_name == "root":
		root_transform = finger_transform
		#root_transform = Transform.IDENTITY
	elif bone_name == "wrist":
		wrist_transform = Transform.IDENTITY
		finger_transform = Transform.IDENTITY
	else:
	
		#var hand_trans = self.get_node("../").transform
		#if not hand_trans:
		#	#print("No hand transform")
		#	#return
		#	pass
			
		#var inv_hand = hand_trans.affine_inverse()
		#if not inv_hand:
		#	print("Could not invert hand transform")
		
		#finger_transform = finger_transform
		
		#var inv_root = root_transform.affine_inverse()
		#finger_transform = finger_transform * inv_root
		
		var prev_bone_name_hand = prev_bone_name + prev_suffix
		var prev_bone_index = skel.find_bone(prev_bone_name_hand)
		
		
		#var prev_bone_transform = skel.get_bone_pose(prev_bone_index)
		
		var prev_openxr_bone = mapping[prev_bone_name]
		var prev_bone_transform = openxr_ext.get_hand_tracking(cid, prev_openxr_bone)
		
		var inv_prev_bone_transform = prev_bone_transform.affine_inverse()
		finger_transform = finger_transform * inv_prev_bone_transform

	skel.set_bone_pose(bone_index, finger_transform)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	finger(cid, "root", "", "", "")
	finger(cid, "wrist", hand_suffix(), "", "")
	
	finger(cid, "finger_index_0", hand_suffix(), "root", "")
	finger(cid, "finger_index_1", hand_suffix(), "finger_index_0", hand_suffix())
	finger(cid, "finger_index_2", hand_suffix(), "finger_index_1", hand_suffix())
	finger(cid, "finger_index", hand_suffix() + "_end", "finger_index_2", hand_suffix())

	finger(cid, "finger_middle_0", hand_suffix(), "root", "")
	finger(cid, "finger_middle_1", hand_suffix(), "finger_middle_0", hand_suffix())
	finger(cid, "finger_middle_2", hand_suffix(), "finger_middle_1", hand_suffix())
	finger(cid, "finger_middle", hand_suffix() + "_end", "finger_middle_2", hand_suffix())
	
	finger(cid, "finger_ring_0", hand_suffix(), "root", "")
	finger(cid, "finger_ring_1", hand_suffix(), "finger_ring_0", hand_suffix())
	finger(cid, "finger_ring_2", hand_suffix(), "finger_ring_1", hand_suffix())
	finger(cid, "finger_ring", hand_suffix() + "_end", "finger_ring_2", hand_suffix())

	finger(cid, "finger_pinky_0", hand_suffix(), "root", "")
	finger(cid, "finger_pinky_1", hand_suffix(), "finger_pinky_0", hand_suffix())
	finger(cid, "finger_pinky_2", hand_suffix(), "finger_pinky_1", hand_suffix())
	finger(cid, "finger_pinky", hand_suffix() + "_end", "finger_pinky_2", hand_suffix())
	
