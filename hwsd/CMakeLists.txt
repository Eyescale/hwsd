
configure_file("version.in.h" "${OUTPUT_INCLUDE_DIR}/hwsd/version.h" @ONLY)

set(HEADERS
  "${OUTPUT_INCLUDE_DIR}/hwsd/version.h"
  api.h
)

set(SOURCES
  version.cpp
)

set(LIBRARY hwsd)
set(INCLUDE include/${LIBRARY})

source_group(\\ FILES CMakeLists.txt ${HEADERS} ${SOURCES})
add_library(${LIBRARY} SHARED ${HEADERS} ${SOURCES})
set_target_properties(${LIBRARY} PROPERTIES
  VERSION ${VERSION} SOVERSION ${VERSION_MAJOR}
  PUBLIC_HEADER "${HEADERS}" COMPILE_DEFINITIONS "HWSD_SHARED")
if(CMAKE_PROJECT_NAME MATCHES "hwsd")
  install(TARGETS ${LIBRARY}
    PUBLIC_HEADER DESTINATION ${INCLUDE} COMPONENT dev
    ARCHIVE DESTINATION lib COMPONENT dev
    RUNTIME DESTINATION bin COMPONENT runtime
    LIBRARY DESTINATION lib COMPONENT runtime)
endif()

add_subdirectory(gpu)
#add_subdirectory(net)
