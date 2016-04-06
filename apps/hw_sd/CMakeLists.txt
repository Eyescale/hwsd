
if(NOT SERVUS_USE_DNSSD AND NOT SERVUS_USE_AVAHI_CLIENT)
  message(STATUS "Servus does not provide ZEROCONF, not building: hw_sd")
  return()
endif()

set(HW_SD_SOURCES hw_sd.cpp)

include_directories(${SERVUS_INCLUDE_DIRS})
set(HW_SD_LINK_LIBRARIES ${LINK_LIBRARIES} Servus Lunchbox
  hwsd_gpu_dns_sd hwsd_net_dns_sd hwsd_net_sys)

if(Boost_FOUND)
  list(APPEND HW_SD_LINK_LIBRARIES ${Boost_PROGRAM_OPTIONS_LIBRARY})
  add_definitions(-DBOOST_PROGRAM_OPTIONS_DYN_LINK)
else()
  message(STATUS
    "boost::program_options missing, command line options not available")
endif()

common_application(hw_sd)
install(FILES ${HW_SD_SOURCES} DESTINATION share/hwsd/examples
  COMPONENT examples)
