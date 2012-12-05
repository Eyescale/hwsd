
/* Copyright (c) 2011, Stefan Eilemann <eile@eyescale.ch>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 2.1 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef HWSD_VERSION_H
#define HWSD_VERSION_H

#include <string>

/**
 * @namespace hwsd
 * @brief The HW-SD discovery library
 */
namespace hwsd
{
/** The current major version. */
#   define HWSD_VERSION_MAJOR @VERSION_MAJOR@

/** The current minor version. */
#   define HWSD_VERSION_MINOR @VERSION_MINOR@

/** The current patch level. */
#   define HWSD_VERSION_PATCH @VERSION_PATCH@

/** True if the current version is newer than the given one. */
#   define HWSD_VERSION_GT( MAJOR, MINOR, PATCH )                      \
    ( (HWSD_VERSION_MAJOR>MAJOR) ||                                    \
      (HWSD_VERSION_MAJOR==MAJOR && (HWSD_VERSION_MINOR>MINOR ||      \
                                      (HWSD_VERSION_MINOR==MINOR &&    \
                                       HWSD_VERSION_PATCH>PATCH))))

/** True if the current version is equal or newer to the given. */
#   define HWSD_VERSION_GE( MAJOR, MINOR, PATCH )                      \
    ( (HWSD_VERSION_MAJOR>MAJOR) ||                                    \
      (HWSD_VERSION_MAJOR==MAJOR && (HWSD_VERSION_MINOR>MINOR ||      \
                                      (HWSD_VERSION_MINOR==MINOR &&    \
                                       HWSD_VERSION_PATCH>=PATCH))))

/** True if the current version is older than the given one. */
#   define HWSD_VERSION_LT( MAJOR, MINOR, PATCH )                      \
    ( (HWSD_VERSION_MAJOR<MAJOR) ||                                    \
      (HWSD_VERSION_MAJOR==MAJOR && (HWSD_VERSION_MINOR<MINOR ||      \
                                      (HWSD_VERSION_MINOR==MINOR &&    \
                                       HWSD_VERSION_PATCH<PATCH))))

/** True if the current version is older or equal to the given. */
#   define HWSD_VERSION_LE( MAJOR, MINOR, PATCH )                      \
    ( (HWSD_VERSION_MAJOR<MAJOR) ||                                    \
      (HWSD_VERSION_MAJOR==MAJOR && (HWSD_VERSION_MINOR<MINOR ||      \
                                      (HWSD_VERSION_MINOR==MINOR &&    \
                                       HWSD_VERSION_PATCH<=PATCH))))

/** Information about the current HW-SD version. */
class Version
{
public:
    /** @return the current major version of HW-SD. */
    static int getMajor();

    /** @return the current minor version of HW-SD. */
    static int getMinor();

    /** @return the current patch level of HW-SD. */
    static int getPatch();

    /** @return the current HW-SD version (MM.mm.pp). */
    static std::string getString();
};

}

#endif // HWSD_VERSION_H
