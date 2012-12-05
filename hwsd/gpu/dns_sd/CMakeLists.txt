
if(NOT LUNCHBOX_USE_DNSSD)
  return()
endif()

set(HEADERS
  module.h
)

set(SOURCES
  module.cpp
)

include_directories(${LUNCHBOX_INCLUDE_DIRS})

set(LIBRARY hwsd_gpu_dns_sd)
set(INCLUDE include/hwsd/gpu/dns_sd)

source_group(\\ FILES CMakeLists.txt ${HEADERS} ${SOURCES})
add_library(${LIBRARY} SHARED ${HEADERS} ${SOURCES})
if(WIN32)
  target_link_libraries(${LIBRARY} ${LUNCHBOX_LIBRARIES} hwsd ws2_32)
else()
  target_link_libraries(${LIBRARY} ${LUNCHBOX_LIBRARIES} hwsd)
endif()
set_target_properties(${LIBRARY} PROPERTIES VERSION ${VERSION}
  SOVERSION ${VERSION_MAJOR} PUBLIC_HEADER "${HEADERS}")
install(TARGETS ${LIBRARY}
  PUBLIC_HEADER DESTINATION ${INCLUDE} COMPONENT dev
  ARCHIVE DESTINATION lib COMPONENT dev
  RUNTIME DESTINATION bin COMPONENT runtime
  LIBRARY DESTINATION lib COMPONENT runtime)
