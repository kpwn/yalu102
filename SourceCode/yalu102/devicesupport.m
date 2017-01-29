#import <Foundation/Foundation.h>
#import "devicesupport.h"
#import <sys/utsname.h>
#import <sys/sysctl.h>


NSMutableArray* consttable = nil;
NSMutableArray* collide = nil;
int constload() {
    struct utsname uts;
    uname(&uts);
    if (strstr(uts.version, "Marijuan")) {
        return -2;
    }

    NSString* strv = [NSString stringWithUTF8String:uts.version];
    NSArray *dp =[[NSArray alloc] initWithContentsOfFile: [[NSBundle mainBundle] pathForResource: @"def" ofType:@"plist"]];
    int m = 0;
    collide = [NSMutableArray new];
    
    for (NSDictionary* dict in dp) {
        if ([dict[@"vers"] isEqualToString:strv]) {
            [collide setObject:[NSMutableArray new] atIndexedSubscript:m];
            int i = 0;
            for (NSString* str in dict[@"val"]) {
                [collide[m] setObject:[NSNumber numberWithUnsignedLongLong:strtoull([str UTF8String], 0, 0)] atIndexedSubscript:i];
                i++;
            }
            m++;
        }
    }
    if (m) {
        return 0;
    }
    return -1;
}
char affine_const_by_surfacevt(uint64_t surfacevt_slid)
{
    for (NSArray* arr in collide) {
        if ((surfacevt_slid & (0xfffff)) == ([[arr objectAtIndex:1] unsignedLongLongValue] & 0xfffff)) {
            NSLog(@"affined");
            consttable = arr;
            return 0;
        }
    }
    return -1;
}
uint64_t constget(int idx){
    return [[consttable objectAtIndex:idx] unsignedLongLongValue];
}
