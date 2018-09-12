# WELCOME TO THE QUB³D PROJECT!

This is the main repository for the Qub3d project, just clone this, and run a a few CMake commands and you're good to go!

##To Build:
Run: 
```
cmake -H. -Bbuild
```
This will generate configurations into a directory called "build"

Now run:
```
cmake --build build
```
and this will actually build the project.

Things like the client, launcher and server will be built into ``COMPILE/bin``
Things like the engine and renderer will be built into ``COMPILE/lib``
