//
//  AppDelegate.m
//  yalu102
//
//  Created by qwertyoruiop on 05/01/2017.
//  Copyright © 2017 kimjongcracks. All rights reserved.
//

#import "AppDelegate.h"

@interface AppDelegate ()

@end

@implementation AppDelegate
@synthesize shouldJailbreak = _shouldJailbreak;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    return YES;
}

- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url {
    // URL scheme handling
    NSString *urlParameter = [url host];
    if ([urlParameter isEqual:@"break"]) {
        // URL scheme to jailbreak is being handled
        UIAlertController *alertvc = [UIAlertController alertControllerWithTitle:@"Do you really want to jailbreak?" message:@"You used a URI scheme to break out of jail." preferredStyle:UIAlertControllerStyleAlert];
        UIAlertAction *actionOk = [UIAlertAction actionWithTitle:@"I want to jailbreak!" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
            NSLog(@"We're breaking out of jail bois!");
            _shouldJailbreak = YES;
            [[NSNotificationCenter defaultCenter] postNotificationName:@"ReevaluateShouldJailbreak" object:nil userInfo:nil];
        }];
        UIAlertAction* cancelAction = [UIAlertAction actionWithTitle:@"Cancel" style:UIAlertActionStyleDestructive handler:nil];
        [alertvc addAction:actionOk];
        [alertvc addAction:cancelAction];
        UIViewController *vc = self.window.rootViewController;
        [vc presentViewController:alertvc animated:YES completion:nil];
    }
    return YES;
}

- (void)application:(UIApplication *)application performActionForShortcutItem:(UIApplicationShortcutItem *)shortcutItem completionHandler:(void (^)(BOOL))completionHandler {
    // 3D Touch shortcut action handling
    NSString *bundleIdentifier = [[NSBundle mainBundle] bundleIdentifier];
    NSLog(@"%@", shortcutItem.type);
    if ([shortcutItem.type isEqual:[NSString stringWithFormat: @"%@.BREAK", bundleIdentifier]]) {
        // User has requested through 3D Touch to jailbreal
        NSLog(@"3D Touch shortcut action to jailbreak hit!");
        _shouldJailbreak = YES;
        [[NSNotificationCenter defaultCenter] postNotificationName: @"ReevaluateShouldJailbreak" object:nil userInfo:nil];
    }
}

- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and invalidate graphics rendering callbacks. Games should use this method to pause the game.
}


- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}


- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the active state; here you can undo many of the changes made on entering the background.
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}


- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

@end
