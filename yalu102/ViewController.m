//
//  ViewController.m
//  yalu102
//
//  Created by qwertyoruiop on 05/01/2017.
//  Copyright © 2017 kimjongcracks. All rights reserved.
//

#import "ViewController.h"
#import "YAExploitTFP0.h"
#import "YAKernelOffsets.h"
#import "jailbreak.h"
#import <sys/utsname.h>

@interface ViewController ()

@property (nonatomic, strong) YAKernelOffsets *offsets;
@property (nonatomic, weak) IBOutlet UIButton *dope;
@property (nonatomic, weak) IBOutlet UIView *sshView;
@property (nonatomic, weak) IBOutlet UISwitch *sshSwitch;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    struct utsname u = { 0 };
    uname(&u);
    if (strstr(u.version, "MarijuanARM")) {
        [self.dope setEnabled:NO];
        [self.dope setTitle:@"already jailbroken" forState:UIControlStateDisabled];
        return;
    }

    self.offsets = [YAKernelOffsets offsetsForCurrentDevice];
    if (!self.offsets) {
        [self.dope setEnabled:NO];
        [self.dope setTitle:@"kernel unsupported" forState:UIControlStateDisabled];
        return;
    }

    // load settings
    {
        NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
        [self.sshSwitch setOn:[defaults boolForKey:@"YAEnableRemoteSSH"]];
        [self.sshView setHidden:NO];
    }
}

- (IBAction)yolo:(UIButton*)sender {
    mach_port_t _tfp0;
    uint64_t _kernbase;
    BOOL ok = [YAExploitTFP0 getTaskForPid0WithOffsets:self.offsets
                                        taskPortResult:&_tfp0
                                      kernelBaseResult:&_kernbase];
    if (ok) {
        // store settings
        {
            NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
            [defaults setBool:self.sshSwitch.on forKey:@"YAEnableRemoteSSH"];
            [defaults synchronize];
        }

        // via jailbreak.h
        tfp0 = _tfp0;
        kernbase = _kernbase;
        slide = kernbase - 0xFFFFFFF007004000;
        allprocs_offset = self.offsets->allproc_offset;
        rootvnode_offset = self.offsets->rootvnode_offset;
        cfg_enable_remote_ssh = self.sshSwitch.on;
        jailbreak();

        [self.dope setEnabled:NO];
        [self.dope setTitle:@"already jailbroken" forState:UIControlStateDisabled];
        [self.sshView setHidden:YES];
    }
    else {
        [self.dope setTitle:@"failed, retry?" forState:UIControlStateNormal];
    }
}

@end
