//
//  AppDelegate.h
//  Valera3D
//
//  Created by fedoriusv on 7/11/15.
//
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////

@interface AppDelegate : NSObject

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender;

- (void)applicationDidChangeScreenParameters:(NSNotification *) notification;
- (void)applicationDidFinishLaunching:(NSNotification *)notification;
- (void)applicationDidBecomeActive:(NSNotification *)notification;
- (void)applicationDidResignActive:(NSNotification *)notification;
- (void)applicationDidHide:(NSNotification *)notification;
- (void)applicationDidUnhide:(NSNotification *)notification;

@end

/////////////////////////////////////////////////////////////////////////////////////////////////////

@interface Application : NSApplication

- (void)sendEvent:(NSEvent *)event;

@end

/////////////////////////////////////////////////////////////////////////////////////////////////////