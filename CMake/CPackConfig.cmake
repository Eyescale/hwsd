# Copyright (c) 2010 Daniel Pfeifer <daniel@pfeifer-mail.de>
#               2010-2012 Stefan Eilemann <eile@eyescale.ch>

#info: http://www.itk.org/Wiki/CMake:Component_Install_With_CPack

#configure_file(${CMAKE_SOURCE_DIR}/CMake/Equalizer.in.spec 
#              ${CMAKE_SOURCE_DIR}/CMake/Equalizer.spec @ONLY)

set(CPACK_PROJECT_NAME gpu-sd) # historical...
set(CPACK_PACKAGE_VENDOR "www.eyescale.ch")
set(CPACK_PACKAGE_CONTACT "Stefan Eilemann <eile@eyescale.ch>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Local and remote GPU discovery")
set(CPACK_DEBIAN_BUILD_DEPENDS libgl1-mesa-dev libx11-dev
  libavahi-compat-libdnssd-dev libboost-program-options-dev
  ${SERVUS_DEB_BUILD_DEPENDENCIES})
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libstdc++6, libgl1-mesa-glx, libavahi-compat-libdnssd1, libboost-program-options-dev, ${LUNCHBOX_DEB_DEPENDENCIES}")

# components
set(CPACK_COMPONENTS_ALL dev runtime tools daemon)

set(CPACK_COMPONENT_RUNTIME_DISPLAY_NAME
  "GPU-SD libraries and applications")
set(CPACK_COMPONENT_RUNTIME_DESCRIPTION
  "Runtime components of GPU-SD: The core library, discovery modules, announcement daemon and discovery tools")

set(CPACK_COMPONENT_DEV_DISPLAY_NAME "GPU-SD development files")
set(CPACK_COMPONENT_DEV_DESCRIPTION
  "GPU-SD header and library Files for development")
set(CPACK_COMPONENT_DEV_DEPENDS runtime)

set(CPACK_COMPONENT_DAEMON_DISPLAY_NAME "GPU-SD ZeroConf daemon")
set(CPACK_COMPONENT_DAEMON_DESCRIPTION
  "GPU-SD ZeroConf annoucement daemon")
set(CPACK_COMPONENT_DAEMON_DEPENDS runtime)

set(CPACK_COMPONENT_TOOLS_DISPLAY_NAME "GPU-SD helper applications")
set(CPACK_COMPONENT_TOOLS_DESCRIPTION
  "GPU-SD Helper applications")
set(CPACK_COMPONENT_TOOLS_DEPENDS runtime)

include(CommonCPack)
