if(NOT X11_FOUND OR NOT OPENGL_FOUND)
  return()
endif()

set(HEADERS
  module.h
)

set(SOURCES
  module.cpp
)

include_directories(SYSTEM /usr/X11R6/include)

if(APPLE) # reset OpenGL to X11 version
  get_filename_component(X11DIR "${X11_X11_LIB}" PATH)
  set(OPENGL_gl_LIBRARY ${X11DIR}/libGL.dylib)
endif()

set(LIBRARY hwsd_gpu_glx)
set(INCLUDE include/hwsd/gpu/glx)

source_group(\\ FILES CMakeLists.txt ${HEADERS} ${SOURCES})
add_library(${LIBRARY} SHARED ${HEADERS} ${SOURCES})
target_link_libraries(${LIBRARY} ${OPENGL_gl_LIBRARY} ${X11_LIBRARIES} hwsd)
set_target_properties(${LIBRARY} PROPERTIES VERSION ${VERSION}
  SOVERSION ${VERSION_MAJOR} PUBLIC_HEADER "${HEADERS}")
install(TARGETS ${LIBRARY}
  PUBLIC_HEADER DESTINATION ${INCLUDE} COMPONENT dev
  ARCHIVE DESTINATION lib COMPONENT dev
  RUNTIME DESTINATION bin COMPONENT runtime
  LIBRARY DESTINATION lib COMPONENT runtime)
