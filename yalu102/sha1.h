//
//  sha1.h
//  yalu102
//
//  Created by Andrea Bentivegna on 29/01/17.
//  Copyright © 2017 kimjongcracks. All rights reserved.
//

#ifndef sha1_h
#define sha1_h

#import <Foundation/Foundation.h>
#include <CommonCrypto/CommonDigest.h>

@interface AiChecksum : NSObject
{
}

/**
 * Get the md5 hash of a file
 *
 * @returns		NSString
 * @since		20140120
 * @author		costmo
 * @param		path		Full path to the file
 */
+(NSString *)md5HashOfPath:(NSString *)path;

/**
 * Get the sha1 hash of a file
 *
 * @returns		NSString
 * @since		20140120
 * @author		costmo
 * @param		path		Full path to the file
 */
+(NSString *)shaHashOfPath:(NSString *)path;

@end

#endif /* sha1_h */
