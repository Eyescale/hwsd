
if(NOT SERVUS_USE_DNSSD AND NOT SERVUS_USE_AVAHI_CLIENT)
  message(STATUS "Warning: Servus does not provide ZEROCONF, not building: hw_sd_list")
  return()
endif()

set(HEADERS)
set(SOURCES
  hw_sd_list.cpp
)

list(APPEND LINK_LIBRARIES hwsd_gpu_dns_sd hwsd_net_dns_sd hwsd_net_sys)
source_group(\\ FILES ${SOURCES} ${HEADERS})

add_executable(hw_sd_list ${SOURCES} ${HEADERS})
target_link_libraries(hw_sd_list ${LINK_LIBRARIES})

install(TARGETS hw_sd_list DESTINATION bin COMPONENT tools)
install_symlink(FROM hw_sd_list TO gpu_sd_list WORKING_DIRECTORY bin COMPONENT tools)
install_symlink(FROM hw_sd_list TO net_sd_list WORKING_DIRECTORY bin COMPONENT tools)
install(FILES ${SOURCES} ${HEADERS} DESTINATION share/hwsd/examples
  COMPONENT examples)

