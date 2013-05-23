//
//  ESAppDelegate.m
//  ES
//
//  Created by cong zhu on 3/31/09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import "ESAppDelegate.h"
#import "EAGLView.h"

@implementation ESAppDelegate

@synthesize window;
@synthesize glView;

- (void)applicationDidFinishLaunching:(UIApplication *)application {
    
	glView.animationInterval = 1.0 / 60.0;
	// Make the window active
	[window makeKeyAndVisible];
	
	// Now try to set the orientation to landscape (right)
	[[UIDevice currentDevice] setOrientation:UIInterfaceOrientationLandscapeRight];
	[glView startAnimation];
	
}


- (void)applicationWillResignActive:(UIApplication *)application {
	glView.animationInterval = 1.0 / 5.0;
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
	glView.animationInterval = 1.0 / 60.0;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	return (interfaceOrientation == UIInterfaceOrientationLandscapeRight);
}

- (void)dealloc {
	[window release];
	[glView release];
	[super dealloc];
}

@end
