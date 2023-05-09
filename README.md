Important (kinda)
-----------
since every suckless patch, for when modyfing the config
file, they use `config.def.h` instead of `config.h` and since i 
don't want to remove and forget the `config.h` when compiling, 
i added these lines on `Makefile`:
```make
config.h: config.def.h #<- this word
        cp config.def.h $@
```
what it does, it's going to copy and replace from `config.def.h` to `config.h` and replace it everytime we run `make` or `sudo make install`
if you want to directly modify from the `config.h` file and leave `config.def.h` alone, just remove `config.def.h` near `config.h: ` and compile it without editing config.def.h

My personal dwm build, with so many patches
============================

Requirements
------------
In order to build dwm you need the Xlib header files.

Patches
------------
* actualfullscreen
* awesomebar
* centeredmaster
* cfacts
* cool-autostart
* exitmenu
* fixborders
* focusadjacenttag
* moveresize
* movestack
* pertag
* preserveonrestart
* removeborder
* resizecorners
* restartsig
* rmaster
* savefloats
* status2d-systray-6.4
* statusbutton
* steam
* swallow
* tagspawn
* titlecolor
* underlinetags
* vertical-horizontal-i3like
* viewontag
* winicon
