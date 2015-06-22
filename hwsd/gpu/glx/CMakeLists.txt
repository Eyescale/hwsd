if(NOT X11_FOUND OR NOT OPENGL_FOUND)
  return()
endif()

include_directories(SYSTEM /usr/X11R6/include)

if(APPLE) # reset OpenGL to X11 version
  get_filename_component(X11DIR "${X11_X11_LIB}" PATH)
  set(OPENGL_gl_LIBRARY ${X11DIR}/libGL.dylib)
endif()

set(HWSD_GPU_GLX_PUBLIC_HEADERS module.h)
set(HWSD_GPU_GLX_SOURCES module.cpp)
set(HWSD_GPU_GLX_LINK_LIBRARIES ${OPENGL_gl_LIBRARY} ${X11_LIBRARIES} hwsd)

set(HWSD_GPU_GLX_INCLUDE_NAME hwsd/gpu/glx)
common_library(hwsd_gpu_glx)
