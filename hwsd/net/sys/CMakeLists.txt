
set(HWSD_NET_SYS_PUBLIC_HEADERS module.h)
set(HWSD_NET_SYS_SOURCES module.cpp)
set(HWSD_NET_SYS_LINK_LIBRARIES PUBLIC hwsd PRIVATE Lunchbox)
if(WIN32)
  list(APPEND HWSD_NET_SYS_LINK_LIBRARIES iphlpapi ws2_32)
endif()

set(HWSD_NET_SYS_INCLUDE_NAME hwsd/net/sys)
common_library(hwsd_net_sys)
