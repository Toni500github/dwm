#!/bin/bash
export XDG_CURRENT_DESKTOP=dwm
export XDG_SESSION_TYPE=x11
export XDG_SESSION_DESKTOP=dwm

nm-applet &
#slbar &
slstatus &
picom -b &
#$HOME/.conky/awesome-conky/launch.sh
nitrogen --set-scaled $HOME/Immagini/005.png
xset r rate 270 45
#dwmblocks
/usr/lib/polkit-gnome/polkit-gnome-authentication-agent-1 &
#dbus-update-activation-environment --systemd X11_DISPLAY XDG_CURRENT_DESKTOP=dwm
dbus-update-activation-environment --systemd DBUS_SESSION_BUS_ADDRESS DISPLAY XAUTHORITY
dbus-update-activation-environment --systemd --all
#systemctl --user import-environment XDG_SESSION_TYPE XDG_CURRENT_DESKTOP

sleep 1
killall xdg-desktop-portal-hyprland
killall xdg-desktop-portal-gnome
killall xdg-desktop-portal-kde
killall xdg-desktop-portal-lxqt
killall xdg-desktop-portal-wlr
killall xdg-desktop-portal-gtk
killall xdg-desktop-portal
sleep 1
/usr/lib/xdg-desktop-portal-gtk &
sleep 2
/usr/lib/xdg-desktop-portal &
