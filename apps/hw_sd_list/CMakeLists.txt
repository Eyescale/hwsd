
if(NOT LUNCHBOX_USE_DNSSD)
  return()
endif()

set(HEADERS)
set(SOURCES
  hw_sd_list.cpp
)

source_group(\\ FILES ${SOURCES} ${HEADERS})
add_executable(hw_sd_list ${SOURCES} ${HEADERS})
target_link_libraries(hw_sd_list hwsd hwsd_gpu_dns_sd hwsd_net_dns_sd)
install(TARGETS hw_sd_list DESTINATION bin COMPONENT tools)
install_symlink(bin/hw_sd_list bin/gpu_sd_list COMPONENT tools)
install_symlink(bin/hw_sd_list bin/net_sd_list COMPONENT tools)
install(FILES ${SOURCES} ${HEADERS} DESTINATION share/hwsd/examples
  COMPONENT examples)

