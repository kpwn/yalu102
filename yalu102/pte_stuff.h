#ifndef pte_stuff_h
#define pte_stuff_h


#define TTE_INDEX_SHIFT 3
#define TTE_SIZE (1 << TTE_INDEX_SHIFT)
#define TTE_INDEX(vma, level) ((vma.vm_info.level ## _index) << TTE_INDEX_SHIFT)
#define TTE_GET(tte, mask) (tte & mask)
#define TTE_SETB(tte, mask) tte = tte | mask
#define TTE_SET(tte, mask, val) tte = ((tte & (~mask)) | (val & mask))
#define TTE_IS_VALID_MASK 0x1
#define TTE_IS_TABLE_MASK 0x2
#define TTE_BLOCK_ATTR_INDEX_MASK 0x1C
#define TTE_BLOCK_ATTR_NS_MASK 0x20
#define TTE_BLOCK_ATTR_AP_MASK 0xC0
#define TTE_BLOCK_ATTR_AP_RO_MASK 0x80
#define TTE_BLOCK_ATTR_AP_CAN_EL0_ACCESS_MASK 0x40
#define TTE_BLOCK_ATTR_SH_MASK 0x300
#define TTE_BLOCK_ATTR_AF_MASK 0x400
#define TTE_BLOCK_ATTR_NG_MASK 0x800
#define TTE_PHYS_VALUE_MASK 0xFFFFFFFFF000ULL
#define TTE_BLOCK_ATTR_CONTIG_MASK (1ULL << 52)
#define TTE_BLOCK_ATTR_PXN_MASK (1ULL << 53)
#define TTE_BLOCK_ATTR_UXN_MASK (1ULL << 54)
#define VIRT_TO_PHYS(vaddr) (vaddr - gVirtBase + gPhysBase)
#define PHYS_TO_VIRT(paddr) (paddr - gPhysBase + gVirtBase)
uint64_t gPhysBase,gVirtBase,pmap_store,level1_table,hibit_guess;

typedef union VMA_4K {
    struct {
        uint64_t block_off : 12;
        uint64_t level3_index : 9;
        uint64_t level2_index : 9;
        uint64_t level1_index : 9;
        uint64_t level0_index : 9;
        uint64_t ttbr_selector : 16;
    } __attribute__((packed)) vm_info;
    uint64_t vmaddr;
} VMA_4K;
typedef union VMA_16K {
    struct {
        uint64_t block_off : 14;
        uint64_t level3_index : 11;
        uint64_t level2_index : 11;
        uint64_t level1_index : 11;
        uint64_t level0_index : 1;
        uint64_t ttbr_selector : 16;
    } __attribute__((packed)) vm_info;
    uint64_t vmaddr;
} VMA_16K;

VMA_16K vad16;
VMA_4K vad4;
char isvad = 0;

#define TTELog NSLog
vm_size_t sz = 0;

void checkvad() {
    if (!sz) {
        struct utsname u = { 0 };
        uname(&u);
        host_page_size(mach_host_self(), &sz);
        NSLog(@"checkvad: %x %x", sz, getpagesize());
        if (strstr(u.machine, "iPad5,") == u.machine) {
            sz = 4096; // this is 4k but host_page_size lies to us
        }
        assert(sz);
        if (sz == 4096) {
            isvad = 1;
        }
    }
}
void parse_block_tte(uint64_t tte) {
    TTELog(@"TTE physaddr: %016llx", TTE_GET(tte, TTE_PHYS_VALUE_MASK));
    TTELog(@"TTE table: %d", !!(TTE_GET(tte, TTE_IS_TABLE_MASK)));
    TTELog(@"TTE valid: %d", !!(TTE_GET(tte, TTE_IS_VALID_MASK)));
    TTELog(@"TTE userland eXecute Never: %d", !!(TTE_GET(tte, TTE_BLOCK_ATTR_UXN_MASK)));
    TTELog(@"TTE privileged eXecute Never: %d", !!(TTE_GET(tte, TTE_BLOCK_ATTR_PXN_MASK)));
    TTELog(@"TTE contiguous: %d", !!(TTE_GET(tte, TTE_BLOCK_ATTR_CONTIG_MASK)));
    TTELog(@"TTE not global: %d", !!(TTE_GET(tte, TTE_BLOCK_ATTR_NG_MASK)));
    TTELog(@"TTE access flag: %d", !!(TTE_GET(tte, TTE_BLOCK_ATTR_AF_MASK)));
    TTELog(@"TTE shareable: %01llx", (TTE_GET(tte, TTE_BLOCK_ATTR_SH_MASK) >> 8));
    TTELog(@"TTE access permissions: %s", (TTE_GET(tte, TTE_BLOCK_ATTR_AP_RO_MASK)) ? "Read only" : "Read/Write");
    TTELog(@"TTE access permissions: %s", (TTE_GET(tte, TTE_BLOCK_ATTR_AP_CAN_EL0_ACCESS_MASK)) ? "EL0 (Userland) can access this page" : "EL0 (Userland) cannot access this page");
}

void pagestuff_64(vm_address_t vmaddr, void (^pagestuff_64_callback)(vm_address_t tte_addr, int addr), vm_address_t table, int level) {
    
    checkvad();
    if (!table) table = level1_table;
    if (!level) level = 1;
    
    vm_address_t tteaddr = 0;
    
    
    
    if (sz == 4096) {
        VMA_4K target_addr;
        target_addr.vmaddr = vmaddr;

        if (level == 1) {
            target_addr.vm_info.level1_index -= 0x1c0;
        }

        switch (level) {
            case 0:
                tteaddr = table + TTE_INDEX(target_addr, level0);
                break;
            case 1:
                tteaddr = table + TTE_INDEX(target_addr, level1);
                break;
                
            case 2:
                tteaddr = table + TTE_INDEX(target_addr, level2);
                break;
                
            case 3:
                tteaddr = table + TTE_INDEX(target_addr, level3);
                break;
                
            default:
                break;
        }
        
    } else if (sz == 4096*4) {
        VMA_16K target_addr;
        target_addr.vmaddr = vmaddr;
        
        switch (level) {
            case 0:
                tteaddr = table + TTE_INDEX(target_addr, level0);
                break;
            case 1:
                tteaddr = table + TTE_INDEX(target_addr, level1);
                break;
                
            case 2:
                tteaddr = table + TTE_INDEX(target_addr, level2);
                break;
                
            case 3:
                tteaddr = table + TTE_INDEX(target_addr, level3);
                break;
                
            default:
                break;
        }
        
        
    }
    
    //parse_block_tte(level1_entry);
    
    pagestuff_64_callback(tteaddr, level);
    
    uint64_t level1_entry = ReadAnywhere64(tteaddr);
    
    if (TTE_GET(level1_entry, TTE_IS_TABLE_MASK) && level != 3) {
        pagestuff_64(vmaddr, pagestuff_64_callback, (TTE_GET(level1_entry, TTE_PHYS_VALUE_MASK)) - gPhysBase + gVirtBase, level + 1);
    }
}

uint64_t findphys_real(uint64_t virtaddr) {
    __block uint64_t physvar = 0;
    pagestuff_64(virtaddr, ^(vm_address_t tte_addr, int addr) {
        uint64_t tte = ReadAnywhere64(tte_addr);
        if (addr == 3) {\
            physvar = TTE_GET(tte, TTE_PHYS_VALUE_MASK);
        }
    }, level1_table, isvad ? 1 : 2);
    
    return physvar;
    
}
uint64_t physalloc(uint64_t size) {
    uint64_t ret = 0;
    mach_vm_allocate(tfp0, (mach_vm_address_t*) &ret, size, VM_FLAGS_ANYWHERE);
    return ret;
}

#endif
