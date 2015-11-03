# Copyright (c) 2010-2015 Stefan Eilemann <eile@eyescale.ch>
#                         Daniel Pfeifer <daniel@pfeifer-mail.de>

set(CPACK_PACKAGE_VENDOR "www.eyescale.ch")
set(CPACK_PACKAGE_DESCRIPTION_FILE ${PROJECT_SOURCE_DIR}/doc/Changelog.md)
set(CPACK_RESOURCE_FILE_README ${PROJECT_SOURCE_DIR}/README.md)

set(CPACK_DEBIAN_BUILD_DEPENDS
  ${HWSD_BUILD_DEBS} ${LUNCHBOX_DEB_DEV_DEPENDENCY})
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libstdc++6, libgl1-mesa-glx, libboost-program-options-dev, libboost-regex-dev, libqt4-network, ${LUNCHBOX_DEB_DEV_DEPENDENCY}")

set(CPACK_MACPORTS_CATEGORY graphics)
set(CPACK_MACPORTS_DEPENDS boost qt4-mac-devel Lunchbox)

# components
set(CPACK_COMPONENTS_ALL dev lib apps examples)

set(CPACK_COMPONENT_LIB_DISPLAY_NAME "HW-SD libraries")
set(CPACK_COMPONENT_LIB_DESCRIPTION
  "Runtime components of HW-SD: The core library and discovery modules")

set(CPACK_COMPONENT_DEV_DISPLAY_NAME "HW-SD development files")
set(CPACK_COMPONENT_DEV_DESCRIPTION
  "HW-SD header and library files for development")
set(CPACK_COMPONENT_DEV_DEPENDS lib)

set(CPACK_COMPONENT_APPS_DISPLAY_NAME "HW-SD ZeroConf applications")
set(CPACK_COMPONENT_APPS_DESCRIPTION "HW-SD ZeroConf applications")
set(CPACK_COMPONENT_APPS_DEPENDS lib)

set(CPACK_COMPONENT_EXAMPLES_DISPLAY_NAME "HW-SD Examples")
set(CPACK_COMPONENT_EXAMPLES_DESCRIPTION "HW-SD Example Source Code")
set(CPACK_COMPONENT_EXAMPLES_DEPENDS dev)

include(CommonCPack)
include(OSSCPack)
