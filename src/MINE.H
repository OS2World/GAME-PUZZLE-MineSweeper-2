/* Mine Sweeper/2 - Resource and menu IDs */

#ifndef MINE_H
#define MINE_H

/* Window / dialog IDs */
#define ID_WINDOW        99
#define IDD_ABOUT        1100

/* Bitmap resource IDs */
#define BM_0             100
#define BM_1             101
#define BM_2             102
#define BM_3             103
#define BM_4             104
#define BM_5             105
#define BM_6             106
#define BM_7             107
#define BM_8             108
#define BM_9             109
#define BM_BOMB          110
#define BM_CHECK         111
#define BM_ILL           112
#define BM_FAIL          113
#define BM_FACED         114
#define BM_FACE1         115
#define BM_FACE2         116
#define BM_FACE3         117
#define BM_FACE4         118
#define BM_ASK1          119
#define BM_ASK2          120
#define BM_EMPTY         121
#define BM_B1            122
#define BM_B2            123
#define BM_B3            124
#define BM_B4            125
#define BM_B5            126
#define BM_B6            127
#define BM_B7            128
#define BM_B8            129
#define BM_LAST          129

/* Menu IDs - Game: 100-199 */
#define IDM_NEWGAME      100
#define IDM_PAUSE        101
#define IDM_QUIT         102
#define IDM_EXIT         104

/* Menu IDs - Options: 200-299 */
#define IDM_NOVICE       200
#define IDM_NORMAL       201
#define IDM_PROFY        202
#define IDM_TIMER        210
#define IDM_BACKGRND     220
#define IDM_FRAME        221
#define IDM_SAVEONEXIT   230

/* Menu IDs - Language: 300-399 */
#define IDM_LANG_EN      300
#define IDM_LANG_ES      301
#define IDM_LANG_NL      302
#define IDM_LANG_DE      303
#define IDM_LANG_FR      304
#define IDM_LANG_IT      305

/* Menu IDs - Help: 900-999 */
#define IDM_ABOUT        999

/* Submenu cascade IDs */
#define IDM_SUBMENU_GAME    1000
#define IDM_SUBMENU_OPTIONS 1001
#define IDM_SUBMENU_LEVEL   1002
#define IDM_SUBMENU_LANG    1003
#define IDM_SUBMENU_HELP    1004

#ifndef RC_INVOKED
INT  main(VOID);
MRESULT EXPENTRY AboutDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
#endif

#endif
