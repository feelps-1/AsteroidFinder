# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/flops/projects/asteroidfinder

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/flops/projects/asteroidfinder/build

# Include any dependencies generated for this target.
include CMakeFiles/fits_opencv.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/fits_opencv.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/fits_opencv.dir/flags.make

CMakeFiles/fits_opencv.dir/src/fits_opencv.cpp.o: CMakeFiles/fits_opencv.dir/flags.make
CMakeFiles/fits_opencv.dir/src/fits_opencv.cpp.o: ../src/fits_opencv.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/flops/projects/asteroidfinder/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/fits_opencv.dir/src/fits_opencv.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fits_opencv.dir/src/fits_opencv.cpp.o -c /home/flops/projects/asteroidfinder/src/fits_opencv.cpp

CMakeFiles/fits_opencv.dir/src/fits_opencv.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fits_opencv.dir/src/fits_opencv.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/flops/projects/asteroidfinder/src/fits_opencv.cpp > CMakeFiles/fits_opencv.dir/src/fits_opencv.cpp.i

CMakeFiles/fits_opencv.dir/src/fits_opencv.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fits_opencv.dir/src/fits_opencv.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/flops/projects/asteroidfinder/src/fits_opencv.cpp -o CMakeFiles/fits_opencv.dir/src/fits_opencv.cpp.s

# Object files for target fits_opencv
fits_opencv_OBJECTS = \
"CMakeFiles/fits_opencv.dir/src/fits_opencv.cpp.o"

# External object files for target fits_opencv
fits_opencv_EXTERNAL_OBJECTS =

fits_opencv: CMakeFiles/fits_opencv.dir/src/fits_opencv.cpp.o
fits_opencv: CMakeFiles/fits_opencv.dir/build.make
fits_opencv: /usr/local/lib/libopencv_gapi.so.4.12.0
fits_opencv: /usr/local/lib/libopencv_highgui.so.4.12.0
fits_opencv: /usr/local/lib/libopencv_ml.so.4.12.0
fits_opencv: /usr/local/lib/libopencv_objdetect.so.4.12.0
fits_opencv: /usr/local/lib/libopencv_photo.so.4.12.0
fits_opencv: /usr/local/lib/libopencv_stitching.so.4.12.0
fits_opencv: /usr/local/lib/libopencv_video.so.4.12.0
fits_opencv: /usr/local/lib/libopencv_videoio.so.4.12.0
fits_opencv: /usr/local/lib/libcfitsio.so
fits_opencv: /usr/local/lib/libCCfits.so
fits_opencv: /usr/local/lib/libopencv_imgcodecs.so.4.12.0
fits_opencv: /usr/local/lib/libopencv_dnn.so.4.12.0
fits_opencv: /usr/local/lib/libopencv_calib3d.so.4.12.0
fits_opencv: /usr/local/lib/libopencv_features2d.so.4.12.0
fits_opencv: /usr/local/lib/libopencv_flann.so.4.12.0
fits_opencv: /usr/local/lib/libopencv_imgproc.so.4.12.0
fits_opencv: /usr/local/lib/libopencv_core.so.4.12.0
fits_opencv: CMakeFiles/fits_opencv.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/flops/projects/asteroidfinder/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable fits_opencv"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fits_opencv.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/fits_opencv.dir/build: fits_opencv

.PHONY : CMakeFiles/fits_opencv.dir/build

CMakeFiles/fits_opencv.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/fits_opencv.dir/cmake_clean.cmake
.PHONY : CMakeFiles/fits_opencv.dir/clean

CMakeFiles/fits_opencv.dir/depend:
	cd /home/flops/projects/asteroidfinder/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/flops/projects/asteroidfinder /home/flops/projects/asteroidfinder /home/flops/projects/asteroidfinder/build /home/flops/projects/asteroidfinder/build /home/flops/projects/asteroidfinder/build/CMakeFiles/fits_opencv.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/fits_opencv.dir/depend

