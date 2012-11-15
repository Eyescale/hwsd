
# Copyright (c) 2011-2012 Stefan Eilemann <eile@eyescale.ch>

if(DOXYGEN_FOUND)
  configure_file(doxyfooter.html ${CMAKE_CURRENT_BINARY_DIR}/doxyfooter.html
    @ONLY)
  configure_file(gpusd_w.png @CMAKE_BINARY_DIR@/doc/html/gpusd_w.png @ONLY)
endif()

install(FILES AUTHORS ../LICENSE.txt LGPL.txt GPL.txt
  DESTINATION share/gpusd/doc COMPONENT gpusdruntime)
