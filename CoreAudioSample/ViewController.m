//
//  ViewController.m
//  CoreAudioSample
//
//  Created by Toshiya Nakakura on 8/11/14.
//  Copyright (c) 2014 arukakan. All rights reserved.
//

#import "ViewController.h"
#import "Mixer.h"

@interface ViewController ()
@property (nonatomic, strong)    MixerHostAudio              *audioObject;
@property (nonatomic, strong)    AVAudioSession              *audioSession;

@end

@implementation ViewController
@synthesize audioObject = _audioObject;
@synthesize audioSession = _audioSession;

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    
    _audioObject = [[MixerHostAudio alloc] init];
    self.view.userInteractionEnabled = YES;
    
    self.audioSession = [AVAudioSession sharedInstance];
    [_audioSession setActive:YES error:nil];
    [_audioSession addObserver:self forKeyPath:@"outputVolume" options:NSKeyValueObservingOptionOld | NSKeyValueObservingOptionNew  context:NULL];
}

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context
{
    if ([keyPath isEqualToString:@"outputVolume"])
    {
        NSLog(@"change %f", [change[@"new"] floatValue]);
        //[self setVolume:[change[@"new"] floatValue]];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

// iOSから割り込み(アプリ停止等)があったときのためにハンドラを登録する
- (void) registerForAudioObjectNotifications {
    
    NSNotificationCenter *notificationCenter = [NSNotificationCenter defaultCenter];
    
    [notificationCenter addObserver: self
                           selector: @selector (handlePlaybackStateChanged:)
                               name: @"MixerHostAudioObjectPlaybackStateDidChangeNotification"
                             object: _audioObject];
    [self playOrStop: nil];
}

// ハンドラのコールバック
- (void) handlePlaybackStateChanged: (id) notification {
    
    [self playOrStop: nil];
}

// Handle a play/stop button tap
- (IBAction) playOrStop: (id) sender {
    
    if (_audioObject.isPlaying) {
        
        [_audioObject stopAUGraph];
        
    } else {
        NSURL *source1   = [[NSBundle mainBundle] URLForResource: @"source2"
                                                      withExtension: @"mp3"];
        [_audioObject startAUGraph: source1];
        [_audioObject setVolume: 1.0];
        [_audioObject setMixerOutputGain: 1.0];
    }
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self playOrStop: nil];
}

@end
