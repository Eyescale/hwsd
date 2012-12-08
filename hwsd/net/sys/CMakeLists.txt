set(HEADERS
  module.h
)

set(SOURCES
  module.cpp
)


set(LIBRARY hwsd_net_sys)
set(INCLUDE include/hwsd/net/sys)

source_group(\\ FILES CMakeLists.txt ${HEADERS} ${SOURCES})
add_library(${LIBRARY} SHARED ${HEADERS} ${SOURCES})
if(WIN32)
  target_link_libraries(${LIBRARY} hwsd ${LUNCHBOX_LIBRARIES} iphlpapi ws2_32)
else()
  target_link_libraries(${LIBRARY} hwsd ${LUNCHBOX_LIBRARIES})
endif()
set_target_properties(${LIBRARY} PROPERTIES
  VERSION ${VERSION} SOVERSION ${VERSION_MAJOR}
  PUBLIC_HEADER "${HEADERS}" COMPILE_DEFINITIONS "HWSD_SHARED")
install(TARGETS ${LIBRARY}
  PUBLIC_HEADER DESTINATION ${INCLUDE} COMPONENT dev
  ARCHIVE DESTINATION lib COMPONENT dev
  RUNTIME DESTINATION bin COMPONENT runtime
  LIBRARY DESTINATION lib COMPONENT runtime)
