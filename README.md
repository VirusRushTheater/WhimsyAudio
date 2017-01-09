# WhimsyAudio #
Tests for audio engine for an upcoming project. This engine is based on PortAudio, which works under Linux (PC and ARM), Windows and Mac.

Uses the Data structures written in WhimsyDataStructs and puts them to use in a convenient way.

## How to compile ##
This project uses:
- _PortAudio_: Open Source platform for portable audio APIs. It works with ALSA in Linux and DirectSound in Windows.
- _Qt 5_: API to make graphical user interfaces. I don't know any better ways to make portable GUI applications with. Sorry.

If you can get PortAudio headers straight out from your distro, it's your lucky day. If not, you will have to compile it.
- Go to http://portaudio.com/download.html and download the last stable release. Put all those files in a folder called "portaudio" in this folder, please.
- (LINUX) It's necessary to build PortAudio with ALSA development kit for this to work.
- Run the configure file (./configure) - If you don't do so, it will still compile but the library will be useless, as it won't be able to play anything.
- Go to the Portaudio's build folder and compile it using CMake and Make.
- After that, you should have a couple of files:
    - libportaudio_static.a
    - libportaudio.so/.dll
- Install the library using `sudo make install` in PortAudio's folder.

If you got problems in compiling PortAudio, go to http://portaudio.com/docs/v19-doxydocs/tutorial_start.html.
