if(NOT WIN32 OR NOT OPENGL_FOUND)
  return()
endif()

set(HWSD_GPU_WGL_PUBLIC_HEADERS module.h)
set(HWSD_GPU_WGL_SOURCES module.cpp)
set(HWSD_GPU_WGL_LINK_LIBRARIES PUBLIC hwsd PRIVATE ${OPENGL_gl_LIBRARY})

set(HWSD_GPU_WGL_INCLUDE_NAME hwsd/gpu/wgl)
common_library(hwsd_gpu_wgl)
