//
//  LandscapeViewController.h
//  Wi-Fly-app
//
//  Created by Daniel Dickison on 7/30/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class TrackPadControl;

@interface LandscapeViewController : UIViewController

@property (nonatomic, retain) IBOutlet TrackPadControl *leftTrackPad;
@property (nonatomic, retain) IBOutlet TrackPadControl *rightTrackPad;

- (IBAction)infoButtonAction;

@end
