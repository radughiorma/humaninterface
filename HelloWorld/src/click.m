// File: 
// click.m
//
// Compile with: 
// gcc -o click click.m -framework ApplicationServices -framework Foundation
//
// Usage:
// ./click -x pixels -y pixels 
// At the given coordinates it will click and release.

#import <Foundation/Foundation.h>
#import <ApplicationServices/ApplicationServices.h>


int main(int argc, char *argv[]) {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  NSUserDefaults *args = [NSUserDefaults standardUserDefaults];


  // grabs command line arguments -x and -y
  //
  int x = [args integerForKey:@"x"];
  int y = [args integerForKey:@"y"];


  // The data structure CGPoint represents a point in a two-dimensional
  // coordinate system.  Here, X and Y distance from upper left, in pixels.
  //
  CGPoint pt;
  pt.x = x;
  pt.y = y;

  printf("x=%i y=%i\n", x, y);
  

  // This is where the magic happens.  See CGRemoteOperation.h for details.
  //
  // CGPostMouseEvent( CGPoint        mouseCursorPosition,
  //                   boolean_t      updateMouseCursorPosition,
  //                   CGButtonCount  buttonCount,
  //                   boolean_t      mouseButtonDown, ... )
  //
  // So, we feed coordinates to CGPostMouseEvent, put the mouse there,
  // then click and release.
  //
  // CGPostMouseEvent( pt, 1, 1, 1 );
  // CGPostMouseEvent( pt, 1, 1, 0 );



      // Move to 200x200
    CGEventRef move1 = CGEventCreateMouseEvent(
        NULL, kCGEventMouseMoved,
        CGPointMake(x, y),
        kCGMouseButtonLeft // ignored
    );
    // Move to 250x250
    // CGEventRef move2 = CGEventCreateMouseEvent(
    //     NULL, kCGEventMouseMoved,
    //     CGPointMake(x+250, y+250),
    //     kCGMouseButtonLeft // ignored
    // );

    //     // Move to 250x250
    // CGEventRef move2 = CGEventCreateMouseEvent(
    //     NULL, kCGEventMouseMoved,
    //     CGPointMake(x+150, y+350),
    //     kCGMouseButtonLeft // ignored
    // );

    // // Left button down at 250x250
    // CGEventRef click1_down = CGEventCreateMouseEvent(
    //     NULL, kCGEventLeftMouseDown,
    //     CGPointMake(250, 250),
    //     kCGMouseButtonLeft
    // );
    // // Left button up at 250x250
    // CGEventRef click1_up = CGEventCreateMouseEvent(
    //     NULL, kCGEventLeftMouseUp,
    //     CGPointMake(250, 250),
    //     kCGMouseButtonLeft
    // );

    // Now, execute these events with an interval to make them noticeable
    CGEventPost(kCGHIDEventTap, move1);
    sleep(1);
    // CGEventPost(kCGHIDEventTap, move2);
    // sleep(1);
    // CGEventPost(kCGHIDEventTap, click1_down);
    // CGEventPost(kCGHIDEventTap, click1_up);

    // Release the events
    // CFRelease(click1_up);
    // CFRelease(click1_down);
    // CFRelease(move2);
    CFRelease(move1);


  [pool release];
  return 0;
}