# GDNative driver for OpenXR

Versions
--------

Requires Godot 3.1.

Note: The godot engine (the full engine, not this plugin) must be compiled in release_debug mode, i.e. with

    scons platform=x11 target=release_debug

If godot engine is compiled with `target=debug` instead, you might get a (bogus) error from monado

    client_gl_compositor_init - Needed extension GL_EXT_memory_object_fd not supported

On the other hand the godot_openxr plugin can be compiled in debug mode without issues.

Status
------

* This plugin only supports Linux/X11.
* HMD poses and HMD rendering work.
* Controller/Actions are unimplemented.
* The code structure could be improved.

Building this module
--------------------
In order to compile this module you will have to clone the source code to disk. You will need a C/C++ compiler, python and scons installed. This is the same toolchain you will need in order to compile Godot from master. The documentation on Godot is a very good place to read up on this. It is too much information to duplicate here.

This module presumes that the OpenXR headers openxr/openxr.h and openxr/openxr_platform.h are installed in a location where they can be included without setup, and that the OpenXR loader libopenxr_api.so can be linked without special setup, for example /usr/include/openxr/openxr.h and /usr/lib/libopenxr_api.so.

TODO: add openxr loader and headers as a submodule

*Compiling*
If everything is in place compiling should be pretty straight forward

For Linux: ```scons platform=linux```
For OSX: ```scons platform=osx```
For Windows: ```scons platform=windows```

The compiled plugin and related files will be placed in `demo/addons/`. When using godot_openxr in another project, copy this directory.

Testing
-------
Start Godot, open the godot_openxr/demo project and click play.

Using this module in your own project
-------------------------------------

Example: Converting a godot-openvr project

1. Remove the `addons/godot-openvr` directory from the project.
2. Copy the `godot-openxr/demo/addons/godot-openxr` directory to `your_project/addons/`.
3. If your project was already set up to use OpenVR, find `ARVRServer.find_interface("OpenVR")` and replace `"OpenVR"` with `"OpenXR"`.

Since the module is laid out like godot-openvr, the basic documentation for integrating OpenVR into a project also applies to OpenXR.

The only differences should be: Since there is no OpenXR asset on the store, the godot-openxr directory has to be manually put into the project's addon/ directory and in the project's gdscript the `OpenXR` string has to be used in the `find_interface()` call.

License
-------
The source code for the module is released under MIT license (see license file).

About this repository
---------------------
This repository is a fork of godot_openhmd which was created and is maintained by [Bastiaan Olij](https://github.com/BastiaanOlij) a.k.a. Mux213
OpenXR implementation was created by [Christoph Haag](https://github.com/ChristophHaag/)

Originally hosted on https://gitlab.freedesktop.org/monado/demos/godot_openxr now lives on https://github.com/GodotVR/godot_openxr
