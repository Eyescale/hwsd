
if(NOT SERVUS_USE_DNSSD AND NOT SERVUS_USE_AVAHI_CLIENT)
  message(STATUS "Warning: Servus does not provide ZEROCONF, not building: hw_sd")
  return()
endif()

include(InstallSymlink)

set(HEADERS)
set(SOURCES hw_sd.cpp)

include_directories(${SERVUS_INCLUDE_DIRS})
list(APPEND LINK_LIBRARIES ${SERVUS_LIBRARIES} ${LUNCHBOX_LIBRARIES}
  hwsd_gpu_dns_sd hwsd_net_dns_sd hwsd_net_sys)

if(Boost_FOUND)
  list(APPEND LINK_LIBRARIES ${Boost_PROGRAM_OPTIONS_LIBRARY})
  add_definitions(-DBOOST_PROGRAM_OPTIONS_DYN_LINK)
else()
  message(STATUS
    "boost::program_options missing, command line options not available")
endif()

source_group(\\ FILES ${SOURCES} ${HEADERS})
add_executable(hw_sd ${SOURCES} ${HEADERS})
target_link_libraries(hw_sd ${LINK_LIBRARIES})
install(TARGETS hw_sd DESTINATION bin COMPONENT daemon)
install_symlink(FROM hw_sd TO gpu_sd WORKING_DIRECTORY bin COMPONENT daemon)
install_symlink(FROM hw_sd TO net_sd WORKING_DIRECTORY bin COMPONENT daemon)
install(FILES ${SOURCES} ${HEADERS} DESTINATION share/hwsd/examples
  COMPONENT examples)
