//
//  YAKernelOffsets.m
//  yalu102
//
//  Created by mologie on 29/01/2017.
//

#import "YAKernelOffsets.h"
#import <sys/utsname.h>

@implementation YAKernelOffsets

+ (instancetype)offsetsForCurrentDevice {
    struct utsname u = { 0 };
    uname(&u);
    return [self offsetsForKernelVersion:[NSString stringWithUTF8String:u.version]];
}

+ (instancetype)offsetsForKernelVersion:(NSString *)version {
    NSString *kernelsPlistFile = [[NSBundle bundleForClass:self.class] pathForResource:@"Kernels" ofType:@"plist"];
    NSDictionary *kernels = [NSDictionary dictionaryWithContentsOfFile:kernelsPlistFile];
    NSDictionary *kernel = kernels[version];
    if (kernel) {
        YAKernelOffsets *offsets = [[YAKernelOffsets alloc] init];
        offsets->allproc_offset = [self parseOffset:kernel[@"allproc"]];
        offsets->procoff = 0x360;
        offsets->rootvnode_offset = [self parseOffset:kernel[@"rootvnode"]];
        return offsets;
    } else {
        return nil;
    }
}

+ (uint64_t)parseOffset:(NSString *)offset {
    NSScanner* scanner = [NSScanner scannerWithString:offset];
    if ([offset hasPrefix:@"0x"])
        scanner.scanLocation = 2;
    uint64_t result = 0;
    [scanner scanHexLongLong:&result];
    return result;
}

@end
