#!/bin/bash

#xset r rate 270 55
#dwmblocks
feh --no-fehbg --bg-fill "$HOME/Immagini/005.png" #nitrogen --set-scaled $HOME/Immagini/005.png
slstatus &
dbus-update-activation-environment --systemd DBUS_SESSION_BUS_ADDRESS DISPLAY XAUTHORITY
dbus-update-activation-environment --systemd --all
nvidia-settings --load-config-only -a AllowFlipping=1; nvidia-settings -a AllowFlipping=0
systemctl --user import-environment XDG_SESSION_TYPE XDG_CURRENT_DESKTOP
#slbar &
/usr/lib/polkit-gnome/polkit-gnome-authentication-agent-1 &
nm-applet &
flameshot &
picom -b &
$HOME/.conky/awesome-conky/launch.sh

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
