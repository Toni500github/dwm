/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
#include "exitdwm.c"
#include "movestack.c"

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 2;        /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int swallowfloating    = 1;        /* 1 means swallow floating windows by default */
static const int rmaster            = 0;        /* 1 means master-area is initially on the right */
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int topbar             = 1;        /* 0 means bottom bar */
static const Bool viewontag         = True;     /* Switch view on tag switch */
static const char buttonbar[]       = " |";
#define ICONSIZE 16
#define ICONSPACING 5
static const char font[]          	= "monospace:size=10"; //"Noto Mono:style=Regular:size=10";
static const char dmenufont[]       = "JetBrains Mono:size=10";
//static const char *fonts[]	        = { "MesloLGS Nerd Font:size=11" };
static const char *fonts[]          = { /*font,*/ "Liberation Mono:size=10,5:antialias=true:autohint=true", "FontAwesome:style=Regular:pixelsize=15", "Iosevka:style=Medium:size=12", "JetBrainsMono Nerd Font:style:medium:size=12" };
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#24a4d6";
static char selbgcolor[]            = "#227dc7";

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static char *colors[][3] = {
       /*               fg                  bg               border   */
       [SchemeNorm] = { normfgcolor,      normbgcolor,     normbordercolor },
       [SchemeSel]  = { selfgcolor,       selbgcolor,      selbordercolor  },
       [SchemeTitle]= { selfgcolor,       selbgcolor,      normbgcolor     },
       [SchemeHid]  = { selbgcolor,       normbgcolor,     selbgcolor      }
};

static const char *const autostart[] = {
	"bash", "/home/toni/.config/dwm/autostart.sh", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const unsigned int ulinepad	= 5;	/* horizontal padding between the underline and tag */
static const unsigned int ulinestroke	= 2;	/* thickness / height of the underline */
static const unsigned int ulinevoffset	= 1;	/* how far above the bottom of the bar the line should appear */
static const int ulineall 		= 0;	/* 1 to show underline on all tags, 0 for just the active ones */

static const char *env[] = {
	"_JAVA_AWT_WM_NONREPARENTING=1",
	"XDG_SESSION_TYPE=x11",
	"XDG_SESSION_DESKTOP=dwm",
	"XDG_CURRENT_DESKTOP=dwm" /* if using xdg-desktop-portal */
};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class               instance  title                      tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",              NULL,     NULL,                        0,         1,          0,           0,        -1 },
	{ "Firefox",           NULL,     NULL,                        1 << 8,    0,          0,          -1,        -1 },
	{ "mpv",	       NULL,	 NULL,			      0,	 1,	     0,		 -1,	    -1 },
	{ "St",                NULL,     NULL,                        0,         0,          1,           0,        -1 },
  	{ "steam",             NULL,  	 NULL,                        0,         1,          0,           1,        -1 },
	{ NULL,                NULL,     "Event Tester",              0,         0,          0,           1,        -1 }, /* xev */
	RULE(.class = "Audacious", .iscentered = 0)
};

/* layout(s) */
static float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ " ",       NULL },    /* no layout function means floating behavior */
	{ " ",       monocle },
	{ "|M|",      centeredmaster },
        { ">M>",      centeredfloatingmaster },
	{ "[@]",      spiral },
        { "[\\]",     dwindle },
        { "H[]",      deck },
        { "TTT",      bstack },
        { "===",      bstackhoriz },
        { "HHH",      grid },
        { "###",      nrowgrid },
        { "---",      horizgrid },
        { ":::",      gaplessgrid },
        { NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run_history", "-m", dmenumon, /*"-fn", dmenufont,*/ "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "tabbed", "-r", "2", "st", "-w", "''", NULL };

/* per tag commands for tagspawn function */
static const char** const tagcommands[LENGTH(tags)] = {
	[4] = (const char*[]){ "audacious", NULL }
};

static const Key keys[] = {
	/* modifier                     key        	function        argument */
	TAGKEYS(                        XK_1,                      	0)
	TAGKEYS(                        XK_2,                      	1)
	TAGKEYS(                        XK_3,                      	2)
	TAGKEYS(                        XK_4,                      	3)
	TAGKEYS(                        XK_5,                      	4)
	TAGKEYS(                        XK_6,                      	5)
	TAGKEYS(                        XK_7,                      	6)
	TAGKEYS(                        XK_8,                      	7)
	TAGKEYS(                        XK_9,                      	8)

	{ MODKEY, 			XK_w,		spawn, 		{.v = (const char*[]){ "brave-browser", NULL } } },
	{ MODKEY,                       XK_d,      	spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,		XK_c,		spawn,		SHCMD("configmod") },
	{ MODKEY|ShiftMask,		XK_d,		spawn,		SHCMD("~/.config/rofi/launchers/type-4/launcher.sh") },
	{ MODKEY,             		XK_Return, 	spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_Return,      spawn,          {.v = (const char*[]){ "st", NULL } } },
	{ MODKEY|ControlMask,           XK_Return, 	tagspawn,       {0} },
	{ MODKEY,                       XK_b,      	togglebar,      {0} },
	{ MODKEY,                       XK_j,      	focusstackvis,  {.i = -1 } },
        { MODKEY,                       XK_k,      	focusstackvis,  {.i = +1 } },
        { MODKEY|ShiftMask,             XK_j,      	focusstackhid,  {.i = -1 } },
        { MODKEY|ShiftMask,             XK_k,      	focusstackhid,  {.i = +1 } },
	{ MODKEY,                       XK_i,      	incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_h,      	setmfact,       {.f = -0.01} },
	{ MODKEY,                       XK_l,      	setmfact,       {.f = +0.01} },
        { MODKEY|ShiftMask,             XK_h,           setcfact,       {.f = +0.05} },
        { MODKEY|ShiftMask,             XK_l,           setcfact,       {.f = -0.05} },
        { MODKEY|ShiftMask,             XK_o,           setcfact,       {.f =  0.00} },
  	{ MODKEY|ControlMask,           XK_j,       	movestack,      {.i = +1 } },
  	{ MODKEY|ControlMask,           XK_k,       	movestack,      {.i = -1 } },
	{ MODKEY,                       XK_z, 		zoom,           {0} },
	{ MODKEY,                       XK_Tab,    	view,           {0} },
	{ Mod1Mask,                     XK_Tab,         spawn,          SHCMD("skippy-xd-runner --switch-prev")/*SHCMD("rofi -show window -show-icons -config ~/.local/src/rofi/config.rasi")*/ },
	{ MODKEY,             		XK_q,      	killclient,     {0} },
	{ MODKEY,                       XK_t,      	setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      	setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      	setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_u,      	setlayout,      {.v = &layouts[3]} },
        { MODKEY,                       XK_o,      	setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_space,  	setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  	togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,      	togglefullscr,  {0} },
 	{ MODKEY|ShiftMask,             XK_i,      	togglermaster,  {0} },
	{ MODKEY,                       XK_0,      	view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      	tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  	focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, 	focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  	tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, 	tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_s,      	show,           {0} },
        { MODKEY|ShiftMask,    		XK_a,      	showall,        {0} },
	{ MODKEY|ControlMask,           XK_h,      	hide,           {0} },
/*	{ MODKEY|ShiftMask,           	XK_q,      	exitdwm,	{0}	},*/
	{ MODKEY|ShiftMask,             XK_q,      	spawn,          SHCMD("sysact") },
	{ MODKEY|ShiftMask,             XK_t,           spawn,          SHCMD("dmenu-translate") },
	{ MODKEY|ControlMask,           XK_s,           spawn,         	SHCMD("slock") },
	{ MODKEY|ShiftMask,             XK_r,      	quit,           {1} }, // it will self restart
	{ MODKEY,                       XK_x,   	viewtoleft,     {0} },
        { MODKEY,                       XK_v,  		viewtoright,    {0} },
        { MODKEY|ShiftMask,             XK_x,   	tagtoleft,      {0} },
        { MODKEY|ShiftMask,             XK_v,  		tagtoright,     {0} },
  	{ MODKEY|ShiftMask,             XK_s,       	spawn,          {.v = (const char*[]){ "flameshot", "gui", NULL} } },
	{ 0, XF86XK_AudioMute,                          spawn,          SHCMD("~/.config/dunst/scripts/volume mute") },
        { 0, XF86XK_AudioRaiseVolume,                   spawn,          SHCMD("~/.config/dunst/scripts/volume up") },
        { 0, XF86XK_AudioLowerVolume,                   spawn,          SHCMD("~/.config/dunst/scripts/volume down") },

  	{ MODKEY,                       XK_Down,   	moveresize,     {.v = "0x 25y 0w 0h" } },
	{ MODKEY,                       XK_Up,     	moveresize,     {.v = "0x -25y 0w 0h" } },
	{ MODKEY,                       XK_Right,  	moveresize,     {.v = "25x 0y 0w 0h" } },
	{ MODKEY,                       XK_Left,   	moveresize,     {.v = "-25x 0y 0w 0h" } },
	{ MODKEY|ShiftMask,             XK_Down,   	moveresize,     {.v = "0x 0y 0w 25h" } },
	{ MODKEY|ShiftMask,             XK_Up,     	moveresize,     {.v = "0x 0y 0w -25h" } },
	{ MODKEY|ShiftMask,             XK_Right,  	moveresize,     {.v = "0x 0y 25w 0h" } },
	{ MODKEY|ShiftMask,             XK_Left,   	moveresize,     {.v = "0x 0y -25w 0h" } },
	{ MODKEY|ControlMask,           XK_Up,     	moveresizeedge, {.v = "t"} },
	{ MODKEY|ControlMask,           XK_Down,   	moveresizeedge, {.v = "b"} },
	{ MODKEY|ControlMask,           XK_Left,   	moveresizeedge, {.v = "l"} },
	{ MODKEY|ControlMask,           XK_Right,  	moveresizeedge, {.v = "r"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Up,     	moveresizeedge, {.v = "T"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Down,   	moveresizeedge, {.v = "B"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Left,   	moveresizeedge, {.v = "L"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Right,  	moveresizeedge, {.v = "R"} },

	{ MODKEY|Mod1Mask,              XK_u,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_u,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_i,      incrigaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_o,      incrogaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_h,      incrihgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_h,      incrihgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_v,      incrivgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_v,      incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_p,      incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_p,      incrohgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_y,      incrovgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_y,      incrovgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_t,      togglegaps,     {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_t,      defaultgaps,    {0} },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkButton, 		0,	        Button1,	spawn,		{.v = termcmd } },
//	{ ClkStatusText,        0,              Button1,        spawn,          {.v = termcmd } },
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
 	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[1]} },
  	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkWinTitle,          0,              Button3,        zoom,           {0} },
	{ ClkWinTitle,          0,              Button1,        togglewin,      {0} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
