# Lissa delay

## About
Lissa is a delay module for Korg Nu:Tekt NTS1 and other synthesizers compatible with [logue SDK](https://github.com/korginc/logue-sdk).

The main purpose of Lissa is to generate a sweep signal to display wave form of input signal on an oscilloscpe as a [Lissageous figure](https://en.wikipedia.org/wiki/Lissajous_curve).

Lissa passes the input signal to the right channel of output with no change. The left channel is used for sweep signal which is one of sine wave, sawtooth wave, or the input signal with 90 degrees delay.

Lissa delay is mainly for fun and educational purposes. It is almost useless for usual music production.

## How to use

1. Firstly, install an oscilloscpe software with X-Y display capability on your PC or Mac. I recommend [Oscilloscope](https://asdfg.me/osci/), [Soundcard Scope](https://www.zeitnitz.eu/scope_en), or [Winscope](http://www.zen22142.zen.co.uk/Prac/winscope.htm).

1. Connect your NTS-1 output to audio input of your PC.

1. Hit E key of NTS-1. Then, press EG button and select "oPEn" with the TYPE knob.

1. Hit DELAY button and select "LiSa" with the TYPE knob.

1. Adjust the aspect ratio of Lissage figure with the B knob.

1. Press DELAY button and keep it pressed. Then turn clockwise the B knob to change sweep signal to sawtooth wave.

1. Adjust the pitch of sweep signal (left channel) to the same value with input signal (right channel).

1. Again press DELAY button and keep it pressed. Tweak the B knob and watch how the figure is changed. Modify the tone of input sound by the filter, OSC parameters, etc. and look how it is reflected on the screen.

## Parameters

To synchronize input signal and sweep signal, you have to tune sweep signal manually. That is done by Time parameter which is assigned to the knob A in the case of NTS1.

As time parameter changes, the frequency of sweep signal changes stepwise. The parameter value is between 0 and 127. The frequency of sweep signal become that of the time parameter value as MIDI note number.
