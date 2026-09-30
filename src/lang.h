/* Mine Sweeper/2 -- Language support */

#ifndef LANG_H
#define LANG_H

#define LANG_EN    0
#define LANG_ES    1
#define LANG_NL    2
#define LANG_DE    3
#define LANG_FR    4
#define LANG_IT    5
#define LANG_COUNT 6

enum {
    /* Menu */
    STR_MENU_GAME = 0,
    STR_MENU_NEWGAME,
    STR_MENU_PAUSE,
    STR_MENU_QUIT,
    STR_MENU_EXIT,
    STR_MENU_OPTIONS,
    STR_MENU_LEVEL,
    STR_MENU_NOVICE,
    STR_MENU_NORMAL,
    STR_MENU_PROFY,
    STR_MENU_TIMER,
    STR_MENU_LANGUAGE,
    STR_MENU_BACKGRND,
    STR_MENU_FRAME,
    STR_MENU_SAVEONEXIT,
    STR_MENU_HELP,
    STR_MENU_ABOUT,
    /* App */
    STR_TITLE,
    STR_WIN_GAMEOVER,
    STR_WIN_SUCCESS,
    STR_DLG_TIMEOUT_BODY,
    STR_DLG_TIMEOUT_TITLE,
    STR_DLG_SUCCESS_BODY,
    STR_DLG_SUCCESS_TITLE,
    STR_LEVEL_EASY,
    STR_LEVEL_NORM,
    STR_LEVEL_PROF,
    STR_COUNT
};

extern int current_lang;
extern const char * const lang_strings[LANG_COUNT][STR_COUNT];
#define tr(id) ((char*)lang_strings[current_lang][(id)])

#endif
