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
        allproc_offset = 0x5a4148;     /* SoC Processor used in the: iPhone 6s, iPhone 6s Plus, iPhone SE */
        procoff = 0x360;
        rootvnode_offset = 0x5aa0b8;   /* SoC Processor used in the: iPhone 6s, iPhone 6s Plus, iPhone SE */
    } else if (strcmp(u.version, "Darwin Kernel Version 16.3.0: Tue Nov 29 21:40:09 PST 2016; root:xnu-3789.32.1~4/RELEASE_ARM64_S8000") == 0) {
        allproc_offset = 0x5a8438;     /* Specifically the offsets for: iPhone 6s */
        procoff = 0x360;
        rootvnode_offset = 0x5ae0b8;   /* Specifically the offsets for: iPhone 6s */
    } else if (strcmp(u.version, "Darwin Kernel Version 16.3.0: Tue Nov 29 21:40:08 PST 2016; root:xnu-3789.32.1~4/RELEASE_ARM64_T7000") == 0) {
        allproc_offset = 0x5b8468;     /* Specifically the offsets for: iPhone 6 */
        procoff = 0x360;               /* Offsets found by: @Andywiik */
        rootvnode_offset = 0x5be0b8;   /* Specifically the offsets for: iPhone 6 */
    } else if (strcmp(u.version, "Darwin Kernel Version 16.3.0: Tue Nov 29 21:40:09 PST 2016; root:xnu-3789.32.1~4/RELEASE_ARM64_T7001") == 0) {
        allproc_offset = 0x5b8528;     /* Specifically the offsets for: iPad Air 2 (Wifi) */
        procoff = 0x360;               /* Offsets found by: @nicogibbons (github) */
        rootvnode_offset = 0x5be0b8;   /* Specifically the offsets for: iPad Air 2 (Wifi) */
    } else if (strcmp(u.version, "Darwin Kernel Version 16.3.0: Tue Nov 29 21:40:09 PST 2016; root:xnu-3789.32.1~4/RELEASE_ARM64_S5L8960X") == 0) {
        allproc_offset = 0x5ac418;     /* Specifically the offsets for: iPhone 5s */
        procoff = 0x360;               /* Offsets found by: @jtv7 (github) */
        rootvnode_offset = 0x5b60b8;   /* Specifically the offsets for: iPhone 5s */
    } else if (strcmp(u.version, "Darwin Kernel Version 16.3.0: Tue Nov 29 21:40:08 PST 2016; root:xnu-3789.32.1~4/RELEASE_ARM64_T8010") == 0) {
        allproc_offset = 0x5ec478;     /* SoC Processor used in the: iPhone 7, iPhone 7+ */
        procoff = 0x360;               /* Offsets found by: @Mila432 */
        rootvnode_offset = 0x5f20b8;   /* SoC Processor used in the: iPhone 7, iPhone 7+ */
    } else if (strstr(u.machine, "AppleTV5,3") && strstr(u.version, "root:xnu-3789.22.3~1/RELEASE_ARM64_S5L8960X")) { 
        allproc_offset = 0x5b8168;     /* Specifically the offsets for: Apple TV 4th Gen */
        procoff = 0x360;               /* Offsets found by: @onchehuh (github) */
        rootvnode_offset = 0x5ba0b8;   /* Specifically the offsets for: Apple TV 4th Gen */
    }  else if (strcmp(u.version, "Darwin Kernel Version 16.0.0: Fri Aug  5 22:15:30 PDT 2016; root:xnu-3789.1.24~11/RELEASE_ARM64_S5L8960X") == 0){
        allproc_offset = 0x5a4128;     /* SoC Processor used in the: iPhone 5s, iPad Mini 2, iPad Mini 3 */
        procoff = 0x360;
        rootvnode_offset = 0x5aa0b8;   /* SoC Processor used in the: iPhone 5s, iPad Mini 2, iPad Mini 3 */
    } else if (strcmp(u.version, "Darwin Kernel Version 16.1.0: Thu Sep 29 21:56:10 PDT 2016; root:xnu-3789.22.3~1/RELEASE_ARM64_T8010") == 0) {
        allproc_offset = 0x5ec178;     /* SoC Processor used in the: iPhone 7, iPhone 7+ */
        procoff = 0x360;               /* Offsets found by: @Mila432 */
        rootvnode_offset = 0x5f20b8;   /* SoC Processor used in the: iPhone 7, iPhone 7+ */
    } else if (strcmp(u.version, "Darwin Kernel Version 16.1.0: Thu Sep 29 21:56:12 PDT 2016; root:xnu-3789.22.3~1/RELEASE_ARM64_T7001") == 0) {
        allproc_offset = 0x5b4228;     /* SoC Processor used in the: iPad Air 2 */
        procoff = 0x360;               /* Offsets found by: @Mila432 */
        rootvnode_offset = 0x5ba0b8;   /* SoC Processor used in the: iPad Air 2 */
    } else if (strcmp(u.version, "Darwin Kernel Version 16.1.0: Thu Sep 29 21:56:11 PDT 2016; root:xnu-3789.22.3~1/RELEASE_ARM64_T7000") == 0) {
        allproc_offset = 0x5b4168;     /* SoC Processor used in the: Apple TV 4G, iPad Mini 4, iPhone 6, iPhone 6+, iPod Touch 6G */
        procoff = 0x360;               /* Offsets found by: @Mila432 */
        rootvnode_offset = 0x5ba0b8;   /* SoC Processor used in the: Apple TV 4G, iPad Mini 4, iPhone 6, iPhone 6+, iPod Touch 6G */
    } else if (strcmp(u.version, "Darwin Kernel Version 16.1.0: Thu Sep 29 21:56:12 PDT 2016; root:xnu-3789.22.3~1/RELEASE_ARM64_S8000") == 0) {
        allproc_offset = 0x5a4148;     /* SoC Processor used in the: iPhone 6s, iPhone 6s Plus, iPhone SE */
        procoff = 0x360;               /* Offsets found by: @Mila432 */
        rootvnode_offset = 0x5aa0b8;   /* SoC Processor used in the: iPhone 6s, iPhone 6s Plus, iPhone SE */
    } else if (strcmp(u.version, "Darwin Kernel Version 16.1.0: Thu Sep 29 21:56:11 PDT 2016; root:xnu-3789.22.3~1/RELEASE_ARM64_S5L8960X") == 0) {
        allproc_offset = 0x5a4128;     /* SoC Processor used in the: iPhone 5s, iPad Mini 2, iPad Mini 3 */
        procoff = 0x360;               /* Offsets found by @Mila432 */
        rootvnode_offset = 0x5aa0b8;   /* SoC Processor used in the: iPhone 5s, iPad Mini 2, iPad Mini 3 */
    } else if (strcmp(u.version, "Darwin Kernel Version 16.0.0: Sun Aug 28 20:36:54 PDT 2016; root:xnu-3789.2.4~3/RELEASE_ARM64_T8010") == 0) {
        allproc_offset = 0x5ec178;     /* SoC Processor used in the: iPhone 7, iPhone 7+ */
        procoff = 0x360;               /* Offsets found by @Mila432 */
        rootvnode_offset = 0x5f20b8;   /* SoC Processor used in the: iPhone 7, iPhone 7+ */
    } else if (strcmp(u.version, "Darwin Kernel Version 16.0.0: Sun Aug 28 20:36:54 PDT 2016; root:xnu-3789.2.4~3/RELEASE_ARM64_T7001") == 0) {
        allproc_offset = 0x5b0228;     /* SoC Processor used in the: iPad Air 2 */
        procoff = 0x360;               /* Offsets found by: @Mila432 */
        rootvnode_offset = 0x5b60b8;   /* SoC Processor used in the: iPad Air 2 */
    } else if (strcmp(u.version, "Darwin Kernel Version 16.0.0: Sun Aug 28 20:36:55 PDT 2016; root:xnu-3789.2.4~3/RELEASE_ARM64_T7000") == 0) {
        allproc_offset = 0x5b0168;     /* SoC Processor used in the: Apple TV 4G, iPad Mini 4, iPhone 6, iPhone 6+, iPod Touch 6G */
        procoff = 0x360;               /* Offsets found by: @Mila432 */
        rootvnode_offset = 0x5b60b8;   /* SoC Processor used in the: Apple TV 4G, iPad Mini 4, iPhone 6, iPhone 6+, iPod Touch 6G */
    } else if (strcmp(u.version, "Darwin Kernel Version 16.0.0: Sun Aug 28 20:36:54 PDT 2016; root:xnu-3789.2.4~3/RELEASE_ARM64_S8000") == 0) {
        allproc_offset = 0x5a4148;     /* SoC Processor used in the: iPhone 6s, iPhone 6s Plus, iPhone SE */
        procoff = 0x360;               /* Offsets found by: @Mila432 */
        rootvnode_offset = 0x5aa0b8;   /* SoC Processor used in the: iPhone 6s, iPhone 6s Plus, iPhone SE */
    } else if (strcmp(u.version, "Darwin Kernel Version 16.0.0: Sun Aug 28 20:36:55 PDT 2016; root:xnu-3789.2.4~3/RELEASE_ARM64_S5L8960X") == 0) {
        allproc_offset = 0x5a4128;     /* SoC Processor used in the: iPhone 5s, iPad Mini 2, iPad Mini 3 */
        procoff = 0x360;               /* Offsets found by: @Mila432) */
        rootvnode_offset = 0x5aa0b8;   /* SoC Processor used in the: iPhone 5s, iPad Mini 2, iPad Mini 3 */
    } else {
        printf("missing offset, prob crashing\n");
    }

}
