#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 5;
static const unsigned int snap      = 16;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar				= 1;        /* 0 means bottom bar */
static const int vertpad			= 0;		/* padding for topbar */
static const int sidepad			= 0;
static const char *fonts[]          = { "Terminus:size=20", "Font Awesome:size:20" };
//static const char *fonts[]          = { "Orbitron:size=16", "Rajdhani:size:16" };
static const char dmenufont[]       = "Terminus:size=20";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#333333";
static const char col_cyan[]        = "#5ef6ff";
static const char col_red[]			= "#ff0000";
static const char col_menu_red[]	= "#491d23";
static const char col_selected_red[]	= "#6c2224";
static const char col_background_red[]	= "#260e12";
static const char col_dark_blue[]	= "#0c0c19";
static const char col_green[]		= "#00ff00";
static const char col_mag[]			= "#d300c4";
static const char col_orange[]		= "#f57800";
static const unsigned int baralpha = OPAQUE;
static const unsigned int borderalpha = OPAQUE;


static const char *colors[][3]      = {
	/*               fg         bg         border   */
    //	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeNorm] = { col_cyan, col_gray1, col_menu_red },
	[SchemeSel]  = { col_gray4, col_cyan,  col_red  }, //window border
	[SchemeStatus]  = { col_cyan, col_background_red,  col_gray2  }, // Statusbar right {text,background,not used but cannot be empty}
	// [SchemeTagsSel]  = { col_gray4, col_cyan,  col_gray2  }, // Tagbar left selected {text,background,not used but cannot be empty}
	[SchemeTagsSel]  = { col_cyan, col_selected_red,  col_gray2  }, // Tagbar left selected {text,background,not used but cannot be empty}
    [SchemeTagsNorm]  = { col_cyan, col_background_red,  col_gray2  }, // Tagbar left unselected {text,background,not used but cannot be empty}
    [SchemeInfoSel]  = { col_cyan, col_selected_red,  col_gray2  }, // infobar middle  selected {text,background,not used but cannot be empty}
    [SchemeInfoNorm]  = { col_cyan, col_background_red,  col_gray2  }, // infobar middle  unselected {text,background,not used but cannot be empty}
};

static const unsigned int alphas[][3]	= {
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },

	[SchemeStatus] = { OPAQUE, baralpha, borderalpha },
	[SchemeTagsSel] = { OPAQUE, baralpha, borderalpha },
	[SchemeTagsNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeInfoSel] = { OPAQUE, baralpha, borderalpha },
	[SchemeInfoNorm] = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
// Other fun icons: , , , ﴣ
//static const char *tags[] = { "", "ﲤ", "ﰁ", "", "", "", "", "", "阮"};
static const char *tags[] = { "ﲵ", "", "﬙", "", "", "", "", "", "ﴣ"};
static const char *tagsalt[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9"};
static const int momentaryalttags = 0; // 1 means alttags only show while key is help


static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "",      tile },    /* first entry is default */
	{ "",      NULL },    /* no layout function means floating behavior */
	{ "",      monocle },
	{ "",	  spiral },
	{ "[\\]",	  dwindle },
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

static const char dmenu_hp_list[] = "steam,spotify,discord,xgalaga++,qbwttorrent,qutebrowser,virt-manager";

static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-c", "-m", dmenumon, "-p", "run:", "-fn", dmenufont, "-nb", col_dark_blue, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, "-hb", "#f57800", "-hf", "#333333", "-hp", dmenu_hp_list, NULL };
//static const char *termcmd[]  = { "urxvt", "+sb", NULL };
static const char *termcmd[]  = { "kitty", NULL };
static const char *brave[] = {"brave", NULL};
static const char *librewolf[] = {"librewolf", NULL};
static const char *ranger[] = {"kitty", "-e", "ranger", NULL};
static const char *mutevol[] = { "pactl", "set-sink-mute", "0", "toggle", NULL };
static const char *upvol[] = { "pactl", "set-sink-volume", "0", "+5%", NULL };
static const char *downvol[] = { "pactl", "set-sink-volume", "0", "-5%", NULL };
static const char *up_bright[] = {"light", "-A", "5", NULL};
static const char *down_bright[] = {"light", "-U", "5", NULL};
static const char *audio_back[] = {"playerctl", "previous", NULL};
static const char *audio_play[] = {"playerctl", "play-pause", NULL};
static const char *audio_stop[] = {"playerctl", "stop", NULL};
static const char *audio_next[] = {"playerctl", "next", NULL};

static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = {"alacritty", "-t", "Scratch", "-o", "window.dimensions.columns=68", "window.dimensions.lines=22", NULL};

#include "shift-tools.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_r,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,						XK_o,      togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY|ShiftMask,				XK_b,	   spawn,	       {.v = brave} },
	{ MODKEY|ShiftMask,             XK_f,      spawn,          {.v = librewolf } },
	{ MODKEY|ShiftMask,             XK_t,      spawn,          {.v = ranger } },
	{ 0,				            XF86XK_MonBrightnessUp,         spawn,	       { .v = up_bright} },
	{ 0,						    XF86XK_MonBrightnessDown,       spawn,	       { .v = down_bright} },  
	{ 0,                       		XF86XK_AudioLowerVolume, spawn, {.v = downvol } },
	{ 0,                       		XF86XK_AudioMute, spawn, {.v = mutevol } },
	{ 0,                       		XF86XK_AudioRaiseVolume, spawn, {.v = upvol   } },	
    { Mod1Mask,               XK_h,      spawn,          {.v = audio_back} },
    { Mod1Mask,               XK_j,      spawn,          {.v = audio_play} },
    { Mod1Mask,               XK_k,      spawn,          {.v = audio_stop} },
    { Mod1Mask,               XK_l,      spawn,          {.v = audio_next} },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,	                    XK_c,      killclient,     {0} },
	{ MODKEY|ControlMask,           XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ControlMask,           XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ControlMask,           XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ControlMask,			XK_u,      setlayout,	   {.v = &layouts[3]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,						XK_n,	   togglealttag,   {0} },
	{ MODKEY,						XK_q,	   shiftview,	   {.i = -1} },
	{ MODKEY,						XK_w,	   shiftview,	   {.i = +1} },
	{ MODKEY|ShiftMask,				XK_q,	   shifttag,	   {.i = -1} },
	{ MODKEY|ShiftMask,				XK_w,	   shifttag,	   {.i = +1} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ControlMask,           XK_q,      quit,           {0} },
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

