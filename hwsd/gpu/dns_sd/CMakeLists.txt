
if(NOT LUNCHBOX_USE_DNSSD AND NOT LUNCHBOX_USE_AVAHI_CLIENT)
  message(STATUS "Warning: Lunchbox does not provide ZEROCONF, not building: hwsd_gpu_dns_sd")
  return()
endif()

include_directories(${LUNCHBOX_INCLUDE_DIRS})

set(HWSD_GPU_DNS_SD_PUBLIC_HEADERS module.h)
set(HWSD_GPU_DNS_SD_SOURCES module.cpp)
set(HWSD_GPU_DNS_SD_LINK_LIBRARIES ${LUNCHBOX_LIBRARIES} hwsd)
if(WIN32)
  list(APPEND HWSD_GPU_DNS_SD_LINK_LIBRARIES ${LUNCHBOX_LIBRARIES} ws2_32)
endif()

set(PROJECT_INCLUDE_NAME hwsd/gpu/dns_sd)
common_library(hwsd_gpu_dns_sd)
