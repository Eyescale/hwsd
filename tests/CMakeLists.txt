# Copyright (c) 2010 Daniel Pfeifer
#               2010-2013, Stefan Eilemann <eile@eyescale.ch>
#
# Change this number when adding tests to force a CMake run: 1

if(NOT TARGET ${PROJECT_NAME}-tests)
  add_custom_target(${PROJECT_NAME}-tests)
  set_target_properties(${PROJECT_NAME}-tests PROPERTIES
    EXCLUDE_FROM_DEFAULT_BUILD ON FOLDER ${PROJECT_NAME}/tests)
endif()
