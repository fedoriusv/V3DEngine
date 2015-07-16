//
//  WindowsDelegate.h
//  Valera3D
//
//  Created by fedoriusv on 7/12/15.
//
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

namespace v3d
{
namespace platform
{
    typedef struct SWindowNS WindowNS;
}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

@interface WindowsDelegate : NSObject
{
    v3d::platform::WindowNS* window;
}

- (id)initWithWindow:(v3d::platform::WindowNS*)initWindow;

- (BOOL)windowShouldClose:(id)sender;
- (void)windowDidResize:(NSNotification *)notification;
- (void)windowDidMove:(NSNotification *)notification;
- (void)windowDidMiniaturize:(NSNotification *)notification;
- (void)windowDidDeminiaturize:(NSNotification *)notification;
- (void)windowDidBecomeKey:(NSNotification *)notification;
- (void)windowDidResignKey:(NSNotification *)notification;

@end

/////////////////////////////////////////////////////////////////////////////////////////////////////

@interface ContentView : NSView
{
    v3d::platform::WindowNS*    window;
    NSTrackingArea*             trackingArea;
}

- (id)initWithWindow:(v3d::platform::WindowNS*)initWindow;
- (void)dealloc;
- (BOOL)isOpaque;
- (BOOL)canBecomeKeyView;
- (BOOL)acceptsFirstResponder;
- (void)cursorUpdate:(NSEvent *)event;
- (void)mouseDown:(NSEvent *)event;
- (void)mouseDragged:(NSEvent *)event;
- (void)mouseUp:(NSEvent *)event;
- (void)mouseMoved:(NSEvent *)event;
- (void)rightMouseDown:(NSEvent *)event;
- (void)rightMouseDragged:(NSEvent *)event;
- (void)rightMouseUp:(NSEvent *)event;
- (void)otherMouseDown:(NSEvent *)event;
- (void)otherMouseDragged:(NSEvent *)event;
- (void)otherMouseUp:(NSEvent *)event;
- (void)mouseExited:(NSEvent *)event;
- (void)mouseEntered:(NSEvent *)event;
- (void)viewDidChangeBackingProperties;
- (void)updateTrackingAreas;
- (void)keyDown:(NSEvent *)event;
- (void)flagsChanged:(NSEvent *)event;
- (void)keyUp:(NSEvent *)event;
- (void)scrollWheel:(NSEvent *)event;
- (NSDragOperation)draggingEntered:(id <NSDraggingInfo>)sender;
- (BOOL)prepareForDragOperation:(id <NSDraggingInfo>)sender;
- (BOOL)performDragOperation:(id <NSDraggingInfo>)sender;
- (void)concludeDragOperation:(id <NSDraggingInfo>)sender;

@end

/////////////////////////////////////////////////////////////////////////////////////////////////////

