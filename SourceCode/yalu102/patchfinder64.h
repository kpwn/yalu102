//
//  patchfinder64.h
//
//  Created by qwertyoruiop on 21/09/16.
//  Copyright © 2016 qwertyoruiop. All rights reserved.
//

#ifndef patchfinder64_h
#define patchfinder64_h

#import <Foundation/Foundation.h>

extern uint64_t  text_exec_base;
extern uint64_t  text_exec_size;

extern uint64_t  prelink_exec_base;
extern uint64_t  prelink_exec_size;

extern uint64_t whole_base;
extern uint64_t whole_size;
extern uint8_t* whole_dump;
extern uint64_t find_register_value(uint32_t* opcodes, uint64_t offset, uint64_t opcode_base, uint8_t reg);
extern uint64_t find_reference(uint32_t* opcodes, size_t opcode_size, uint64_t opcode_base, uint64_t target);
enum Search {
    SearchTextExec,
    SearchPrelinkExec
};
extern char uref;
extern uint64_t get_data_for_mode(uint64_t offset, enum Search mode);



void set_text_exec(uint64_t text_exec_base_,uint64_t text_exec_size_);
void set_prelink_exec(uint64_t prelink_exec_base_, uint64_t prelink_exec_size_);
void set_dump(uint64_t whole_base_,uint64_t whole_size_, uint8_t* whole_dump_);
uint64_t find_kernel_pmap();
uint64_t find_gPhysBase();
uint64_t find_flushcache();
uint64_t find_amfi_memcmpstub();
uint64_t find_memwithphys();
uint64_t find_kernel_mount();
uint64_t find_vnode_lookup();
uint64_t find_cpu_list();
uint64_t find_cpacr_write();
uint64_t find_lwvm_mapio_patch();
uint64_t find_lwvm_mapio_newj();
uint64_t find_ret_non0();
uint64_t find_ret_0();
uint64_t find_amfiret();
uint64_t find_sbops();
#define	CS_VALID		0x0000001	/* dynamically valid */
#define CS_ADHOC		0x0000002	/* ad hoc signed */
#define CS_GET_TASK_ALLOW	0x0000004	/* has get-task-allow entitlement */
#define CS_INSTALLER		0x0000008	/* has installer entitlement */

#define	CS_HARD			0x0000100	/* don't load invalid pages */
#define	CS_KILL			0x0000200	/* kill process if it becomes invalid */
#define CS_CHECK_EXPIRATION	0x0000400	/* force expiration checking */
#define CS_RESTRICT		0x0000800	/* tell dyld to treat restricted */
#define CS_ENFORCEMENT		0x0001000	/* require enforcement */
#define CS_REQUIRE_LV		0x0002000	/* require library validation */
#define CS_ENTITLEMENTS_VALIDATED	0x0004000

#define	CS_ALLOWED_MACHO	0x00ffffe

#define CS_EXEC_SET_HARD	0x0100000	/* set CS_HARD on any exec'ed process */
#define CS_EXEC_SET_KILL	0x0200000	/* set CS_KILL on any exec'ed process */
#define CS_EXEC_SET_ENFORCEMENT	0x0400000	/* set CS_ENFORCEMENT on any exec'ed process */
#define CS_EXEC_SET_INSTALLER	0x0800000	/* set CS_INSTALLER on any exec'ed process */

#define CS_KILLED		0x1000000	/* was killed by kernel for invalidity */
#define CS_DYLD_PLATFORM	0x2000000	/* dyld used to load this is a platform binary */
#define CS_PLATFORM_BINARY	0x4000000	/* this is a platform binary */
#define CS_PLATFORM_PATH	0x8000000	/* platform binary by the fact of path (osx only) */
#endif /* patchfinder64_h */
