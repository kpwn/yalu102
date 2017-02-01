//
//  patchfinder64.h
//
//  Created by qwertyoruiop on 21/09/16.
//  Copyright © 2016 qwertyoruiop. All rights reserved.
//

#pragma once

#import <Foundation/Foundation.h>

extern uint64_t text_exec_base;
extern uint64_t text_exec_size;

extern uint64_t prelink_exec_base;
extern uint64_t prelink_exec_size;

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
uint64_t get_data_for_mode(uint64_t offset, enum Search mode);

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
