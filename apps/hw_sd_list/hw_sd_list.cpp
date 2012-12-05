
/*
  Copyright (c) 2011, Stefan Eilemann <eile@eyescale.ch>

  This file is part of the HW-SD discovery tool.

  The HW-SD discovery tool is free software: you can redistribute it and/or
  modify it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or (at your
  option) any later version.

  HW-SD is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with HW-SD. If not, see <http://www.gnu.org/licenses/>.
*/


#include <hwsd/hwsd.h>
#include <hwsd/gpuInfo.h>
#include <hwsd/gpu/dns_sd/module.h>

int main (int argc, const char * argv[])
{
    hwsd::gpu::dns_sd::Module::use();
    const hwsd::GPUInfos gpus = hwsd::discoverGPUs();
    for( hwsd::GPUInfosCIter i = gpus.begin(); i != gpus.end(); ++i )
        std::cout << *i << std::endl;

    return 0;
}
