# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/fengbingbing/nti2022_wohospeech_asr/AlphaSpeech_ASR/config/arch64

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/fengbingbing/nti2022_wohospeech_asr/AlphaSpeech_ASR/build

# Include any dependencies generated for this target.
include CMakeFiles/asr_api_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/asr_api_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/asr_api_test.dir/flags.make

CMakeFiles/asr_api_test.dir/home/fengbingbing/nti2022_wohospeech_asr/AlphaSpeech_ASR/test/asr_api_test.o: CMakeFiles/asr_api_test.dir/flags.make
CMakeFiles/asr_api_test.dir/home/fengbingbing/nti2022_wohospeech_asr/AlphaSpeech_ASR/test/asr_api_test.o: /home/fengbingbing/nti2022_wohospeech_asr/AlphaSpeech_ASR/test/asr_api_test.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fengbingbing/nti2022_wohospeech_asr/AlphaSpeech_ASR/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/asr_api_test.dir/home/fengbingbing/nti2022_wohospeech_asr/AlphaSpeech_ASR/test/asr_api_test.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/asr_api_test.dir/home/fengbingbing/nti2022_wohospeech_asr/AlphaSpeech_ASR/test/asr_api_test.o -c /home/fengbingbing/nti2022_wohospeech_asr/AlphaSpeech_ASR/test/asr_api_test.c

CMakeFiles/asr_api_test.dir/home/fengbingbing/nti2022_wohospeech_asr/AlphaSpeech_ASR/test/asr_api_test.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/asr_api_test.dir/home/fengbingbing/nti2022_wohospeech_asr/AlphaSpeech_ASR/test/asr_api_test.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/fengbingbing/nti2022_wohospeech_asr/AlphaSpeech_ASR/test/asr_api_test.c > CMakeFiles/asr_api_test.dir/home/fengbingbing/nti2022_wohospeech_asr/AlphaSpeech_ASR/test/asr_api_test.i

CMakeFiles/asr_api_test.dir/home/fengbingbing/nti2022_wohospeech_asr/AlphaSpeech_ASR/test/asr_api_test.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/asr_api_test.dir/home/fengbingbing/nti2022_wohospeech_asr/AlphaSpeech_ASR/test/asr_api_test.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/fengbingbing/nti2022_wohospeech_asr/AlphaSpeech_ASR/test/asr_api_test.c -o CMakeFiles/asr_api_test.dir/home/fengbingbing/nti2022_wohospeech_asr/AlphaSpeech_ASR/test/asr_api_test.s

# Object files for target asr_api_test
asr_api_test_OBJECTS = \
"CMakeFiles/asr_api_test.dir/home/fengbingbing/nti2022_wohospeech_asr/AlphaSpeech_ASR/test/asr_api_test.o"

# External object files for target asr_api_test
asr_api_test_EXTERNAL_OBJECTS =

asr_api_test: CMakeFiles/asr_api_test.dir/home/fengbingbing/nti2022_wohospeech_asr/AlphaSpeech_ASR/test/asr_api_test.o
asr_api_test: CMakeFiles/asr_api_test.dir/build.make
asr_api_test: libasr.so
asr_api_test: CMakeFiles/asr_api_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/fengbingbing/nti2022_wohospeech_asr/AlphaSpeech_ASR/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable asr_api_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/asr_api_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/asr_api_test.dir/build: asr_api_test

.PHONY : CMakeFiles/asr_api_test.dir/build

CMakeFiles/asr_api_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/asr_api_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/asr_api_test.dir/clean

CMakeFiles/asr_api_test.dir/depend:
	cd /home/fengbingbing/nti2022_wohospeech_asr/AlphaSpeech_ASR/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/fengbingbing/nti2022_wohospeech_asr/AlphaSpeech_ASR/config/arch64 /home/fengbingbing/nti2022_wohospeech_asr/AlphaSpeech_ASR/config/arch64 /home/fengbingbing/nti2022_wohospeech_asr/AlphaSpeech_ASR/build /home/fengbingbing/nti2022_wohospeech_asr/AlphaSpeech_ASR/build /home/fengbingbing/nti2022_wohospeech_asr/AlphaSpeech_ASR/build/CMakeFiles/asr_api_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/asr_api_test.dir/depend

