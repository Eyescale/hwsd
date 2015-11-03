
if(NOT SERVUS_USE_DNSSD AND NOT SERVUS_USE_AVAHI_CLIENT)
  message(STATUS "Warning: Servus does not provide ZEROCONF, not building: hw_sd_list")
  return()
endif()

set(HW_SD_LIST_SOURCES hw_sd_list.cpp)

set(HW_SD_LIST_LINK_LIBRARIES ${LINK_LIBRARIES}
  hwsd_gpu_dns_sd hwsd_net_dns_sd hwsd_net_sys)

common_application(hw_sd_list)
install_symlink(FROM hw_sd_list TO gpu_sd_list WORKING_DIRECTORY bin
  COMPONENT apps)
install_symlink(FROM hw_sd_list TO net_sd_list WORKING_DIRECTORY bin
  COMPONENT apps)
install(FILES ${HW_SD_LIST_SOURCES} DESTINATION share/hwsd/examples
  COMPONENT examples)

