Release Notes {#mainpage}
============

[TOC]

# Introduction {#Introduction}

Welcome to HWSD, a daemon and library for the discovery and announcement
of hardware resources using ZeroConf. HWSD enables auto-configuration of
ad-hoc GPU clusters and multi-GPU machines.

HWSD 1.1 is a code cleanup release based on HWSD 1.0. It can be retrieved by
downloading the
[source code](https://github.com/Eyescale/hwsd/archive/1.1.0.tar.gz)
or one of the
[binary distributions](https://launchpad.net/~bbp/+archive/ppa).

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

HWSD 1.1 contains the following features, enhancements, bug fixes and
documentation changes over the HWSD 1.0 release:

## New Features {#NewFeatures}

* No new features

## Enhancements {#Enhancements}

* Allow filtering of unknown connections
* Adapted to latest Lunchbox code base

## Optimizations {#Optimizations}

* Reduce mdns browse time to 50ms

## Tools {#Tools}

* No new tools

## Documentation {#Documentation}

The following documentation has been upated since the last release:

* Documentation for the public
  [hwsd API](http://eyescale.github.io/hwsd-1.1/index.html).

## Bug Fixes {#Fixes}

HWSD 1.1 does not include any bugfixes over the HWSD 1.0 release.

## Known Bugs {#Bugs}

No bugs were known at release time. Please file a
[Bug Report](https://github.com/Eyescale/hwsd/issues)
if you find any issue with this release.

# About {#About}

HWSD is a cross-platform library, designed to run on any modern
operating system, including all Unix variants and the Windows operating
system.  Zeroconf support in Lunchbox is required for the DNS_SD module
and applications.

HWSD uses CMake and [Buildyard](https://github.com/Eyescale/Buildyard)
to create a platform-specific build environment. The following platforms
and build environments are tested:

* Linux: Ubuntu 14.04, RHEL 6.5 (Makefile, i386, x64)
* Windows: 7 (Visual Studio 2008, i386, x64)
* Mac OS X: 10.8 (Makefile, i386, x64)

Technical questions can be posted to the eq-dev Mailing List, or
directly to info@equalizergraphics.com.

Commercial support, custom software development and porting services are
available from [Eyescale](http://www.eyescale.ch). Please contact
[info@eyescale.ch](mailto:info@eyescale.ch?subject=HWSD%20support)
for further information.

# Errata
