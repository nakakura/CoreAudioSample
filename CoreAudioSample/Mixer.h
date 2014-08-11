//
//  Mixer.h
//  CoreAudioSample
//
//  Created by Toshiya Nakakura on 8/11/14.
//  Copyright (c) 2014 arukakan. All rights reserved.
//

#import <AudioToolbox/AudioToolbox.h>
#import <AVFoundation/AVFoundation.h>

#define NUM_FILES 1

// Data structure for mono or stereo sound, to pass to the application's render callback function,
//    which gets invoked by a Mixer unit input bus when it needs more audio to play.
typedef struct {
    
    BOOL                 isStereo;           // set to true if there is data in the audioDataRight member
    UInt32               frameCount;         // the total number of frames in the audio data
    UInt32               sampleNumber;       // the next audio sample to play
    AudioUnitSampleType  *audioDataLeft;     // the complete left (or mono) channel of audio data read from an audio file
    AudioUnitSampleType  *audioDataRight;    // the complete right channel of audio data read from an audio file
    
} soundStruct, *soundStructPtr;

@interface MixerHostAudio : NSObject <AVAudioSessionDelegate> {
    
    Float64                         graphSampleRate;
    CFURLRef                        sourceURLArray[NUM_FILES];
    soundStruct                     soundStructArray[NUM_FILES];
    
    // Before using an AudioStreamBasicDescription struct you must initialize it to 0. However, because these ASBDs
    // are declared in external storage, they are automatically initialized to 0.
    AudioStreamBasicDescription     stereoStreamFormat;
    AudioStreamBasicDescription     monoStreamFormat;
    AUGraph                         processingGraph;
    BOOL                            playing;
    BOOL                            interruptedDuringPlayback;
    AudioUnit                       mixerUnit;
}

@property (readwrite)           AudioStreamBasicDescription stereoStreamFormat;
@property (readwrite)           AudioStreamBasicDescription monoStreamFormat;
@property (readwrite)           Float64                     graphSampleRate;
@property (getter = isPlaying)  BOOL                        playing;
@property                       BOOL                        interruptedDuringPlayback;
@property                       AudioUnit                   mixerUnit;

- (void) obtainSoundFileURLs: (NSURL*) fileUrl;
- (void) setupAudioSession;
- (void) setupStereoStreamFormat;
- (void) setupMonoStreamFormat;

- (void) readAudioFilesIntoMemory;

- (void) configureAndInitializeAudioProcessingGraph;
- (void) startAUGraph: (NSURL*) fileUrl;
- (void) stopAUGraph;

- (void) enableMixerInput: (UInt32) inputBus isOn: (AudioUnitParameterValue) isONValue;
- (void) setMixerInput: (UInt32) inputBus gain: (AudioUnitParameterValue) inputGain;
- (void) setMixerOutputGain: (AudioUnitParameterValue) outputGain;

- (void) printASBD: (AudioStreamBasicDescription) asbd;
- (void) printErrorMessage: (NSString *) errorString withStatus: (OSStatus) result;
-(void) setVolume: (float) volume;

@end


