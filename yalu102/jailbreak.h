
#pragma once

#import <stdbool.h>
#import <stdint.h>
#import <mach/mach.h>

extern mach_port_t tfp0;
extern uint64_t kernbase;
extern uint64_t slide;
extern uint64_t allprocs_offset;
extern uint64_t rootvnode_offset;
extern bool cfg_enable_remote_ssh;

void jailbreak(void);
