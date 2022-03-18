/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx       = 1;   /* border pixel of windows */
static const unsigned int snap           = 32;  /* snap pixel */
static const int showbar                 = 1;   /* 0 means no bar */
static const int topbar                  = 1;   /* 0 means bottom bar */
/* Status is to be shown on: -1 (all monitors), 0 (a specific monitor by index), 'A' (active monitor) */
static const int statusmon               = 'A';
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int showsystray             = 1;   /* 0 means no systray */

/* Indicators: see patch/bar_indicators.h for options */
static int tagindicatortype              = INDICATOR_TOP_LEFT_SQUARE;
static int tiledindicatortype            = INDICATOR_NONE;
static int floatindicatortype            = INDICATOR_TOP_LEFT_SQUARE;
static const char *fonts[]               = { "monospace:size=10" };
static const char dmenufont[]            = "monospace:size=10";

static char c000000[]                    = "#000000"; // placeholder value

static char normfgcolor[]                = "#bbbbbb";
static char normbgcolor[]                = "#222222";
static char normbordercolor[]            = "#444444";
static char normfloatcolor[]             = "#db8fd9";

static char selfgcolor[]                 = "#eeeeee";
static char selbgcolor[]                 = "#005577";
static char selbordercolor[]             = "#005577";
static char selfloatcolor[]              = "#005577";

static char titlenormfgcolor[]           = "#bbbbbb";
static char titlenormbgcolor[]           = "#222222";
static char titlenormbordercolor[]       = "#444444";
static char titlenormfloatcolor[]        = "#db8fd9";

static char titleselfgcolor[]            = "#eeeeee";
static char titleselbgcolor[]            = "#005577";
static char titleselbordercolor[]        = "#005577";
static char titleselfloatcolor[]         = "#005577";

static char tagsnormfgcolor[]            = "#bbbbbb";
static char tagsnormbgcolor[]            = "#222222";
static char tagsnormbordercolor[]        = "#444444";
static char tagsnormfloatcolor[]         = "#db8fd9";

static char tagsselfgcolor[]             = "#eeeeee";
static char tagsselbgcolor[]             = "#005577";
static char tagsselbordercolor[]         = "#005577";
static char tagsselfloatcolor[]          = "#005577";

static char hidnormfgcolor[]             = "#005577";
static char hidselfgcolor[]              = "#227799";
static char hidnormbgcolor[]             = "#222222";
static char hidselbgcolor[]              = "#222222";

static char urgfgcolor[]                 = "#bbbbbb";
static char urgbgcolor[]                 = "#222222";
static char urgbordercolor[]             = "#ff0000";
static char urgfloatcolor[]              = "#db8fd9";



static char *colors[][ColCount] = {
	/*                       fg                bg                border                float */
	[SchemeNorm]         = { normfgcolor,      normbgcolor,      normbordercolor,      normfloatcolor },
	[SchemeSel]          = { selfgcolor,       selbgcolor,       selbordercolor,       selfloatcolor },
	[SchemeTitleNorm]    = { titlenormfgcolor, titlenormbgcolor, titlenormbordercolor, titlenormfloatcolor },
	[SchemeTitleSel]     = { titleselfgcolor,  titleselbgcolor,  titleselbordercolor,  titleselfloatcolor },
	[SchemeTagsNorm]     = { tagsnormfgcolor,  tagsnormbgcolor,  tagsnormbordercolor,  tagsnormfloatcolor },
	[SchemeTagsSel]      = { tagsselfgcolor,   tagsselbgcolor,   tagsselbordercolor,   tagsselfloatcolor },
	[SchemeHidNorm]      = { hidnormfgcolor,   hidnormbgcolor,   c000000,              c000000 },
	[SchemeHidSel]       = { hidselfgcolor,    hidselbgcolor,    c000000,              c000000 },
	[SchemeUrg]          = { urgfgcolor,       urgbgcolor,       urgbordercolor,       urgfloatcolor },
};



static const char *const autostart[] = {
	"/usr/lib/polkit-gnome/polkit-gnome-authentication-agent-1", NULL,
	"xset", "b", "off", NULL,
	"sh", "-c", "/home/diamond/bin/disable_touchscreen.sh", NULL,
	"hsetroot", "-solid", "darkgray", NULL,
	"xrdb", "/home/diamond/.Xresources", NULL,
	"sh", "-c", "while :; do /home/diamond/bin/dwmstatus.sh -; sleep 60; done", NULL,
	"sh", "-c", "/home/diamond/bin/startcompositor", NULL,
	"dunst", NULL,
	/* "xss-lock", "slock", NULL, */
	"xfce4-power-manager", NULL,
	"libinput-gestures", NULL,
	"nm-applet", NULL,
	"blueman-applet", NULL,
	"/home/diamond/git/matebook-applet/matebook-applet", NULL,
	/* "/usr/bin/volumeicon", NULL, */
	"emacs", "--daemon", NULL,
	"dropbox", "start", "-i", NULL,
	NULL /* terminate */
};


/* Tags
 * In a traditional dwm the number of tags in use can be changed simply by changing the number
 * of strings in the tags array. This build does things a bit different which has some added
 * benefits. If you need to change the number of tags here then change the NUMTAGS macro in dwm.c.
 *
 * Examples:
 *
 *  1) static char *tagicons[][NUMTAGS*2] = {
 *         [DEFAULT_TAGS] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "G", "H", "I" },
 *     }
 *
 *  2) static char *tagicons[][1] = {
 *         [DEFAULT_TAGS] = { "•" },
 *     }
 *
 * The first example would result in the tags on the first monitor to be 1 through 9, while the
 * tags for the second monitor would be named A through I. A third monitor would start again at
 * 1 through 9 while the tags on a fourth monitor would also be named A through I. Note the tags
 * count of NUMTAGS*2 in the array initialiser which defines how many tag text / icon exists in
 * the array. This can be changed to *3 to add separate icons for a third monitor.
 *
 * For the second example each tag would be represented as a bullet point. Both cases work the
 * same from a technical standpoint - the icon index is derived from the tag index and the monitor
 * index. If the icon index is is greater than the number of tag icons then it will wrap around
 * until it an icon matches. Similarly if there are two tag icons then it would alternate between
 * them. This works seamlessly with alternative tags and alttagsdecoration patches.
 */
static char *tagicons[][NUMTAGS] = {
	[DEFAULT_TAGS]        = { "1", "2", "3", "4", "5", "6", "7", "8", "9" },
	[ALTERNATIVE_TAGS]    = { "A", "B", "C", "D", "E", "F", "G", "H", "I" },
	[ALT_TAGS_DECORATION] = { "<1>", "<2>", "<3>", "<4>", "<5>", "<6>", "<7>", "<8>", "<9>" },
};


/* There are two options when it comes to per-client rules:
 *  - a typical struct table or
 *  - using the RULE macro
 *
 * A traditional struct table looks like this:
 *    // class      instance  title  wintype  tags mask  isfloating  monitor
 *    { "Gimp",     NULL,     NULL,  NULL,    1 << 4,    0,          -1 },
 *    { "Firefox",  NULL,     NULL,  NULL,    1 << 7,    0,          -1 },
 *
 * The RULE macro has the default values set for each field allowing you to only
 * specify the values that are relevant for your rule, e.g.
 *
 *    RULE(.class = "Gimp", .tags = 1 << 4)
 *    RULE(.class = "Firefox", .tags = 1 << 7)
 *
 * Refer to the Rule struct definition for the list of available fields depending on
 * the patches you enable.
 */
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 *	WM_WINDOW_ROLE(STRING) = role
	 *	_NET_WM_WINDOW_TYPE(ATOM) = wintype
	 */
	RULE(.wintype = WTYPE "DIALOG", .isfloating = 1)
	RULE(.wintype = WTYPE "UTILITY", .isfloating = 1)
	RULE(.wintype = WTYPE "TOOLBAR", .isfloating = 1)
	RULE(.wintype = WTYPE "SPLASH", .isfloating = 1)
	/* RULE(.class = "Gimp", .isfloating = 1) */
	RULE(.class = "Event Tester", .isfloating = 1)
	RULE(.class = "Pavucontrol", .isfloating = 1)
	RULE(.title = "Library", .isfloating = 1)
	RULE(.class = "Transmission-gtk", .isfloating = 1)
	RULE(.class = "Xfce4-appfinder", .isfloating = 1)
	RULE(.class = "TelegramDesktop", .isfloating = 1)
	RULE(.class = "Galculator", .isfloating = 1)
	RULE(.class = "Gpick", .isfloating = 1)
	RULE(.class = "Gnome-calendar", .isfloating = 1)
	RULE(.class = "Blueman-manager", .isfloating = 1)
	RULE(.class = "Gparted", .isfloating = 1)
	RULE(.class = "Cheese", .isfloating = 1)
	RULE(.class = "XTerm", .isfloating = 1)
	RULE(.class = "Steam", .isfloating = 1)
	RULE(.class = "zoom", .isfloating = 1)
	RULE(.class = "Gzdoom", .isfloating = 1)
	RULE(.class = "Tk", .isfloating = 1)
	RULE(.class = "openmw-launcher", .isfloating = 1)
	RULE(.class = "Toplevel", .isfloating = 1)
	RULE(.class = "Gnome-system-monitor", .isfloating = 1)
	RULE(.class = "Xfce4-power-manager-settings", .isfloating = 1)
	RULE(.class = "Nm-connection-editor", .isfloating = 1)
	RULE(.class = "Software-properties-gtk", .isfloating = 1)
	RULE(.class = "Lightdm-gtk-greeter-settings", .isfloating = 1)
};



/* Bar rules allow you to configure what is shown where on the bar, as well as
 * introducing your own bar modules.
 *
 *    monitor:
 *      -1  show on all monitors
 *       0  show on monitor 0
 *      'A' show on active monitor (i.e. focused / selected) (or just -1 for active?)
 *    bar - bar index, 0 is default, 1 is extrabar
 *    alignment - how the module is aligned compared to other modules
 *    widthfunc, drawfunc, clickfunc - providing bar module width, draw and click functions
 *    name - does nothing, intended for visual clue and for logging / debugging
 */
static const BarRule barrules[] = {
	/* monitor   bar    alignment         widthfunc                drawfunc                clickfunc                name */
	{ -1,        0,     BAR_ALIGN_LEFT,   width_tags,              draw_tags,              click_tags,              "tags" },
	{  0,        0,     BAR_ALIGN_RIGHT,  width_systray,           draw_systray,           click_systray,           "systray" },
	{ -1,        0,     BAR_ALIGN_LEFT,   width_ltsymbol,          draw_ltsymbol,          click_ltsymbol,          "layout" },
	{ statusmon, 0,     BAR_ALIGN_RIGHT,  width_status,            draw_status,            click_status,            "status" },
	{ -1,        0,     BAR_ALIGN_NONE,   width_awesomebar,        draw_awesomebar,        click_awesomebar,        "awesomebar" },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int decorhints  = 1;    /* 1 means respect decoration hints */



static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
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
static const char *dmenucmd[] = {
	"dmenu_run",
	"-m", dmenumon,
	"-fn", dmenufont,
	"-nb", normbgcolor,
	"-nf", normfgcolor,
	"-sb", selbgcolor,
	"-sf", selfgcolor,
	NULL
};
/* static const char *termcmd[]  = { "st", NULL }; */
static const char *roficmd[]  = { "rofi", "-combi-modi","window,drun","-show", "combi", NULL };
static const char *rofitab[]  = { "rofi", "-show","window", NULL };
static const char *termcmd[]  = { "xfce4-terminal", NULL };
static const char *findercmd[]  = { "xfce4-appfinder", NULL };
static const char *xterm[]  = { "xterm", NULL };
static const char *web[] = { "firefox", NULL };
static const char *altweb[] = { "chromium", NULL };
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
/* static const char *exitdwm[] = { "sh", "/home/diamond/bin/exitdwm", NULL }; */
static const char *xkill[] = { "xkill", NULL };
static const char *slockcmd[] = { "slock", NULL };
static const char *toggle_compositor[] = { "sh", "/home/diamond/bin/dwm_tog_comp", NULL };
static const char *dunst_dismiss[] = { "sh", "/home/diamond/bin/dunst_dismiss", NULL };

/* Volume and brightness binds */
/* static const char *volup[] = { "pactl", "set-sink-volume", "0", "+5%", NULL }; */
/* static const char *voldown[] = { "pactl", "set-sink-volume", "0", "-5%", NULL }; */
/* static const char *micmute[] = { "pactl", "set-source-mute", "0", "toggle", NULL }; */
static const char *micmute[] = { "sh", "/home/diamond/bin/toggleMic", NULL };
static const char *volmute[] = { "sh", "/home/diamond/bin/toggleVolume", NULL };
static const char *volup[] = { "sh", "/home/diamond/bin/changeVolume", "+5%", NULL };
static const char *voldown[] = { "sh", "/home/diamond/bin/changeVolume", "-5%", NULL };
/* static const char *brup[] = { "brightnessctl", "set", "5%+", NULL }; */
/* static const char *brdown[] = { "brightnessctl", "set", "5%-", NULL }; */
static const char *brup[] = { "sh", "/home/diamond/bin/changeBrightness", "5%+", NULL };
static const char *brdown[] = { "sh", "/home/diamond/bin/changeBrightness", "5%-", NULL };


static Key keys[] = {
	/* modifier                     key            function                argument */
	{ MODKEY,                       XK_p,          spawn,                  {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return,     spawn,                  {.v = termcmd } },
	{ MODKEY,                       XK_b,          togglebar,              {0} },
	{ MODKEY|ControlMask,           XK_t,	   spawn,          {.v = xterm } },
	{ MODKEY,						XK_w,	   spawn,	   	   {.v = web } },
	{ MODKEY|ShiftMask,				XK_w,	   spawn,	   	   {.v = altweb } },
	{ 0,							XK_Print,  spawn,	   	   {.v = shot } },
	{ MODKEY,						XK_Print,  spawn,	   	   {.v = winshot } },
	{ ShiftMask,	                XK_Print,  spawn,	   	   {.v = selshot } },
	{ Mod4Mask,                     XK_l,      spawn,          {.v = slockcmd } },
	{ MODKEY,						XK_e,      spawn,	  	   {.v = explorer } },
	{ Mod4Mask,						XK_e,      spawn,	  	   {.v = emacs } },
	{ Mod4Mask,						XK_space,  spawn,	  	   {.v = roficmd } },
	{ ControlMask,						XK_space,  spawn,	  	   {.v = dunst_dismiss } },
	{ Mod4Mask,						XK_m,      spawn,	  	   {.v = music } },
	{ Mod4Mask,						XK_v,      spawn,	  	   {.v = mpv } },
	{ Mod4Mask,						XK_g,      spawn,	  	   {.v = calc } },
	{ MODKEY,						XK_F3,     spawn,	  	   {.v = findercmd } },
	{ MODKEY|ControlMask,			XK_p,	   spawn,	  	   {.v = mixer } },
	{ Mod4Mask,						XK_x,      spawn,	  	   {.v = launcher } },
	{ MODKEY|ShiftMask,				XK_F12,    spawn,	  	   {.v = toggle_compositor } },
	{ MODKEY|ShiftMask,				XK_f,      spawn,	  	   {.v = statuscmd } },
	{ MODKEY|ShiftMask,				XK_x,      spawn,	  	   {.v = xkill } },
	{ MODKEY|ControlMask,           XK_Right,  shiftview,  { .i = +1 } },
	{ MODKEY|ControlMask,           XK_Left,   shiftview,  { .i = -1 } },
	{ 0, 							XF86XK_AudioRaiseVolume,   spawn, 	   	{.v = volup} },
	{ 0, 							XF86XK_AudioLowerVolume,   spawn, 	   	{.v = voldown} },
	{ 0, 							XF86XK_AudioMute,	   	   spawn, 	   	{.v = volmute} },
	{ 0, 							XF86XK_AudioMicMute,	   spawn, 	   	{.v = micmute} },
	{ 0, 							XF86XK_MonBrightnessUp,    spawn, 	   	{.v = brup} },
	{ 0, 							XF86XK_MonBrightnessDown,  spawn, 	  	{.v = brdown} },
	{ MODKEY,                       XK_grave,      showhideclient,                {0} },
	{ MODKEY,                       XK_j,          focusstack,             {.i = +1 } },
	{ MODKEY,                       XK_k,          focusstack,             {.i = -1 } },
	{ MODKEY,                       XK_i,          incnmaster,             {.i = +1 } },
	{ MODKEY,                       XK_d,          incnmaster,             {.i = -1 } },
	{ MODKEY,                       XK_h,          setmfact,               {.f = -0.05} },
	{ MODKEY,                       XK_l,          setmfact,               {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,          setcfact,               {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,          setcfact,               {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,          setcfact,               {0} },
	{ MODKEY|Mod4Mask,              XK_Down,       moveresize,             {.v = "0x 25y 0w 0h" } },
	{ MODKEY|Mod4Mask,              XK_Up,         moveresize,             {.v = "0x -25y 0w 0h" } },
	{ MODKEY|Mod4Mask,              XK_Right,      moveresize,             {.v = "25x 0y 0w 0h" } },
	{ MODKEY|Mod4Mask,              XK_Left,       moveresize,             {.v = "-25x 0y 0w 0h" } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_Down,       moveresize,             {.v = "0x 0y 0w 25h" } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_Up,         moveresize,             {.v = "0x 0y 0w -25h" } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_Right,      moveresize,             {.v = "0x 0y 25w 0h" } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_Left,       moveresize,             {.v = "0x 0y -25w 0h" } },
	{ MODKEY|ShiftMask,             XK_j,          movestack,              {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,          movestack,              {.i = -1 } },
	{ MODKEY,                       XK_Return,     zoom,                   {0} },
	{ Mod4Mask,                     XK_Tab,        spawn,                  {.v = rofitab} },
	{ MODKEY,                       XK_Tab,        view,                   {0} },
	{ MODKEY|ControlMask,           XK_z,          showhideclient,         {0} },
	{ MODKEY|ShiftMask,             XK_c,          killclient,             {0} },
	{ MODKEY|ShiftMask,             XK_q,          quit,                   {0} },
	{ MODKEY,                       XK_t,          setlayout,              {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,          setlayout,              {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,          setlayout,              {.v = &layouts[2]} },
	{ MODKEY,                       XK_u,          setlayout,              {.v = &layouts[3]} },
	{ MODKEY,                       XK_o,          setlayout,              {.v = &layouts[4]} },
	{ MODKEY,                       XK_space,      setlayout,              {0} },
	{ MODKEY|ShiftMask,             XK_space,      togglefloating,         {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_h,          togglehorizontalmax,    {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_l,          togglehorizontalmax,    {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_j,          toggleverticalmax,      {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_k,          toggleverticalmax,      {0} },
	{ MODKEY|ControlMask,           XK_m,          togglemax,              {0} },
	{ MODKEY,                       XK_0,          view,                   {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,          tag,                    {.ui = ~0 } },
	{ MODKEY,                       XK_comma,      focusmon,               {.i = -1 } },
	{ MODKEY,                       XK_period,     focusmon,               {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,      tagmon,                 {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,     tagmon,                 {.i = +1 } },
	TAGKEYS(                        XK_1,                                  0)
	TAGKEYS(                        XK_2,                                  1)
	TAGKEYS(                        XK_3,                                  2)
	TAGKEYS(                        XK_4,                                  3)
	TAGKEYS(                        XK_5,                                  4)
	TAGKEYS(                        XK_6,                                  5)
	TAGKEYS(                        XK_7,                                  6)
	TAGKEYS(                        XK_8,                                  7)
	TAGKEYS(                        XK_9,                                  8)
};


/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask           button          function        argument */
	{ ClkLtSymbol,          0,                   Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,                   Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,                   Button1,        togglewin,      {0} },
	{ ClkWinTitle,          0,                   Button3,        showhideclient, {0} },
	{ ClkWinTitle,          0,                   Button2,        zoom,           {0} },
	{ ClkStatusText,        0,                   Button2,        spawn,          {.v = termcmd } },
	/* placemouse options, choose which feels more natural:
	 *    0 - tiled position is relative to mouse cursor
	 *    1 - tiled postiion is relative to window center
	 *    2 - mouse pointer warps to window center
	 *
	 * The moveorplace uses movemouse or placemouse depending on the floating state
	 * of the selected client. Set up individual keybindings for the two if you want
	 * to control these separately (i.e. to retain the feature to move a tiled window
	 * into a floating position).
	 */
	{ ClkClientWin,         MODKEY,              Button1,        moveorplace,    {.i = 1} },
	{ ClkClientWin,         MODKEY,              Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,              Button3,        resizemouse,    {0} },
	{ ClkClientWin,         MODKEY|ShiftMask,    Button3,        dragcfact,      {0} },
	{ ClkClientWin,         MODKEY|ShiftMask,    Button1,        dragmfact,      {0} },
	{ ClkTagBar,            0,                   Button1,        view,           {0} },
	{ ClkTagBar,            0,                   Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,              Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,              Button3,        toggletag,      {0} },
};
