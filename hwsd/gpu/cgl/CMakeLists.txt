if(NOT APPLE OR NOT OPENGL_FOUND)
  return()
endif()

find_library(CARBON_LIBRARY Carbon)

set(HWSD_GPU_CGL_PUBLIC_HEADERS module.h)
set(HWSD_GPU_CGL_SOURCES module.cpp)
set(HWSD_GPU_CGL_LINK_LIBRARIES ${CARBON_LIBRARY} ${OPENGL_gl_LIBRARY} hwsd)

set(HWSD_GPU_CGL_INCLUDE_NAME hwsd/gpu/cgl)
common_library(hwsd_gpu_cgl)
