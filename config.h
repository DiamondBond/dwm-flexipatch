/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;     /* 0 means no bar */
static const int topbar             = 1;     /* 0 means bottom bar */
/* static const char *fonts[]          = { "terminus:size=12" }; */
/* static const char dmenufont[]       = "terminus:size=12"; */
/* static const char col_gray1[]       = "#000000"; */
/* static const char col_gray2[]       = "#AAAAAA"; */
/* static const char col_gray3[]       = "#ffffff"; */
/* static const char col_gray4[]       = "#ffffff"; */
/* static const char col_cyan[]        = "#000000"; */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_cyan },
	[SchemeSel]  = { col_gray4, col_cyan,  col_gray2 },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Pavucontrol",     NULL,       NULL,       0,            1,           -1 },
	{ "Transmission-gtk",     NULL,       NULL,       0,            1,           -1 },
	{ "Xfce4-appfinder",     NULL,       NULL,       0,            1,           -1 },
	{ "TelegramDesktop",     NULL,       NULL,       0,            1,           -1 },
	{ "Galculator",     NULL,       NULL,       0,            1,           -1 },
	{ "Gnome-calendar",     NULL,       NULL,       0,            1,           -1 },
	{ "Blueman-manager",     NULL,       NULL,       0,            1,           -1 },
	{ "GParted",     NULL,       NULL,       0,            1,           -1 },
	{ "Cheese",     NULL,       NULL,       0,            1,           -1 },
	{ "XTerm",     NULL,       NULL,       0,            1,           -1 },
	{ "Steam",     NULL,       NULL,       0,            1,           -1 },
	{ "zoom",     NULL,       NULL,       0,            1,           -1 },
	{ "Software-properties-gtk",     NULL,       NULL,       0,            1,           -1 },
	{ "Lightdm-gtk-greeter-settings",     NULL,       NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *findercmd[]  = { "xfce4-appfinder", NULL };
static const char *roficmd[]  = { "rofi", "-combi-modi","window,drun","-show", "combi", NULL };
static const char *termcmd[]  = { "xfce4-terminal", NULL };
static const char *xterm[]  = { "xterm", NULL };
static const char *web[] = { "firefox", NULL };
static const char *shot[] = { "xfce4-screenshooter", "-f", NULL };
static const char *winshot[] = { "xfce4-screenshooter", "-w", NULL };
static const char *selshot[] = { "xfce4-screenshooter", "-r", NULL };
static const char *explorer[] = { "thunar", NULL };
static const char *emacs[] = { "emacsclient", "-c","-n","-a","'emacs'", NULL };
static const char *mixer[] = { "pavucontrol", NULL };
static const char *calc[] = { "galculator", NULL };
static const char *mpv[] = { "mpv", "--player-operation-mode=pseudo-gui", NULL };
static const char *music[] = { "sh", "/home/diamond/bin/startmusic", NULL };
static const char *launcher[] = { "sh", "/home/diamond/bin/launcher.sh", NULL };
static const char *statuscmd[] = { "sh", "/home/diamond/bin/statf", NULL };
static const char *xkill[] = { "xkill", NULL };

/* Volume and brightness binds */
static const char *volup[] = { "pactl", "set-sink-volume", "0", "+5%", NULL };
static const char *voldown[] = { "pactl", "set-sink-volume", "0", "-5%", NULL };
static const char *volmute[] = { "pactl", "set-sink-mute", "0", "toggle", NULL };
static const char *brup[] = { "sudo", "brightnessctl", "set", "5%+", NULL };
static const char *brdown[] = { "sudo", "brightnessctl", "set", "5%-", NULL };


static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ControlMask,           XK_t,	   spawn,          {.v = xterm } },
	{ MODKEY,						XK_w,	   spawn,	   	   {.v = web } },
	{ 0,							XK_Print,  spawn,	   	   {.v = shot } },
	{ MODKEY,						XK_Print,  spawn,	   	   {.v = winshot } },
	{ MODKEY|ShiftMask,				XK_Print,  spawn,	   	   {.v = selshot } },
	{ MODKEY,						XK_e,      spawn,	  	   {.v = explorer } },
	{ Mod4Mask,						XK_e,      spawn,	  	   {.v = emacs } },
	{ Mod4Mask,						XK_space,  spawn,	  	   {.v = roficmd } },
	{ Mod4Mask,						XK_m,      spawn,	  	   {.v = music } },
	{ Mod4Mask,						XK_v,      spawn,	  	   {.v = mpv } },
	{ Mod4Mask,						XK_g,      spawn,	  	   {.v = calc } },
	{ MODKEY,						XK_F3,     spawn,	  	   {.v = findercmd } },
	{ MODKEY|ControlMask,			XK_p,	   spawn,	  	   {.v = mixer } },
	{ Mod4Mask,						XK_x,      spawn,	  	   {.v = launcher } },
	{ MODKEY|ShiftMask,				XK_f,      spawn,	  	   {.v = statuscmd } },
	{ MODKEY|ShiftMask,				XK_x,      spawn,	  	   {.v = xkill } },
	{ MODKEY|ControlMask,           XK_Right,  shiftview,  { .i = +1 } },
	{ MODKEY|ControlMask,           XK_Left,   shiftview,  { .i = -1 } },
	{ 0, 							XF86XK_AudioRaiseVolume,   spawn, 	   	{.v = volup} },
	{ 0, 							XF86XK_AudioLowerVolume,   spawn, 	   	{.v = voldown} },
	{ 0, 							XF86XK_AudioMute,	   	   spawn, 	   	{.v = volmute} },
	{ 0, 							XF86XK_MonBrightnessUp,    spawn, 	   	{.v = brup} },
	{ 0, 							XF86XK_MonBrightnessDown,  spawn, 	  	{.v = brdown} },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ Mod1Mask,		        		XK_F4,     killclient,     {0} },
	{ MODKEY,						XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
