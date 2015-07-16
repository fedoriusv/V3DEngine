//
//  WindowsDelegate.m
//  Valera3D
//
//  Created by fedoriusv on 7/12/15.
//
//

#import "WindowsDelegate.h"
#import "WindowMacOSX.h"
#include "Engine.h"

using namespace v3d;
using namespace platform;

/////////////////////////////////////////////////////////////////////////////////////////////////////

@implementation WindowsDelegate

- (id)initWithWindow:(WindowNS *)initWindow
{
    self = [super init];
    if (self != nil)
    {
        window = initWindow;
    }
    
    return self;
}

- (BOOL)windowShouldClose:(id)sender
{
    const PlatformPtr& platform = CEngine::getInstance()->getPlatform();
    platform->closeWindow();
    
    return NO;
}

- (void)windowDidResize:(NSNotification *)notification
{
}

- (void)windowDidMove:(NSNotification *)notification
{
}

- (void)windowDidMiniaturize:(NSNotification *)notification
{
}

- (void)windowDidDeminiaturize:(NSNotification *)notification
{
}

- (void)windowDidBecomeKey:(NSNotification *)notification
{
}

- (void)windowDidResignKey:(NSNotification *)notification
{
}

@end

/////////////////////////////////////////////////////////////////////////////////////////////////////

@implementation ContentView

- (id)initWithWindow:(WindowNS *)initWindow
{
    self = [super init];
    if (self != nil)
    {
        window = initWindow;
        trackingArea = nil;
        
        [self updateTrackingAreas];
        [self registerForDraggedTypes:[NSArray arrayWithObjects:
                                       NSFilenamesPboardType, nil]];
    }
    
    return self;
}

-(void)dealloc
{
    [trackingArea release];
    [super dealloc];
}

- (BOOL)isOpaque
{
    return YES;
}

- (BOOL)canBecomeKeyView
{
    return YES;
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (void)cursorUpdate:(NSEvent *)event
{
}

- (void)mouseDown:(NSEvent *)event
{
}

- (void)mouseDragged:(NSEvent *)event
{
    [self mouseMoved:event];
}

- (void)mouseUp:(NSEvent *)event
{
}

- (void)mouseMoved:(NSEvent *)event
{
}

- (void)rightMouseDown:(NSEvent *)event
{
}

- (void)rightMouseDragged:(NSEvent *)event
{
    [self mouseMoved:event];
}

- (void)rightMouseUp:(NSEvent *)event
{
}

- (void)otherMouseDown:(NSEvent *)event
{
}

- (void)otherMouseDragged:(NSEvent *)event
{
    [self mouseMoved:event];
}

- (void)otherMouseUp:(NSEvent *)event
{
}

- (void)mouseExited:(NSEvent *)event
{
}

- (void)mouseEntered:(NSEvent *)event
{
}

- (void)viewDidChangeBackingProperties
{
}

- (void)updateTrackingAreas
{
    if (trackingArea != nil)
    {
        [self removeTrackingArea:trackingArea];
        [trackingArea release];
    }
    
    const NSTrackingAreaOptions options = NSTrackingMouseEnteredAndExited |
    NSTrackingActiveInKeyWindow |
    NSTrackingEnabledDuringMouseDrag |
    NSTrackingCursorUpdate |
    NSTrackingInVisibleRect |
    NSTrackingAssumeInside;
    
    trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds]
                                                options:options
                                                  owner:self
                                               userInfo:nil];
    
    [self addTrackingArea:trackingArea];
    [super updateTrackingAreas];
}

- (void)keyDown:(NSEvent *)event
{
}

- (void)flagsChanged:(NSEvent *)event
{
}

- (void)keyUp:(NSEvent *)event
{
}

- (void)scrollWheel:(NSEvent *)event
{
    f64 deltaX, deltaY;
    
    if (floor(NSAppKitVersionNumber) > NSAppKitVersionNumber10_6)
    {
        deltaX = [event scrollingDeltaX];
        deltaY = [event scrollingDeltaY];
        
        if ([event hasPreciseScrollingDeltas])
        {
            deltaX *= 0.1;
            deltaY *= 0.1;
        }
    }
    else
    {
        deltaX = [event deltaX];
        deltaY = [event deltaY];
    }
    
    if (fabs(deltaX) > 0.0 || fabs(deltaY) > 0.0)
    {
        //TODO:
    }
}

- (NSDragOperation)draggingEntered:(id <NSDraggingInfo>)sender
{
    if ((NSDragOperationGeneric & [sender draggingSourceOperationMask]) == NSDragOperationGeneric)
    {
        [self setNeedsDisplay:YES];
        return NSDragOperationGeneric;
    }
    
    return NSDragOperationNone;
}

- (BOOL)prepareForDragOperation:(id <NSDraggingInfo>)sender
{
    [self setNeedsDisplay:YES];
    return YES;
}

- (BOOL)performDragOperation:(id <NSDraggingInfo>)sender
{
    return YES;
}

- (void)concludeDragOperation:(id <NSDraggingInfo>)sender
{
    [self setNeedsDisplay:YES];
}

@end

/////////////////////////////////////////////////////////////////////////////////////////////////////
