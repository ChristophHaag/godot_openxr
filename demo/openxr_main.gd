extends ARVROrigin

# Add this script to your ARVROrigin node and it will initialise OpenXR for you automatically.

# If we render to a custom viewport give our node path here.
export (NodePath) var viewport = null

# Convenience setting for setting physics update rate to a multiple of our HMDs frame rate (set to 0 to ignore)
export var physics_factor = 2

var arvr_interface : ARVRInterface = null

func _ready():
	# Find the interface and initialise
	arvr_interface = ARVRServer.find_interface("OpenXR")
	if arvr_interface and arvr_interface.initialize():
		# We can't query our HMDs refresh rate just yet so we hardcode this to 90
		var refresh_rate = 90
		
		# check our viewport
		var vp : Viewport = null
		if viewport:
			vp = get_node(viewport)
			if vp:
				# We copy this, while the ARVRServer will resize the size of the viewport automatically
				# it can't feed it back into the node. 
				vp.size = arvr_interface.get_render_targetsize()
				
		
		# No viewport? get our main viewport
		if !vp:
			vp = get_viewport()
		
		# switch to ARVR mode
		vp.arvr = true
		print("Started ARVR")
		
		# keep linear color space, not needed and thus ignored with the GLES2 renderer
		vp.keep_3d_linear = true
		
		# make sure vsync is disabled or we'll be limited to 60fps
		OS.vsync_enabled = false
		
		if physics_factor > 0:
			# Set our physics to a multiple of our refresh rate to get in sync with our rendering
			Engine.iterations_per_second = refresh_rate * physics_factor
