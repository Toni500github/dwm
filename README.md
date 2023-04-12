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
* actualfullscreen-20191112-cb3f58a.diff
* alwayscenter-20200625-f04cac6.diff
* centeredmaster-6.1.diff
* cool-autostart-6.2.diff
* cyclelayouts-20180524-6.2.diff
* exitmenu-6.3.diff
* fixborders-6.2.diff
* gaplessgrid-20160731-56a31dc.diff
* gridmode-20170909-ceac8c9.diff
* moveresize-20221210-7ac106c.diff
* movestack-20211115-a786211.diff
* nrowgrid-6.1.diff
* pertag-20200914-61bb8b2.diff
* preserveonrestart-6.3.diff
* removeborder-20220626-d3f93c7.diff
* resizecorners-6.2.diff
* restartsig-20180523-6.2.diff
* rmaster-6.2.diff
* status2d-systray-6.4.diff
* statusbutton-20180524-c8e9479.diff
* steam-6.2.diff
* swallow-6.3.diff
* titlecolor-20210815-ed3ab6b4.diff
* underlinetags-6.2.diff
* vertical-horizontal-i3like.diff
* winicon-6.3-v2.1.diff
