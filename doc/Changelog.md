Changelog {#Changelog}
=========

# git master

* [57](https://github.com/Eyescale/hwsd/pull/57):
  Only try X Server ports up to 9. On some distributions the first port number
  used by forwarded displays is 10 instead of 11.

# Release 2.0 (09-12-2016)

* [56](https://github.com/Eyescale/hwsd/pull/56):
  - Added a new flag to GPUInfo to identify which is the default display under X.
  - Removed GPUInfo::defaultValue.
  - GPU filter fixed to also consider the port numbers.
  - Do not probe display ports greater than 9 under X to avoid opening ssh -X
    virtual displays.
* [55](https://github.com/Eyescale/hwsd/pull/55): Fix memory corruption
  with SessionFilter
* [54](https://github.com/Eyescale/hwsd/pull/54): Added a command line options
  to hw_sd_list. Now results can be filtered by session name.

# Version 1.3 (2-Nov-2015)

* [47](https://github.com/Eyescale/hwsd/pull/47): Added a flag to GPU
  info to indicate VNC virtual servers
* [43](https://github.com/Eyescale/hwsd/pull/43): OS X: Fix GLX detection

# Version 1.2 (7-Jun-2015)

* [38](https://github.com/Eyescale/hwsd/pull/38): OS X: only find main
  display in hwsd_gpu_cgl module
* [37](https://github.com/Eyescale/hwsd/pull/37): MSVC: Fix static init deadlock
* [33](https://github.com/Eyescale/hwsd/pull/33): Port to Qt5

# Version 1.1 (7-Oct-2014)

* Allow filtering of unknown connections
* Adapted to latest Lunchbox code base
* Reduce mdns browse time to 50ms
