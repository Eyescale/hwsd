
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

#ifndef HWSD_API_H
#define HWSD_API_H

#if defined(_MSC_VER) || defined(__declspec)
#define HWSD_DLLEXPORT __declspec(dllexport)
#define HWSD_DLLIMPORT __declspec(dllimport)
#else
#define HWSD_DLLEXPORT
#define HWSD_DLLIMPORT
#endif

#if defined(HWSD_STATIC)
#define HWSD_API
#elif defined(HWSD_SHARED)
#define HWSD_API HWSD_DLLEXPORT
#else
#define HWSD_API HWSD_DLLIMPORT
#endif

#if defined(HWSD_SHARED_INL)
#define HWSD_INL HWSD_DLLEXPORT
#else
#define HWSD_INL HWSD_DLLIMPORT
#endif

#endif // HWSD_API_H
