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
* actualfullscreen-20191112-cb3f58a
* alwayscenter-20200625-f04cac6
* centeredmaster-6.1
* cool-autostart-6.2
* cyclelayouts-20180524-6.2
* exitmenu-6.3
* fixborders-6.2
* gaplessgrid-20160731-56a31dc
* gridmode-20170909-ceac8c9
* moveresize-20221210-7ac106c
* movestack-20211115-a786211
* nrowgrid-6.1
* preserveonrestart-6.3
* resizecorners-6.2
* restartsig-20180523-6.2
* rmaster-6.2
* statusbutton-20180524-c8e9479
* steam-6.2
* swallow-6.3
* systray-6.4
* titlecolor-20210815-ed3ab6b4
* underlinetags-6.2
* vertical-horizontal-i3like
* winicon-6.3-v2.1
* xresources-20210827-138b405
