
#pragma once

#import <stdint.h>
#import <mach/mach.h>

void exploit(mach_port_t pt, uint64_t kernbase, uint64_t allprocs);
