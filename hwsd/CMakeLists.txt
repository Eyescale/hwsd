
configure_file("version.in.h" "${OUTPUT_INCLUDE_DIR}/hwsd/version.h" @ONLY)

set(HEADERS # public, installed
  "${OUTPUT_INCLUDE_DIR}/hwsd/version.h"
  api.h
  filter.h
  hwsd.h
  module.h
  module.ipp
  netInfo.h
  nodeInfo.h
  gpuInfo.h
  types.h
)

set(DETAIL_HEADERS # private, not installed
  detail/dns_sd_module.h
)

set(SOURCES
  filter.cpp
  hwsd.cpp
  nodeInfo.cpp
  version.cpp
)

list(APPEND LINK_LIBRARIES ${LUNCHBOX_LIBRARIES})
list(APPEND LINK_LIBRARIES ${Boost_REGEX_LIBRARY})

if(QT_QTNETWORK_FOUND)
  include(${QT_USE_FILE})
  list(APPEND LINK_LIBRARIES ${QT_LIBRARIES})
endif()

set(LIBRARY hwsd)
set(INCLUDE include/${LIBRARY})

source_group(\\ FILES CMakeLists.txt ${HEADERS} ${SOURCES})
source_group(detail FILES ${DETAIL_HEADERS})

add_library(${LIBRARY} SHARED ${HEADERS} ${DETAIL_HEADERS} ${SOURCES})
target_link_libraries(${LIBRARY} ${LINK_LIBRARIES})
set_target_properties(${LIBRARY} PROPERTIES
  VERSION ${VERSION} SOVERSION ${VERSION_MAJOR}
  PUBLIC_HEADER "${HEADERS}" COMPILE_DEFINITIONS "HWSD_SHARED;HWSD_SHARED_INL")
install(TARGETS ${LIBRARY}
  PUBLIC_HEADER DESTINATION ${INCLUDE} COMPONENT dev
  ARCHIVE DESTINATION lib COMPONENT dev
  RUNTIME DESTINATION bin COMPONENT runtime
  LIBRARY DESTINATION lib COMPONENT runtime)

add_subdirectory(gpu)
add_subdirectory(net)
