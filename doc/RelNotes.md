Release Notes {#mainpage}
============

[TOC]

# Introduction {#Introduction}

Welcome to HWSD, a daemon and library for the discovery and announcement
of hardware resources using ZeroConf. HWSD enables auto-configuration of
ad-hoc GPU clusters and multi-GPU machines.

HWSD 1.0 is a feature release based on GPU-SD 1.4. It can be retrieved by
downloading the
[source code](http://www.equalizergraphics.com/gpu-sd/downloads/hwsd-1.0.0.tar.gz)
or one of the
[binary distributions](http://www.equalizergraphics.com/gpu-sd/downloads.html).

## Features {#Features}

HWSD provides the following features to enable GPU and network device discovery:

* Local Discovery: Automatic network device and GPU discovery for Linux
  (X11/GLX), Mac OS X (CGL, GLX) and Windows (WGL, WGL_NV_gpu_affinity,
  WGL_AMD_gpu_association).
* Network announcement: A daemon for network announcement of available
  devices and their configuration using DNS service discovery and ZeroConf
  networking.
* Remote Discovery: Remote GPU discovery of resources announced using
  the HWSD daemon.

# New in this release {#New}

HWSD 1.0 is a feature release and contains the following features,
enhancements, bug fixes and documentation changes over the gpusd 1.4 release:

## New Features {#NewFeatures}

* [gpusd#5:](https://github.com/Eyescale/gpusd/issues/5) net_dns_sd:
  discovery module for remote zeroconf (Bonjour) network interfaces
* [gpusd#5:](https://github.com/Eyescale/gpusd/issues/5) net_sys:
  discovery of local network interfaces

## Enhancements {#Enhancements}

* [gpusd#9:](https://github.com/Eyescale/gpusd/issues/9) Daemon support
  for hw_sd application

## Optimizations {#Optimizations}

* No optimizations

## Tools {#Tools}

* New hw_sd daemon announcing local GPUs and network interfaces
* New hw_sd_list tool listing remote GPUs and network interfaces

## Documentation {#Documentation}

The following documentation has been added or substantially improved
since the last release:

* Documentation for the public
  [hwsd API](http://eyescale.github.io/hwsd-1.0/index.html).

## Bug Fixes {#Fixes}

HWSD 1.0 includes various bugfixes over the gpusd 1.4 release,
including the following:

* [4:](https://github.com/Eyescale/hwsd/issues/4) Bug in regex GPU filter

## Known Bugs {#Bugs}

The following bugs were known at release time. Please file a [Bug Report](https://github.com/Eyescale/Collage/issues) if you find any other issue with this release.

* No known bugs

# About {#About}

HWSD is a cross-platform library, designed to run on any modern
operating system, including all Unix variants and the Windows operating
system.  Zeroconf support in Lunchbox is required for the DNS_SD module
and applications.

HWSD uses CMake and [Buildyard](https://github.com/Eyescale/Buildyard)
to create a platform-specific build environment. The following platforms
and build environments are tested:

* Linux: Ubuntu 12.04, 12.10, 13.04, RHEL 6.3 (Makefile, i386, x64)
* Windows: 7 (Visual Studio 2008, i386, x64)
* Mac OS X: 10.8 (Makefile, i386, x64)

Technical questions can be posted to the eq-dev Mailing List, or
directly to info@equalizergraphics.com.

Commercial support, custom software development and porting services are
available from [Eyescale](http://www.eyescale.ch). Please contact
[info@eyescale.ch](mailto:info@eyescale.ch?subject=Collage%20support)
for further information.

# Errata
