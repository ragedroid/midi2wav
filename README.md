# midi2wav
Open-source project based on midi conversion to a wav file (using BasicSynth, © 2008, Daniel R. Mitchell).

SoundFont used : GeneralUser GS 1.471 Version 2.1.0.0 1997-2015 by S. Christian Collins

How to execute
--------------

Set "master/Midi2Wav/Bin" as the current directory on cmd or bash. From here type "./midi2wav ../Midi/midin.mid", where 'midin' is any of the MIDI example files in the Midi directory. The "wavout.wav" file will be generated in the Wav directory and can be played easily using any multimedia software. 

Note that only single file-input and single file-output compatibility is taken care of as of June 26 2020, 17:00 pm. 

(last tested on Ubuntu 18.04.1 LTS on Friday, June 25th. The program only seems to work if put under the administrator directory in C:// on Windows 10 (and maybe earlier versions as well))

Improvements required
---------------------
- In main.cpp : Create 'bsString wavFile' to have the same midi-file name as in 'bsString midFile'. (update: DONE)
- Implement a directory (midin) -> directory (wavout) format within the code so that midi-files don't have to be put from the command line.
- Display the names of midi file on command line while parsing through a directory. (update: DONE for single file inputs)
