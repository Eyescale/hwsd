
if(NOT SERVUS_USE_DNSSD AND NOT SERVUS_USE_AVAHI_CLIENT)
  message(STATUS "Warning: Servus does not provide ZEROCONF, not building: hw_sd_list")
  return()
endif()

set(HW_SD_LIST_SOURCES hw_sd_list.cpp)

set(HW_SD_LIST_LINK_LIBRARIES ${LINK_LIBRARIES}
  hwsd_gpu_dns_sd hwsd_net_dns_sd hwsd_net_sys)

if(Boost_FOUND)
  list(APPEND HW_SD_LIST_LINK_LIBRARIES ${Boost_PROGRAM_OPTIONS_LIBRARY})
  add_definitions(-DBOOST_PROGRAM_OPTIONS_DYN_LINK)
else()
  message(STATUS
    "boost::program_options missing, command line options not available")
endif()

common_application(hw_sd_list)
install(FILES ${HW_SD_LIST_SOURCES} DESTINATION share/hwsd/examples
  COMPONENT examples)

