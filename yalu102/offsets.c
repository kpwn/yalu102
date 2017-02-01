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

    /*
    Offsets shared by:
    - @qwertyoruiop
    - @Mila432
    - @Andywiik
    - @nicogibbons
    - @jtv7
    - @onchehuh
    */

    const char *prefix = "Darwin Kernel Version 16.";
    const size_t prefix_len = 25; // strlen(prefix);
    if (strncmp(u.version, prefix, prefix_len) != 0)
        goto some_beta_ios;

    char *s = u.version + prefix_len;

    int supported_version = 0;

    const char vers[3][5] = { "3.0: ", "1.0: ", "0.0: ", };
    const int ver_prefix_len = 5;

    for(int i = 0; i < 3; i += 1) {
        if(strncmp(s, vers[i], ver_prefix_len) == 0) {
            supported_version = 1;
        }
    }

    if(supported_version == 0)
        goto some_beta_ios;

    s += ver_prefix_len;

    if (strcmp(s, "Tue Nov 29 21:40:09 PST 2016; root:xnu-3789.32.1~4/RELEASE_ARM64_T7001") == 0) {
        allproc_offset = 0x5b8528;
        rootvnode_offset = 0x5be0b8;
    }
    else if (strcmp(s, "Tue Nov 29 21:40:08 PST 2016; root:xnu-3789.32.1~4/RELEASE_ARM64_T7000") == 0) {
        allproc_offset = 0x5b8468;
        rootvnode_offset = 0x5be0b8;
    }
    else if (strcmp(s, "Tue Nov 29 21:40:09 PST 2016; root:xnu-3789.32.1~4/RELEASE_ARM64_S8000") == 0) {
        allproc_offset = 0x5a8438;
        rootvnode_offset = 0x5ae0b8;
    }
    else if (strcmp(s, "Tue Nov 29 21:40:09 PST 2016; root:xnu-3789.32.1~4/RELEASE_ARM64_S5L8960X") == 0) {
        allproc_offset = 0x5ac418;
        rootvnode_offset = 0x5b20b8;
    }
    else if (strcmp(s, "Tue Nov 29 21:40:08 PST 2016; root:xnu-3789.32.1~4/RELEASE_ARM64_T8010") == 0) {
        allproc_offset = 0x5ec478;
        rootvnode_offset = 0x5f20b8;
    }
    else if (strcmp(s, "Mon Dec 19 19:56:48 PST 2016; root:xnu-3789.43.2~1/RELEASE_ARM64_T7000") == 0) {
        allproc_offset = 0x5b8468;
        rootvnode_offset = 0x5be0b8;
    }//apple tv 4
    else if (strcmp(s, "Tue Nov 29 20:25:47 PST 2016; root:xnu-3789.33.1~1/RELEASE_ARM64_T7000") == 0) {
        allproc_offset = 0x5b8468;
        rootvnode_offset = 0x5be0b8;
    }//apple tv 4
    else if (strcmp(s, "Thu Sep 29 22:13:19 PDT 2016; root:xnu-3789.23.3~2/RELEASE_ARM64_T7000") == 0) {
        allproc_offset = 0x5b4168;
        rootvnode_offset = 0x5ba0b8;
    }//apple tv 4
    else if (strcmp(s, "Thu Sep 29 21:56:10 PDT 2016; root:xnu-3789.22.3~1/RELEASE_ARM64_T8010") == 0) {
        allproc_offset = 0x5ec178;
        rootvnode_offset = 0x5f20b8;
    }
    else if (strcmp(s, "Thu Sep 29 21:56:12 PDT 2016; root:xnu-3789.22.3~1/RELEASE_ARM64_T7001") == 0) {
        allproc_offset = 0x5b4228;
        rootvnode_offset = 0x5ba0b8;
    }
    else if (strcmp(s, "Thu Sep 29 21:56:11 PDT 2016; root:xnu-3789.22.3~1/RELEASE_ARM64_T7000") == 0) {
        allproc_offset = 0x5b4168;
        rootvnode_offset = 0x5ba0b8;
    }
    else if (strcmp(s, "Thu Sep 29 21:56:12 PDT 2016; root:xnu-3789.22.3~1/RELEASE_ARM64_S8000") == 0) {
        allproc_offset = 0x5a4148;
        rootvnode_offset = 0x5aa0b8;
    }
    else if (strcmp(s, "Thu Sep 29 21:56:11 PDT 2016; root:xnu-3789.22.3~1/RELEASE_ARM64_S5L8960X") == 0) {
        allproc_offset = 0x5a4128;
        rootvnode_offset = 0x5aa0b8;
    }
    else if (strcmp(s, "Sun Aug 28 20:36:54 PDT 2016; root:xnu-3789.2.4~3/RELEASE_ARM64_T8010") == 0) {
        allproc_offset = 0x5ec178;
        rootvnode_offset = 0x5f20b8;
    }
    else if (strcmp(s, "Sun Aug 28 20:36:54 PDT 2016; root:xnu-3789.2.4~3/RELEASE_ARM64_T7001") == 0) {
        allproc_offset = 0x5b0228;
        rootvnode_offset = 0x5b60b8;
    }
    else if (strcmp(s, "Sun Aug 28 20:36:55 PDT 2016; root:xnu-3789.2.4~3/RELEASE_ARM64_T7000") == 0) {
        allproc_offset = 0x5b0168;
        rootvnode_offset = 0x5b60b8;
    }
    else if (strcmp(s, "Sun Aug 28 20:36:54 PDT 2016; root:xnu-3789.2.4~3/RELEASE_ARM64_S8000") == 0) {
        allproc_offset = 0x5a4148;
        rootvnode_offset = 0x5aa0b8;
    }
    else if (strcmp(s, "Sun Aug 28 20:36:55 PDT 2016; root:xnu-3789.2.4~3/RELEASE_ARM64_S5L8960X") == 0) {
        allproc_offset = 0x5a4128;
        rootvnode_offset = 0x5aa0b8;
    }
    else if (strcmp(s, "Fri Aug  5 22:15:30 PDT 2016; root:xnu-3789.1.24~11/RELEASE_ARM64_S5L8960X") == 0) {
        allproc_offset = 0x5a4128;
        rootvnode_offset = 0x5aa0b8;
    }
    else {
        goto some_beta_ios;
    }

    return;

some_beta_ios:
    printf("missing offset, prob crashing\n");
}
