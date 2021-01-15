////////////////////////////////////////////////////////////////////////////////////////////////
// Class for OpenXR Paths
// Note, included through OpenXRApi.h
//
// XrPath is an atom in OpenXR that identifies a given input, profile or other such
// entity. Toward the developer these are exposed as a string such as "/user/hand/left".
// the XrPath object holds an atomic value that relates to this path and it is that
// which we use to identify entities we're interacting with
//
// Path simply encapsulates this so we can keep the string during configuration and
// obtain our atoms once OpenXR is initialised.
// There is a container of paths in OpenXRApi so we can ensure we have a single instance
// per path string and thus use the pointer to this instance to identify the path

// Things to discuss, decide upon going forward:
// - add some form of reference counting so we automatically clean up paths we no longer access
// - forego this and instead just store the path by name and resolve it to an XrPath when needed, we're kind of duplicating what OpenXR already does.
// - create a full dictionary of possible paths, OpenXR doesn't have any way to enumerate paths so we may need this if we want to provide a coherent UI

#ifndef OPENXR_PATH_H
#define OPENXR_PATH_H

class Path {
private:
	char name[XR_MAX_PATH_LENGTH];

	// Populated once OpenXR is initialised
	XrPath path = XR_NULL_PATH;

public:
	Path();
	Path(const char *p_name);
	~Path();

	char *get_name() const;
	bool set_name(const char *p_name);

	XrPath get_path() const;
	bool create(OpenXRApi *p_api);
	void destroy();
};

#endif /* !OPENXR_PATH_H */
