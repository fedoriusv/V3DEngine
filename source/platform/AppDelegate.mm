//
//  AppDelegate.m
//  Valera3D
//
//  Created by fedoriusv on 7/11/15.
//
//

#import "AppDelegate.h"
#include "Engine.h"

using namespace v3d;
using namespace platform;

/////////////////////////////////////////////////////////////////////////////////////////////////////

@implementation AppDelegate

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender
{
    const PlatformPtr& platform = CEngine::getInstance()->getPlatform();
    platform->closeWindow();
    
    return NSTerminateCancel;
}

- (void)applicationDidChangeScreenParameters:(NSNotification *) notification
{
}

- (void)applicationDidFinishLaunching:(NSNotification *)notification
{
    [NSApp stop:nil];
    
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    NSEvent* event = [NSEvent otherEventWithType:NSApplicationDefined
                                        location:NSMakePoint(0, 0)
                                   modifierFlags:0
                                       timestamp:0
                                    windowNumber:0
                                         context:nil
                                         subtype:0
                                           data1:0
                                           data2:0];
    [NSApp postEvent:event atStart:YES];
    [pool drain];
}

- (void)applicationDidBecomeActive:(NSNotification *)notification
{
}

- (void)applicationDidResignActive:(NSNotification *)notification
{
}

- (void)applicationDidHide:(NSNotification *)notification
{
}

- (void)applicationDidUnhide:(NSNotification *)notification
{
}

@end

/////////////////////////////////////////////////////////////////////////////////////////////////////

@implementation Application

- (void)sendEvent:(NSEvent *)event
{
    if ([event type] == NSKeyUp && ([event modifierFlags] & NSCommandKeyMask))
    {
        [[self keyWindow] sendEvent:event];
    }
    else
    {
        [super sendEvent:event];
    }
}

@end


/////////////////////////////////////////////////////////////////////////////////////////////////////
