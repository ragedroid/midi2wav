# midi2wav
Open-source project based on midi conversion to a wav file (using BasicSynth, © 2008, Daniel R. Mitchell).

How to execute
--------------

Set "master/Midi2Wav/Bin" as the current directory on cmd or bash. From here type "./midi2wav ./midin.mid ./wavout.wav", where 'midin' is any of the MIDI example files in the Bin directory and 'wavout' is the custom name of WAV output. The "wavout.wav" file will be generated in the Bin directory itself and can be played easily using any multimedia software. 

Note that only single file-input and single file-output compatibility is taken care of as of June 20 2020, 16:19 pm. 

(last tested on Ubuntu 18.04.1 LTS on Friday, June 19th. The program only seems to work if put under the administrator directory in C:// on Windows 10 (and maybe earlier versions as well))

Improvements required
---------------------

- Removal of unnecessary header and cpp files (to the best extent possible without damaging the purpose of the program).
- In main.cpp : Create 'bsString wavFile' to have the same midi-file name as in 'bsString midFile'.
- Implement a directory (midin) -> directory (wavout) format within the code so that midi-files don't have to be put from the command line.
- Display the names of midi file on command line while parsing through a directory. (update: DONE)
