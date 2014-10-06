

set(HWSD_PACKAGE_VERSION 1.1)
set(HWSD_REPO_URL https://github.com/Eyescale/hwsd.git)
set(HWSD_REPO_TAG master)
set(HWSD_DEPENDS OpenGL Qt4 X11 REQUIRED Lunchbox Boost)
set(HWSD_BOOST_COMPONENTS "program_options regex system")
set(HWSD_QT4_COMPONENTS "QtNetwork")
set(HWSD_DEB_DEPENDS libx11-dev libgl1-mesa-dev libqt4-dev
  libboost-program-options-dev libboost-regex-dev libboost-system-dev)
set(HWSD_PORT_DEPENDS boost qt4-mac)
set(HWSD_FORCE_BUILD ${CI_BUILD})

if(CI_BUILD_COMMIT)
  set(HWSD_REPO_TAG ${CI_BUILD_COMMIT})
else()
  set(HWSD_REPO_TAG master)
endif()
set(HWSD_FORCE_BUILD ON)
set(HWSD_SOURCE ${CMAKE_SOURCE_DIR})