//
//  ViewController.h
//  yalu102
//
//  Created by qwertyoruiop on 05/01/2017.
//  Copyright © 2017 kimjongcracks. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <CommonCrypto/CommonDigest.h>

#include <stdlib.h>
#include <string.h>

@interface ViewController : UIViewController
{
    IBOutlet UIButton* dope;
    IBOutlet UILabel *trackLabel;
    
    AVPlayer *audioPlayer;
    
    NSMutableArray *tracks;
    NSInteger trackIndex;
    NSInteger maxTracks;
}
- (IBAction)yolo:(id)sender;

@end

