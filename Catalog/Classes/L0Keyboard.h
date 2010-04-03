//
//  L0Keyboard.h
//  MuiKit
//
//  Created by ∞ on 30/01/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#if TARGET_OS_IPHONE

#import <UIKit/UIKit.h>

@class L0KeyboardRubberBand;
@protocol L0KeyboardObserver;

@interface L0Keyboard : NSObject {
	NSMutableSet* observers;
	
	BOOL shown;
	
	CGRect bounds;
	CGPoint center;
	
	BOOL animating;
	CGPoint animationStartCenter;
	NSTimeInterval animationDuration;
	UIViewAnimationCurve animationCurve;
	
	CGFloat barHeight;
}

+ (L0Keyboard*) sharedInstance;

/* Adding an observer retains it! Be wary. */
- (void) addObserver:(id <L0KeyboardObserver>) o;
- (void) removeObserver:(id <L0KeyboardObserver>) o;

/* 'original' is the frame the view has when there is no keyboard around.
 If the keyboard isn't shown, then the original frame will be returned (or the view resized to it). */
- (CGRect) resizedFrameOfViewToPreventCovering:(UIView*) v originalFrame:(CGRect) original;

/* Resizes the view as per the above method. animated:YES will animate the resizing with the same duration and curve as the keyboard, if it's animating, or with a default animation otherwise. */
- (void) resizeViewToPreventCovering:(UIView*) v originalFrame:(CGRect) original animated:(BOOL) ani;

@property(readonly, getter=isShown) BOOL shown;
@property(readonly, getter=isAnimating) BOOL animating;

@property(readonly) CGRect bounds;
@property(readonly) CGPoint center;
@property(readonly) CGPoint origin;

@property(readonly) CGRect frame;

@property(readonly) CGPoint animationStartCenter;
@property(readonly) CGPoint animationStartOrigin;
@property(readonly) NSTimeInterval animationDuration;
@property(readonly) UIViewAnimationCurve animationCurve;

- (void) beginViewAnimationsAlongsideKeyboard:(NSString*) name context:(void*) context;

// The keyboard bar height is taken into account by resizedFrame... and company, creating an empty space above the keyboard where an accessory view can go (keyboard bar).
// Changing this changes the effect of subsequent invocations of resize...
// Default is 0.0.
@property CGFloat barHeight;

@end


@protocol L0KeyboardObserver <NSObject>

@optional

- (void) keyboardWillAppear:(L0Keyboard*) k;
- (void) keyboardWillDisappear:(L0Keyboard*) k;

- (void) keyboardDidAppear:(L0Keyboard*) k;
- (void) keyboardDidDisappear:(L0Keyboard*) k;

- (void) keyboardDidChangeBarHeight:(L0Keyboard*) k;

- (void) keyboardDidAddObserver:(L0Keyboard*) k;
- (void) keyboardDidRemoveObserver:(L0Keyboard*) k;

@end

/* Keyboard rubber bands are keyboard observers that automatically resize their associated views when the keyboard appears and disappears.
 
 Rubber bands also resize their views at appropriate times, for instance when a keyboard bar is added or changes.
 
 */
@interface L0KeyboardRubberBand : NSObject <L0KeyboardObserver> {
	CGRect originalFrame;
	UIView* view;
	BOOL animated;
}

@property CGRect originalFrame;
@property(retain) UIView* view;
@property BOOL animated; // defaults to YES

+ keyboardRubberBandForView:(UIView*) v; // sets the original frame to v's current one. useful for views coming from NIBs.
+ keyboardRubberBandForView:(UIView*) v originalFrame:(CGRect) o;

@end


/* Keyboard bar controllers are keyboard observers that automatically add or remove a view (called a keyboard bar) above the keyboard as it slides up or down.
 
 PLEASE NOTE: This is for toolbars that disappear when the keyboard goes down. If you want a bar to rest at the bottom of the screen and be lifted, use autoresizing masks and a keyboard rubber band (above) for its superview instead.
 
 PLEASE NOTE 2: The view's width will be resized to match the keyboard. The view's height will not be changed.
 
 PLEASE NOTE 3: Only ONE keyboard bar controller at a time, thank you. Remove all other ones before adding a new one.
 */
@interface L0KeyboardBarController : NSObject <L0KeyboardObserver> {
	UIView* view; UIResponder* inputResponder;
	CGFloat height;
	UIWindow* window;
	BOOL overlapsContent;
	BOOL active;
	BOOL hasEverAnimated;
}

@property(retain) UIView* view;

// Call this if the view's height changes.
- (void) updateBarHeight;

// This calls -updateBarHeight automatically.
- (void) setViewHeight:(CGFloat) h;

// The window where the view will be displayed. If unset, the current key window will be used.
@property(retain) UIWindow* window;

// If the text field or other text input responder is inside the bar view, there's a catch-22 (you can't make it first responder while not in a window, but the bar controller removes it from the window until the keyboard comes up). This works around the issue -- set the UIResponder that invokes the keyboard as the .inputResponder below, and use this class's -makeInputResponderFirst method instead of that object's -becomeFirstResponder. This will set up things so that the keyboard will come up for the responder AND the bar will work correctly.
// For this to work, the input responder MUST be either the .view or a subview of it.
// The input responder is NOT retained. Make sure you unset it if you change the view.
@property(assign) UIResponder* inputResponder;
- (void) makeInputResponderFirst;

// If YES, the view will be added without informing other views of its presence. If NO, the keyboard's bar height (-[L0Keyboard barHeight]) will be set to match the view's height, which will cause keyboard rubber bands to resize their views.
// If NO, it is recommended that the view be opaque.
// Default is NO, but the constructor sets this to YES in some cases. See below.
@property BOOL overlapsContent;

// Makes a keyboard bar controller.
// If the view is a UIToolbar, UINavigationBar or UISearchBar with .translucent set to YES, .overlapsContent will be set to YES by this method.
// The view will be removed from its superview, if any.
+ keyboardBarControllerWithView:(UIView*) v window:(UIWindow*) w;
+ keyboardBarControllerWithView:(UIView*) v; /* window set to nil */

@end


#endif
