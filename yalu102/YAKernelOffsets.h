//
//  YAKernelOffsets.h
//  yalu102
//
//  Created by mologie on 29/01/2017.
//

#import <Foundation/Foundation.h>
#import <stdint.h>

@interface YAKernelOffsets : NSObject {
@public
    uint64_t allproc_offset;
    uint64_t procoff;
    uint64_t rootvnode_offset;
}

+ (instancetype)offsetsForCurrentDevice;

@end
