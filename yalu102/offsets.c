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
	
	// reorderd by codexx420, also credits to https://www.theiphonewiki.com/wiki/Kernel#Version_List_.28iOS.29 
	if (strcmp(u.version, "Darwin Kernel Version 16.0.0: Sun Aug 28 20:36:55 PDT 2016; root:xnu-3789.2.4~3/RELEASE_ARM64_S5L8960X") == 0) { // A7 - 10.0.X
        	allproc_offset = 0x5a4128; /* @Mila432 */
        	procoff = 0x360;
		rootvnode_offset = 0x5aa0b8; /* @Mila432 */
	} else if (strcmp(u.version, "Darwin Kernel Version 16.1.0: Thu Sep 29 21:56:11 PDT 2016; root:xnu-3789.22.3~1/RELEASE_ARM64_S5L8960X") == 0) { // A7 - 10.1.X
      	  	allproc_offset = 0x5a4128; /* @Mila432 */
       		 procoff = 0x360;
       		rootvnode_offset = 0x5aa0b8; /* @Mila432 */
	} else if (strcmp(u.version, "Darwin Kernel Version 16.3.0: Tue Nov 29 21:40:09 PST 2016; root:xnu-3789.32.1~4/RELEASE_ARM64_S5L8960X") == 0) { // A7 - 10.2
        	allproc_offset = 0x5ac418;
        	procoff = 0x360; // iPhone 5s, 10.2, @jtv7 (github)
        	rootvnode_offset = 0x5b60b8;
	} else if (strcmp(u.version, "Darwin Kernel Version 16.0.0: Sun Aug 28 20:36:55 PDT 2016; root:xnu-3789.2.4~3/RELEASE_ARM64_T7000") == 0) { // A8 - 10.0.X
        	allproc_offset = 0x5b0168; /* @Mila432 */
        	procoff = 0x360;
        	rootvnode_offset = 0x5b60b8; /* @Mila432 */
	} else if (strcmp(u.version, "Darwin Kernel Version 16.1.0: Thu Sep 29 21:56:11 PDT 2016; root:xnu-3789.22.3~1/RELEASE_ARM64_T7000") == 0) { // A8 - 10.1.X
        	allproc_offset = 0x5b4168; /* @Mila432 */
        	procoff = 0x360;
        	rootvnode_offset = 0x5ba0b8; /* @Mila432 */
   	 } else if (strcmp(u.version, "Darwin Kernel Version 16.3.0: Tue Nov 29 21:40:08 PST 2016; root:xnu-3789.32.1~4/RELEASE_ARM64_T7000") == 0) { // A8 - 10.2
        	allproc_offset = 0x5b8468;
        	procoff = 0x360; // iPhone 6, 10.2, credit to @Andywiik
        	rootvnode_offset = 0x5be0b8;
	} else if (strcmp(u.version, "Darwin Kernel Version 16.0.0: Sun Aug 28 20:36:54 PDT 2016; root:xnu-3789.2.4~3/RELEASE_ARM64_T7001") == 0) { // A8X - 10.0.X
        	allproc_offset = 0x5b0228; /* @Mila432 */
        	procoff = 0x360;
        	rootvnode_offset = 0x5b60b8; /* @Mila432 */
    	} else if (strcmp(u.version, "Darwin Kernel Version 16.1.0: Thu Sep 29 21:56:12 PDT 2016; root:xnu-3789.22.3~1/RELEASE_ARM64_T7001") == 0) { // A8X - 10.1.X
        	allproc_offset = 0x5b4228; /* @Mila432 */
       	 	procoff = 0x360;
        	rootvnode_offset = 0x5ba0b8; /* @Mila432 */
	} else if (strcmp(u.version, "Darwin Kernel Version 16.3.0: Tue Nov 29 21:40:09 PST 2016; root:xnu-3789.32.1~4/RELEASE_ARM64_T7001") == 0) { // A8X - 10.2
        	allproc_offset = 0x5b8528;
        	procoff = 0x360; // iPad air 2 (wifi), 10.2, @nicogibbons (github)
        	rootvnode_offset = 0x5be0b8;
    	} else if (strcmp(u.version, "Darwin Kernel Version 16.0.0: Sun Aug 28 20:36:54 PDT 2016; root:xnu-3789.2.4~3/RELEASE_ARM64_S8000") == 0) { // A9 - 10.0.X
        	allproc_offset = 0x5a4148; /* @Mila432 */
        	procoff = 0x360;
        	rootvnode_offset = 0x5aa0b8; /* @Mila432 */
    	} else if (strcmp(u.version, "Darwin Kernel Version 16.1.0: Thu Sep 29 21:56:12 PDT 2016; root:xnu-3789.22.3~1/RELEASE_ARM64_S8000") == 0) { // A9 - 10.1.X
        	allproc_offset = 0x5a4148;
        	procoff = 0x360;
        	rootvnode_offset = 0x5aa0b8;
    	} else if (strcmp(u.version, "Darwin Kernel Version 16.3.0: Tue Nov 29 21:40:09 PST 2016; root:xnu-3789.32.1~4/RELEASE_ARM64_S8000") == 0) { // A9 - 10.2
        	allproc_offset = 0x5a8438;
        	procoff = 0x360; 
        	rootvnode_offset = 0x5ae0b8;
     	} else if (strcmp(u.version, "Darwin Kernel Version 16.0.0: Sun Aug 28 20:36:54 PDT 2016; root:xnu-3789.2.4~3/RELEASE_ARM64_T8010") == 0) { // A10 - 10.0.X
        	allproc_offset = 0x5ec178; /* @Mila432 */
        	procoff = 0x360;
        	rootvnode_offset = 0x5f20b8; /* @Mila432 */
    	} else if (strcmp(u.version, "Darwin Kernel Version 16.1.0: Thu Sep 29 21:56:10 PDT 2016; root:xnu-3789.22.3~1/RELEASE_ARM64_T8010") == 0) { // A10 - 10.1.X
        	allproc_offset = 0x5ec178; /* @Mila432 */
        	procoff = 0x360;
        	rootvnode_offset = 0x5f20b8; /* @Mila432 */
   	} else if (strcmp(u.version, "Darwin Kernel Version 16.3.0: Tue Nov 29 21:40:08 PST 2016; root:xnu-3789.32.1~4/RELEASE_ARM64_T8010") == 0) { // A10 - 10.2
        	allproc_offset = 0x5ec478; /* @Mila432 */
        	procoff = 0x360;
        	rootvnode_offset = 0x5f20b8; /* @Mila432 */
	 } else if (strstr(u.machine, "AppleTV5,3") && strstr(u.version, "root:xnu-3789.22.3~1/RELEASE_ARM64_S5L8960X")) { //Using Ian's condition, not having my AppleTV nearby
        	allproc_offset = 0x5b8168;
        	procoff = 0x360; // tvOS 10.0.1 (14U71) @onchehuh (github)
		rootvnode_offset = 0x5ba0b8;
	}  else if (strcmp(u.version, "Darwin Kernel Version 16.0.0: Fri Aug  5 22:15:30 PDT 2016; root:xnu-3789.1.24~11/RELEASE_ARM64_S5L8960X") == 0){ // This should be A7 with 10.0.0 Beta 5 - don't know why lol
        	allproc_offset = 0x5a4128;
        	procoff = 0x360;
		rootvnode_offset = 0x5aa0b8;
    	} else {
        	printf("missing offset, prob crashing\n");
    	}
}
