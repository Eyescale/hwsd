
if(NOT SERVUS_USE_DNSSD AND NOT SERVUS_USE_AVAHI_CLIENT)
  message(STATUS "Warning: Servus does not provide ZEROCONF, not building: hwsd_gpu_dns_sd")
  return()
endif()

include_directories(${SERVUS_INCLUDE_DIRS})

set(HWSD_GPU_DNS_SD_PUBLIC_HEADERS module.h)
set(HWSD_GPU_DNS_SD_SOURCES module.cpp)
set(HWSD_GPU_DNS_SD_LINK_LIBRARIES PUBLIC hwsd PRIVATE Servus Lunchbox)
if(WIN32)
  list(APPEND HWSD_GPU_DNS_SD_LINK_LIBRARIES ws2_32)
endif()

set(HWSD_GPU_DNS_SD_INCLUDE_NAME hwsd/gpu/dns_sd)
common_library(hwsd_gpu_dns_sd)
