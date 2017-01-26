#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/utsname.h>

#include "offsets.h"

uint64_t allproc_offset;
uint64_t kernproc_offset;
uint64_t procoff;
uint64_t rootvnode_offset;

void init_offsets() {
    struct utsname u = { 0 };
    uname(&u);
    
    printf("sysname: %s\n", u.sysname);
    printf("nodename: %s\n", u.nodename);
    printf("release: %s\n", u.release);
    printf("version: %s\n", u.version);
    printf("machine: %s\n", u.machine);
    
    allproc_offset = 0x5a4148;
    procoff = 0x360;
    rootvnode_offset = 0x5aa0b8;

    if (strcmp(u.version, "Darwin Kernel Version 16.1.0: Thu Sep 29 21:56:12 PDT 2016; root:xnu-3789.22.3~1/RELEASE_ARM64_S8000") == 0) {
        allproc_offset = 0x5a4148;
        procoff = 0x360;
        rootvnode_offset = 0x5aa0b8;
    } else if (strcmp(u.version, "Darwin Kernel Version 16.0.0: Sun Aug 28 20:36:54 PDT 2016; root:xnu-3789.2.4~3/RELEASE_ARM64_S8000") == 0) {
        allproc_offset = 0x5a4148;
        procoff = 0x360;
        rootvnode_offset = 0x5aa0b8; /* this one is wrong i think but no devices to test */
    } else if (strcmp(u.version, "Darwin Kernel Version 16.3.0: Tue Nov 29 21:40:09 PST 2016; root:xnu-3789.32.1~4/RELEASE_ARM64_S8000") == 0) {
        allproc_offset = 0x5a8438;
        procoff = 0x360; // iphone 6s
        rootvnode_offset = 0x5ae0b8;
    } else if (strcmp(u.version, "Darwin Kernel Version 16.3.0: Tue Nov 29 21:40:08 PST 2016; root:xnu-3789.32.1~4/RELEASE_ARM64_T7000") == 0) {
        allproc_offset = 0x5b8468;
        procoff = 0x360; // iphone 6, credit to @Andywiik
        rootvnode_offset = 0x5be0b8;
    } else if (strcmp(u.version, "Darwin Kernel Version 16.3.0: Tue Nov 29 21:40:09 PST 2016; root:xnu-3789.32.1~4/RELEASE_ARM64_S5L8960X") == 0) {
        allproc_offset = 0x5b20e0;
        procoff = 0x360; // iphone 5s, @Slonick (github)
        rootvnode_offset = 0x5b20b8;
    } else {
        printf("missing offset, prob crashing\n");
    }

}
