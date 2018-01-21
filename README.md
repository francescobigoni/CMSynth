# CMSynth
Sound synthesis using an allpass filter chain with coefficient modulation

JUCE audio plugin implemented after Kleimola et al., <i>Sound Synthesis Using an Allpass Filter Chain with Audio-Rate Coefficient Modulation</i>,
Proc. of the 12th Int. Conference on Digital Audio Effects (DAFx-09), Como, Italy, September 1-4, 2009.

By using linear interpolation, a fractional number of stages is allowed to avoid phase discontinuities when altering the chain length in real time. 

## Parameters
- Modulation frequency: frequency of the modulating sine wave
- Modulation index: amplitude of the modulating sine wave
- Number of stages: number of cascade-coupled first-order allpass filters in the chain

## How to play
The plugin takes one audio input (sine oscillator, or other) and gives a stereo output. To get more complex spectral structures, two or more allpass filter chains can be coupled.
