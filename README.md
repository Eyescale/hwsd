# Hardware Service Discovery

HW-SD is a library and daemon for the discovery and announcement of
hardware resources using ZeroConf. It enables auto-configuration
of ad-hoc GPU clusters and multi-GPU machines.

The source code is hosted on
[github](https://github.com/Eyescale/hwsd) and documented on
[eyescale.github.io](https://eyescale.github.io/#hwsd).

## Modules

The HW-SD library uses modules which implement discovery using different
protocols. Each module is a separate library, which can be selectively
linked by applications to limit dependencies. Currently available are:

- gpu_dns_sd: Remote ZeroConf (Bonjour) discovery for GPUs announced by
  the daemon
- gpu_cgl: Local discovery of Carbon displays (Mac OS X only)
- gpu_glx: Local discovery of X11 servers and screens
- gpu_wgl: Local discovery of WGL_NV_gpu_affinity, WGL_AMD_gpu_association
  or Windows displays (Windows only)
- net_dns_sd: Remote ZeroConf (Bonjour) discovery for network interfaces
  announced by the daemon
- net_sys: Local discovery of network interfaces

## VirtualGL

When an application is run through VirtualGL, hwsd detects this and
sets the FLAG\_VIRTUALGL on all local GPUs, and additionally
FLAG\_VIRTUALGL\_DISPLAY on the GPU used by VirtualGL for
redirection. This is only implemented for GLX so far
([more info](https://github.com/Eyescale/Equalizer/issues/67)).

## Announcement Daemon

The daemon uses all available local modules to query local GPUs and
network interfaces to announce them using ZeroConf to the local
network. The service type name is "_gpu-sd" and "_net-sd". The dns_sd
discovery module gathers the information announced by all daemons on the
local network. The following protocol is used by the daemon:

* Session=default | &lt;string&gt;
* NodeID=&lt;UUID&gt;
* Hostname=&lt;string&gt; // optional, hostname for connections

* GPU Count=&lt;integer&gt;
* GPU&lt;integer&gt; Type=GLX | WGL | WGLn | WGLa | CGL
* GPU&lt;integer&gt; Port=&lt;integer&gt; // X11 display number, 0 otherwise
* GPU&lt;integer&gt; Device=&lt;integer&gt; // X11 screen number,
  wglEnumGpusNV index, CGDirectDisplayID
* GPU&lt;integer&gt; Width=&lt;integer&gt;
* GPU&lt;integer&gt; Height=&lt;integer&gt;
* GPU&lt;integer&gt; X=&lt;integer&gt;
* GPU&lt;integer&gt; Y=&lt;integer&gt;
* GPU&lt;integer&gt; Flags=&lt;integer&gt; // optional flags (see gpuInfo.h)

* Net Count=&lt;integer&gt;
* Net&lt;integer&gt; Type=TYPE_ETHERNET | TYPE_INFINIBAND | TYPE_LOOPBACK | TYPE_UNKNOWN
* Net&lt;integer&gt; Name=&lt;string&gt;
* Net&lt;integer&gt; Hostname=&lt;string&gt;
* Net&lt;integer&gt; MAC=&lt;string&gt; // ':' as separator
* Net&lt;integer&gt; IPv4=&lt;string&gt; // ':' as separator
* Net&lt;integer&gt; IPv6=&lt;string&gt; // ':' as separator
* Net&lt;integer&gt; Linkspeed=&lt;integer&gt; // in Megabits per second
* Net&lt;integer&gt; Up=&lt;bool&gt;

## Building from Source

HWSD is a cross-platform library, designed to run on any modern
operating system, including all Unix variants and the Windows operating
system.  Zeroconf support in Lunchbox is required for the DNS_SD module
and applications. The following platforms and build environments are
tested:

* Linux: Ubuntu 16.04, RHEL 6.8 (Makefile, Ninja)
* Windows: 7 (Visual Studio 2012)
* Mac OS X: 10.9 (Makefile, Ninja)

The build system is using CMake, with the standard CMake build process:

    git clone https://github.com/Eyescale/hwsd.git
    mkdir hwsd/build
    cd hwsd/build
    cmake -GNinja ..
    ninja

A ZeroConf implementation is required for the dns_sd module and the
daemon. On Mac OS X it is part of the operating system, on Linux AVAHI
is tested ('sudo apt-get install libavahi-compat-libdnssd-dev' on
Ubuntu), on Windows use the
[Bonjour SDK](https://developer.apple.com/downloads/index.action?q=Bonjour%20SDK%20for%20Windows).
If no ZeroConf implementation is found, HW-SD is only compiled with
local discovery modules.

## Bugs

Please file a [Bug Report](https://github.com/Eyescale/hwsd/issues) if
you find any issue with this software.

## Usage

An application can use the discovery by linking the relevant module
libraries, instantiating the modules in the code and then quering the
instantiated modules. The following will find all remote and local GPUs
and local network interfaces on Windows:

    #include <hwsd/hwsd.h>

    hwsd::gpu::wgl::Module::use();
    hwsd::gpu::dns_sd::Module::use();
    const hwsd::GPUInfos& gpus = hwsd::discoverGPUInfos();

    hwsd::net::sys::Module::use();
    const hwsd::NetInfos& nets = hwsd::discoverNetInfos();

Filters are chainable functors which can be passed to the query function
to discard information. The following filters are provided:

* DuplicateFilter eliminates duplicates, e.g., when one announcement is
  seen on multiple interfaces
* MirrorFilter eliminates the same GPU with a different type, e.g., when
  enabling both the cgl and glx module on Mac OS X.
* SessionFilter discards all resources not belonging to a given session
