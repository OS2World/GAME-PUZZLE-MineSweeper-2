/* Mine Sweeper/2 for OS/2 -- OpenWatcom port
   Original (c) 1999 Dmitry Zaharov
   Port (c) 2026 OS2World
   License: BSD 3-Clause
*/

#define INCL_WIN
#define INCL_GPI
#define INCL_DOS

#include <os2.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include "mine.h"
#include "lang.h"

/* Display scale: 2 = double all pixel dimensions */
#define S  2
#define CS (16*S)   /* cell pixel size  (32) */
#define TS  (8*S)   /* border tile size (16) */

/* Draw a bitmap stretched to (w x h) pixels at (x, y) */
static void BmpDraw(HPS hps, HBITMAP bmp, LONG x, LONG y, LONG w, LONG h)
{
    POINTL p[2];
    p[0].x = x;   p[0].y = y;
    p[1].x = x+w; p[1].y = y+h;
    WinDrawBitmap(hps, bmp, NULL, p, 0, 0, DBM_STRETCH);
}

/* BLDLEVEL */
static const char bldlevel[] =
    "@#Dmitry Zaharov:1.5#@##1## 30 Sep 2026 00:00:00      "
    "ARCAOS:::0::::@@Mine Sweeper/2 for OS/2\r\n\x1a";

/* Language table */
const char * const lang_strings[LANG_COUNT][STR_COUNT] = {
  /* LANG_EN */ {
    "~Game",                         /* STR_MENU_GAME       */
    "~New Game\tCtrl+N",             /* STR_MENU_NEWGAME    */
    "~Pause Game\tCtrl+P",           /* STR_MENU_PAUSE      */
    "~Quit Game\tCtrl+Q",            /* STR_MENU_QUIT       */
    "E~xit\tCtrl+X",                 /* STR_MENU_EXIT       */
    "~Options",                      /* STR_MENU_OPTIONS    */
    "~Level",                        /* STR_MENU_LEVEL      */
    "N~ovice",                       /* STR_MENU_NOVICE     */
    "N~ormal",                       /* STR_MENU_NORMAL     */
    "~Profy",                        /* STR_MENU_PROFY      */
    "~Timer",                        /* STR_MENU_TIMER      */
    "~Language",                     /* STR_MENU_LANGUAGE   */
    "~Background Run\tCtrl+B",       /* STR_MENU_BACKGRND   */
    "~Frame Controls\tCtrl+F",       /* STR_MENU_FRAME      */
    "~Save settings on exit",        /* STR_MENU_SAVEONEXIT */
    "~Help",                         /* STR_MENU_HELP       */
    "~About...",                     /* STR_MENU_ABOUT      */
    "Mine Sweeper/2",                /* STR_TITLE           */
    "Mine Sweeper/2 - GAME OVER",    /* STR_WIN_GAMEOVER    */
    "Mine Sweeper/2 - SUCCESSFULL!", /* STR_WIN_SUCCESS     */
    "Time-delay bomb blow-up!",      /* STR_DLG_TIMEOUT_BODY  */
    "GAME OVER",                     /* STR_DLG_TIMEOUT_TITLE */
    "You safe all mines!",           /* STR_DLG_SUCCESS_BODY  */
    "Information",                   /* STR_DLG_SUCCESS_TITLE */
    "EASY  ",                        /* STR_LEVEL_EASY      */
    "NORMAL",                        /* STR_LEVEL_NORM      */
    "PROFY ",                        /* STR_LEVEL_PROF      */
  },
  /* LANG_ES */ {
    "~Juego",
    "~Nueva partida\tCtrl+N",
    "~Pausar juego\tCtrl+P",
    "~Abandonar juego\tCtrl+Q",
    "S~alir\tCtrl+X",
    "~Opciones",
    "~Nivel",
    "N~ovato",
    "N~ormal",
    "~Profy",
    "~Temporizador",
    "~Idioma",
    "~Fondo Activo\tCtrl+B",
    "~Controles de ventana\tCtrl+F",
    "~Guardar ajustes al salir",
    "~Ayuda",
    "~Acerca de...",
    "Mine Sweeper/2",
    "Mine Sweeper/2 - FIN DEL JUEGO",
    "Mine Sweeper/2 - EXITO!",
    "Bomba de tiempo exploto!",
    "FIN DEL JUEGO",
    "Marcaste todas las minas!",
    "Informacion",
    "FACIL ",
    "NORMAL",
    "PROFY ",
  },
  /* LANG_NL */ {
    "~Spel",
    "~Nieuw spel\tCtrl+N",
    "~Pauze spel\tCtrl+P",
    "~Spel stoppen\tCtrl+Q",
    "A~fsluiten\tCtrl+X",
    "~Opties",
    "~Niveau",
    "B~eginner",
    "N~ormaal",
    "~Profy",
    "~Timer",
    "~Taal",
    "~Achtergrond Actief\tCtrl+B",
    "~Vensterbediening\tCtrl+F",
    "~Instellingen bewaren",
    "~Help",
    "~Over...",
    "Mine Sweeper/2",
    "Mine Sweeper/2 - SPEL VOORBIJ",
    "Mine Sweeper/2 - GEWONNEN!",
    "Tijdbom ontploft!",
    "SPEL VOORBIJ",
    "Je hebt alle mijnen gevonden!",
    "Informatie",
    "MAKKELIJK",
    "NORMAAL",
    "PROFY ",
  },
  /* LANG_DE */ {
    "~Spiel",
    "~Neues Spiel\tStrg+N",
    "~Pause\tStrg+P",
    "Spiel ~beenden\tStrg+Q",
    "~Beenden\tStrg+X",
    "~Optionen",
    "~Schwierigkeit",
    "A~nfaenger",
    "N~ormal",
    "~Profy",
    "~Zeituhr",
    "~Sprache",
    "~Hintergrundlauf\tStrg+B",
    "~Rahmensteuerung\tStrg+F",
    "~Einstellungen speichern",
    "~Hilfe",
    "~Ueber...",
    "Mine Sweeper/2",
    "Mine Sweeper/2 - SPIEL VORBEI",
    "Mine Sweeper/2 - GESCHAFFT!",
    "Zeitbombe explodiert!",
    "SPIEL VORBEI",
    "Alle Minen markiert!",
    "Information",
    "LEICHT ",
    "NORMAL ",
    "PROFY  ",
  },
  /* LANG_FR */ {
    "~Jeu",
    "~Nouveau jeu\tCtrl+N",
    "~Pause jeu\tCtrl+P",
    "A~bandonner\tCtrl+Q",
    "~Quitter\tCtrl+X",
    "~Options",
    "~Niveau",
    "D~ebutant",
    "N~ormal",
    "~Profy",
    "~Minuterie",
    "~Langue",
    "~Arriere-plan Actif\tCtrl+B",
    "~Controles de fenetre\tCtrl+F",
    "~Sauvegarder les options",
    "~Aide",
    "~A propos...",
    "Mine Sweeper/2",
    "Mine Sweeper/2 - FIN DU JEU",
    "Mine Sweeper/2 - BRAVO!",
    "Bombe a retardement!",
    "FIN DU JEU",
    "Toutes les mines trouvees!",
    "Information",
    "FACILE",
    "NORMAL",
    "PROFY ",
  },
  /* LANG_IT */ {
    "~Gioco",
    "~Nuova partita\tCtrl+N",
    "~Pausa gioco\tCtrl+P",
    "A~bbandonare\tCtrl+Q",
    "~Esci\tCtrl+X",
    "~Opzioni",
    "~Livello",
    "P~rincipiante",
    "N~ormale",
    "~Profy",
    "~Timer",
    "~Lingua",
    "~Sfondo Attivo\tCtrl+B",
    "~Controlli cornice\tCtrl+F",
    "~Salva opzioni all'uscita",
    "~Guida",
    "~Informazioni...",
    "Mine Sweeper/2",
    "Mine Sweeper/2 - PARTITA FINITA",
    "Mine Sweeper/2 - OTTIMO!",
    "Bomba a orologeria!",
    "PARTITA FINITA",
    "Hai trovato tutte le mine!",
    "Informazione",
    "FACILE",
    "NORMALE",
    "PROFY  ",
  },
};

/* Globals */
HAB     hab;
HWND    hwndClient, hwndFrame, hwndMenu;
HBITMAP bitmaps[BM_LAST-BM_0+1];
CHAR    pull[32][20];
CHAR    mark[32][20];
CHAR    fill[32][20];
CHAR    show[32][20];
INT     gameover=0, gamePaused=0;
INT     bombs, XS=16, YS=16, bomborigin;
ULONG   timer_count=0;
LONG    lMenuHeight;

/* Settings */
int     saveonexit  = 1;
int     current_lang = LANG_EN;
int     detaillevel = 1;
int     bShowTimer  = 1;
int     bBackgrndRun = 0;
int     bFocusPaused = 0;

/* Frame controls */
HWND    hwndTitleBar = NULLHANDLE;
HWND    hwndSysMenu  = NULLHANDLE;
HWND    hwndMinMax   = NULLHANDLE;
HWND    hwndMenuBar  = NULLHANDLE;
HWND    hwndObject   = NULLHANDLE;
int     bFrameHidden = 0;

/* ------------------------------------------------------------------ */
/* Settings persistence                                                */
/* ------------------------------------------------------------------ */

#define CFGFILE "MINE.cfg"

static void load_settings(void)
{
    FILE *f = fopen(CFGFILE, "rb");
    if (!f) return;
    fread(&saveonexit,   sizeof(int), 1, f);
    fread(&detaillevel,  sizeof(int), 1, f);
    fread(&current_lang, sizeof(int), 1, f);
    fread(&bShowTimer,   sizeof(int), 1, f);
    fread(&bBackgrndRun, sizeof(int), 1, f);
    fclose(f);
    if (current_lang < 0 || current_lang >= LANG_COUNT) current_lang = LANG_EN;
    if (detaillevel  < 0 || detaillevel  > 2)           detaillevel  = 1;
}

static void save_settings(void)
{
    FILE *f = fopen(CFGFILE, "wb");
    if (!f) return;
    fwrite(&saveonexit,   sizeof(int), 1, f);
    fwrite(&detaillevel,  sizeof(int), 1, f);
    fwrite(&current_lang, sizeof(int), 1, f);
    fwrite(&bShowTimer,   sizeof(int), 1, f);
    fwrite(&bBackgrndRun, sizeof(int), 1, f);
    fclose(f);
}

/* ------------------------------------------------------------------ */
/* Frame controls toggle                                               */
/* ------------------------------------------------------------------ */

static void ToggleFrame(void)
{
    LONG h;
    HWND park = bFrameHidden ? hwndFrame : hwndObject;
    WinSetParent(hwndTitleBar, park, FALSE);
    WinSetParent(hwndSysMenu,  park, FALSE);
    WinSetParent(hwndMinMax,   park, FALSE);
    WinSetParent(hwndMenuBar,  park, FALSE);
    bFrameHidden = !bFrameHidden;
    WinCheckMenuItem(hwndMenu, IDM_FRAME, bFrameHidden);
    WinSendMsg(hwndFrame, WM_UPDATEFRAME,
               (MPARAM)(FCF_TITLEBAR|FCF_SYSMENU|FCF_MINBUTTON|FCF_MENU), NULL);
    h = bFrameHidden ? YS*CS+6*TS-1 : YS*CS+6*TS-1+lMenuHeight;
    WinSetWindowPos(hwndFrame, HWND_TOP, 0, 0,
                    XS*CS+2*TS+2, h, SWP_SIZE);
    WinInvalidateRect(hwndFrame, NULL, TRUE);
    WinUpdateWindow(hwndFrame);
}

/* ------------------------------------------------------------------ */
/* Language support                                                    */
/* ------------------------------------------------------------------ */

static void set_language(int lang)
{
    MENUITEM mi;
    HWND     hwndSub;
    int      i;

    current_lang = lang;

    /* top-level menus */
    WinSendMsg(hwndMenu, MM_SETITEMTEXT,
               MPFROMSHORT(IDM_SUBMENU_GAME),    (MPARAM)tr(STR_MENU_GAME));
    WinSendMsg(hwndMenu, MM_SETITEMTEXT,
               MPFROMSHORT(IDM_SUBMENU_OPTIONS), (MPARAM)tr(STR_MENU_OPTIONS));
    WinSendMsg(hwndMenu, MM_SETITEMTEXT,
               MPFROMSHORT(IDM_SUBMENU_HELP),    (MPARAM)tr(STR_MENU_HELP));

    /* Game submenu */
    if (WinSendMsg(hwndMenu, MM_QUERYITEM,
                   MPFROM2SHORT(IDM_SUBMENU_GAME, TRUE), (MPARAM)&mi))
    {
        hwndSub = mi.hwndSubMenu;
        WinSendMsg(hwndSub, MM_SETITEMTEXT, MPFROMSHORT(IDM_NEWGAME), (MPARAM)tr(STR_MENU_NEWGAME));
        WinSendMsg(hwndSub, MM_SETITEMTEXT, MPFROMSHORT(IDM_PAUSE),   (MPARAM)tr(STR_MENU_PAUSE));
        WinSendMsg(hwndSub, MM_SETITEMTEXT, MPFROMSHORT(IDM_QUIT),    (MPARAM)tr(STR_MENU_QUIT));
        WinSendMsg(hwndSub, MM_SETITEMTEXT, MPFROMSHORT(IDM_EXIT),    (MPARAM)tr(STR_MENU_EXIT));
    }

    /* Options submenu */
    if (WinSendMsg(hwndMenu, MM_QUERYITEM,
                   MPFROM2SHORT(IDM_SUBMENU_OPTIONS, TRUE), (MPARAM)&mi))
    {
        hwndSub = mi.hwndSubMenu;
        WinSendMsg(hwndSub, MM_SETITEMTEXT, MPFROMSHORT(IDM_SUBMENU_LEVEL), (MPARAM)tr(STR_MENU_LEVEL));
        WinSendMsg(hwndSub, MM_SETITEMTEXT, MPFROMSHORT(IDM_TIMER),         (MPARAM)tr(STR_MENU_TIMER));
        WinSendMsg(hwndSub, MM_SETITEMTEXT, MPFROMSHORT(IDM_SUBMENU_LANG),  (MPARAM)tr(STR_MENU_LANGUAGE));
        WinSendMsg(hwndSub, MM_SETITEMTEXT, MPFROMSHORT(IDM_BACKGRND),      (MPARAM)tr(STR_MENU_BACKGRND));
        WinSendMsg(hwndSub, MM_SETITEMTEXT, MPFROMSHORT(IDM_FRAME),         (MPARAM)tr(STR_MENU_FRAME));
        WinSendMsg(hwndSub, MM_SETITEMTEXT, MPFROMSHORT(IDM_SAVEONEXIT),    (MPARAM)tr(STR_MENU_SAVEONEXIT));
    }

    /* Level submenu */
    if (WinSendMsg(hwndMenu, MM_QUERYITEM,
                   MPFROM2SHORT(IDM_SUBMENU_LEVEL, TRUE), (MPARAM)&mi))
    {
        hwndSub = mi.hwndSubMenu;
        WinSendMsg(hwndSub, MM_SETITEMTEXT, MPFROMSHORT(IDM_NOVICE), (MPARAM)tr(STR_MENU_NOVICE));
        WinSendMsg(hwndSub, MM_SETITEMTEXT, MPFROMSHORT(IDM_NORMAL), (MPARAM)tr(STR_MENU_NORMAL));
        WinSendMsg(hwndSub, MM_SETITEMTEXT, MPFROMSHORT(IDM_PROFY),  (MPARAM)tr(STR_MENU_PROFY));
    }

    /* Help submenu */
    if (WinSendMsg(hwndMenu, MM_QUERYITEM,
                   MPFROM2SHORT(IDM_SUBMENU_HELP, TRUE), (MPARAM)&mi))
    {
        hwndSub = mi.hwndSubMenu;
        WinSendMsg(hwndSub, MM_SETITEMTEXT, MPFROMSHORT(IDM_ABOUT), (MPARAM)tr(STR_MENU_ABOUT));
    }

    /* Language checkmarks */
    for (i = IDM_LANG_EN; i <= IDM_LANG_IT; i++)
        WinCheckMenuItem(hwndMenu, i, (i == IDM_LANG_EN + lang));

    /* Window title */
    WinSetWindowText(hwndFrame, tr(STR_TITLE));
}

/* ------------------------------------------------------------------ */
/* Game logic (preserved from original)                                */
/* ------------------------------------------------------------------ */

void seet(int count)
{
    int x, y;
    srand((unsigned)clock());
    memset(&pull[0][0], 0, 640);
    memset(&mark[0][0], 0, 640);
    memset(&show[0][0], 0, 640);
    bombs = count; bomborigin = bombs;
    while (count > 0) {
        x = rand() % XS;
        y = rand() % YS;
        if (!pull[x][y]) { pull[x][y] = 1; count--; }
    }
}

int counter(int x, int y)
{
    int c=0, i, j;
    for (i=0; i<3; i++)
        for (j=0; j<3; j++) {
            if (((x+i-1)>=0) && ((x+i-1)<XS) && ((y+j-1)>=0) && ((y+j-1)<YS) &&
                (pull[x+i-1][y+j-1]==1)) c++;
        }
    return c;
}

int calcbombs(void)
{
    int i, j, a=0;
    for (i=0; i<XS; i++) for (j=0; j<YS; j++) if (mark[i][j]) a++;
    return (bomborigin - a);
}

void WriteString(HPS hps, int x, int y, char *s, int fg, int bg)
{
    POINTL pt;
    FATTRS fat;
    pt.x = x; pt.y = y;
    GpiSetColor(hps, fg);
    GpiSetBackColor(hps, bg);
    GpiSetBackMix(hps, BM_OVERPAINT);
    fat.usRecordLength = sizeof(FATTRS);
    fat.fsSelection    = 0;
    fat.lMatch         = 0L;
    fat.idRegistry     = 0;
    fat.usCodePage     = 850;
    fat.lMaxBaselineExt = 18L * S;
    fat.lAveCharWidth  = 8L  * S;
    fat.fsType         = 0;
    fat.fsFontUse      = 0;
    strcpy(fat.szFacename, "System VIO");
    GpiCreateLogFont(hps, NULL, 1L, &fat);
    GpiSetCharSet(hps, 1L);
    GpiCharStringAt(hps, &pt, (LONG)strlen(s), s);
    GpiDeleteSetId(hps, 1L);
}

void showtimer(HPS hps)
{
    char buf[20];
    sprintf(buf, "%ld:%02ld    ", timer_count/60, timer_count%60);
    WriteString(hps, 20, YS*CS+44, buf, CLR_YELLOW, CLR_BLACK);
}

void content(HWND hwnd, int p)
{
    CHAR   buf[10];
    HPS    hps;
    INT    i, j;
    POINTL pt;
    RECTL  rc;

    if (!p) hps = WinGetPS(hwnd);
    else    hps = WinBeginPaint(hwnd, 0L, &rc);

    for (i=0; i<YS*2+4; i++) {
        BmpDraw(hps, bitmaps[BM_B2-BM_0], 0,        TS+i*TS, TS, TS);
        BmpDraw(hps, bitmaps[BM_B2-BM_0], XS*CS+TS, TS+i*TS, TS, TS);
    }
    for (i=0; i<XS*2; i++) {
        BmpDraw(hps, bitmaps[BM_B6-BM_0], i*TS+TS, 0,         TS, TS);
        BmpDraw(hps, bitmaps[BM_B6-BM_0], i*TS+TS, YS*CS+TS,  TS, TS);
        BmpDraw(hps, bitmaps[BM_B6-BM_0], i*TS+TS, (YS+2)*CS+TS, TS, TS);
    }
    BmpDraw(hps, bitmaps[BM_B5-BM_0], 0,        0,            TS, TS);
    BmpDraw(hps, bitmaps[BM_B4-BM_0], XS*CS+TS, 0,            TS, TS);
    BmpDraw(hps, bitmaps[BM_B3-BM_0], XS*CS+TS, (YS+2)*CS+TS, TS, TS);
    BmpDraw(hps, bitmaps[BM_B1-BM_0], 0,        (YS+2)*CS+TS, TS, TS);
    BmpDraw(hps, bitmaps[BM_B7-BM_0], 0,        YS*CS+TS,     TS, TS);
    BmpDraw(hps, bitmaps[BM_B8-BM_0], XS*CS+TS, YS*CS+TS,     TS, TS);

    pt.x=0; pt.y=YS*CS+2*TS;
    GpiSetCurrentPosition(hps, &pt);
    pt.x=XS*CS+2*TS-1; pt.y=YS*CS+6*TS-1;
    GpiSetColor(hps, CLR_BLACK);
    GpiSetBackColor(hps, CLR_BLACK);
    GpiBox(hps, DRO_FILL, &pt, 0L, 0L);

    if (!bShowTimer) {
        if (XS==8)  WriteString(hps, 20, YS*CS+44, tr(STR_LEVEL_EASY), CLR_GREEN, CLR_BLACK);
        if (XS==16) WriteString(hps, 20, YS*CS+44, tr(STR_LEVEL_NORM), CLR_GREEN, CLR_BLACK);
        if (XS==32) WriteString(hps, 20, YS*CS+44, tr(STR_LEVEL_PROF), CLR_GREEN, CLR_BLACK);
    } else {
        showtimer(hps);
    }

    sprintf(buf, "%03d", bombs);
    WriteString(hps, XS*CS-48, YS*CS+44, buf, CLR_RED, CLR_BLACK);

    for (i=0; i<XS; i++)
        for (j=0; j<YS; j++) {
            if (!show[i][j]) {
                if (!mark[i][j])
                    BmpDraw(hps, bitmaps[BM_EMPTY-BM_0], TS+i*CS, TS+j*CS, CS, CS);
                else
                    BmpDraw(hps, bitmaps[BM_CHECK-BM_0], TS+i*CS, TS+j*CS, CS, CS);
            } else {
                BmpDraw(hps, bitmaps[counter(i,j)], TS+i*CS, TS+j*CS, CS, CS);
            }
        }

    if (!p) WinReleasePS(hps);
    else    WinEndPaint(hps);
}

int checkdone(void)
{
    if (!memcmp(&pull[0][0], &mark[0][0], XS*YS)) return 0;
    else return 1;
}

void failed(int x, int y, HWND hwnd)
{
    HPS    hps;
    int    i, j;
    POINTL pt;
    hps = WinGetPS(hwnd);
    for (i=0; i<XS; i++)
        for (j=0; j<YS; j++) {
            if ((i==x) && (j==y))
                BmpDraw(hps, bitmaps[BM_FAIL-BM_0], TS+i*CS, TS+j*CS, CS, CS);
            else {
                if ((pull[i][j]==0) && (mark[i][j]==1))
                    BmpDraw(hps, bitmaps[BM_ILL-BM_0],  TS+i*CS, TS+j*CS, CS, CS);
                if (pull[i][j]==1)
                    BmpDraw(hps, bitmaps[BM_BOMB-BM_0], TS+i*CS, TS+j*CS, CS, CS);
            }
        }
    WinReleasePS(hps);
}

void fills(int x, int y, HWND hwnd)
{
    int    i, j, o=0, a, b, c;
    POINTL pt;
    HPS    hps;
    memset(&fill[0][0], 0, 640);
    fill[x][y] = 1;
    if (counter(x,y) > 0) {
        show[x][y] = 1;
        hps = WinGetPS(hwnd);
        BmpDraw(hps, bitmaps[counter(x,y)], TS+x*CS, TS+y*CS, CS, CS);
        WinReleasePS(hps);
        return;
    }
    o = 1;
    hps = WinGetPS(hwnd);
    BmpDraw(hps, bitmaps[counter(x,y)], TS+x*CS, TS+y*CS, CS, CS);
    mark[x][y] = 0;
    show[x][y] = 1;
    while (o > 0) {
        o = 0;
        for (i=0; i<XS; i++)
            for (j=0; j<YS; j++) {
                if (fill[i][j]==1) {
                    for (a=0; a<3; a++)
                        for (b=0; b<3; b++) {
                            if (((i+a-1)>=0) && ((i+a-1)<XS) &&
                                ((j+b-1)>=0) && ((j+b-1)<YS) &&
                                !fill[i+a-1][j+b-1])
                            {
                                c = counter(i+a-1, j+b-1);
                                fill[i+a-1][j+b-1] = c+1;
                                show[i+a-1][j+b-1] = 1;
                                BmpDraw(hps, bitmaps[c], TS+(i+a-1)*CS, TS+(j+b-1)*CS, CS, CS);
                                mark[i+a-1][j+b-1] = 0;
                                if (!c) o++;
                            }
                        }
                }
            }
    }
    bombs = calcbombs();
    WinReleasePS(hps);
}

void LoadBitmaps(void)
{
    HPS stubHps;
    int i;
    stubHps = WinGetPS(hwndClient);
    for (i=BM_0; i<=BM_LAST; i++)
        bitmaps[i-BM_0] = GpiLoadBitmap(stubHps, NULLHANDLE, (ULONG)i, 0, 0);
    WinReleasePS(stubHps);
}

/* ------------------------------------------------------------------ */
/* start_game: begin new game at current detaillevel                   */
/* ------------------------------------------------------------------ */

static void start_game(void)
{
    gameover   = 0;
    gamePaused = 0;
    timer_count = 0L;
    WinCheckMenuItem(hwndMenu, IDM_PAUSE, FALSE);
    WinStartTimer(hab, hwndClient, 99, 1000L);

    if (detaillevel == 0) { XS=8;  YS=8;  seet(10);  }
    if (detaillevel == 1) { XS=16; YS=16; seet(40);  }
    if (detaillevel == 2) { XS=32; YS=20; seet(100); }

    WinSetWindowText(hwndFrame, tr(STR_TITLE));

    lMenuHeight = WinQuerySysValue(HWND_DESKTOP, SV_CYMENU) +
                  WinQuerySysValue(HWND_DESKTOP, SV_CYTITLEBAR);

    WinSetWindowPos(hwndFrame, HWND_TOP,
                    0, 0,
                    XS*CS+2*TS+2, YS*CS+6*TS-1+lMenuHeight,
                    SWP_ACTIVATE | SWP_SHOW | SWP_SIZE);

    content(hwndClient, 0);
}

/* ------------------------------------------------------------------ */
/* About dialog procedure                                              */
/* ------------------------------------------------------------------ */

MRESULT EXPENTRY AboutDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    switch (msg) {
        case WM_COMMAND:
            switch (COMMANDMSG(&msg)->cmd) {
                case DID_OK:
                case DID_CANCEL:
                    WinDismissDlg(hwnd, TRUE);
                    return 0L;
            }
    }
    return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

/* ------------------------------------------------------------------ */
/* Window procedure                                                    */
/* ------------------------------------------------------------------ */

MRESULT EXPENTRY MyWindowProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    INT    x=0, y=0;
    HPS    hps;
    POINTL pt;
    CHAR   buf[80];

    switch (msg) {

        case WM_TIMER:
            if (bShowTimer && !gameover && !gamePaused) {
                timer_count += 1L;
                if (timer_count >= 600L) {
                    gameover = 1;
                    WinStopTimer(hab, hwnd, 99);
                    WinMessageBox(HWND_DESKTOP, hwndFrame,
                        tr(STR_DLG_TIMEOUT_BODY),
                        tr(STR_DLG_TIMEOUT_TITLE),
                        0, MB_INFORMATION | MB_OK | MB_MOVEABLE | MB_SYSTEMMODAL);
                    WinSetWindowText(hwndFrame, tr(STR_WIN_GAMEOVER));
                    failed(-1, -1, hwndClient);
                }
                hps = WinGetPS(hwnd);
                showtimer(hps);
                WinReleasePS(hps);
            }
            break;

        case WM_CREATE:
            seet(40);
            gameover = 0;
            WinStartTimer(hab, hwnd, 99, 1000L);
            break;

        case WM_SETFOCUS:
            if (!bBackgrndRun) {
                if (!SHORT1FROMMP(mp1)) {
                    /* losing focus */
                    if (!gameover && !gamePaused) {
                        WinStopTimer(hab, hwnd, 99);
                        gamePaused   = 1;
                        bFocusPaused = 1;
                        WinCheckMenuItem(hwndMenu, IDM_PAUSE, TRUE);
                    }
                } else {
                    /* gaining focus */
                    if (bFocusPaused) {
                        if (!gameover && gamePaused) {
                            WinStartTimer(hab, hwnd, 99, 1000L);
                            gamePaused = 0;
                            WinCheckMenuItem(hwndMenu, IDM_PAUSE, FALSE);
                        }
                        bFocusPaused = 0;
                    }
                }
            }
            break;

        case WM_BUTTON1DBLCLK:
            if (!gameover) break;
            start_game();
            break;

        case WM_BUTTON1DOWN:
            if (gameover || gamePaused) break;
            WinQueryPointerPos(HWND_DESKTOP, &pt);
            WinMapWindowPoints(HWND_DESKTOP, hwndClient, &pt, 1);
            if (pt.x<TS || pt.y<TS || pt.x>XS*CS+TS-1 || pt.y>YS*CS+TS-1) break;
            x = (pt.x-TS)/CS; y = (pt.y-TS)/CS;
            if (show[x][y]) break;
            if (mark[x][y]) break;
            if (pull[x][y]) {
                failed(x, y, hwndClient);
                gameover = 1;
                WinStopTimer(hab, hwnd, 99);
                WinSetWindowText(hwndFrame, tr(STR_WIN_GAMEOVER));
            } else {
                fills(x, y, hwndClient);
            }
            break;

        case WM_BUTTON2DOWN:
            if (gameover || gamePaused) break;
            WinQueryPointerPos(HWND_DESKTOP, &pt);
            WinMapWindowPoints(HWND_DESKTOP, hwndClient, &pt, 1);
            if (pt.x<TS || pt.y<TS || pt.x>XS*CS+TS-1 || pt.y>YS*CS+TS-1) break;
            x = (pt.x-TS)/CS; y = (pt.y-TS)/CS;
            if (show[x][y]) break;
            mark[x][y] = !mark[x][y];
            hps = WinGetPS(hwndClient);
            if (mark[x][y]) {
                BmpDraw(hps, bitmaps[BM_CHECK-BM_0], TS+x*CS, TS+y*CS, CS, CS);
                bombs--;
                sprintf(buf, "%03d", bombs);
                WriteString(hps, XS*CS-48, YS*CS+44, buf, CLR_RED, CLR_BLACK);
                WinSetWindowText(hwndFrame, tr(STR_TITLE));
                if (!bombs) {
                    gameover = 1;
                    WinStopTimer(hab, hwnd, 99);
                    if (!checkdone()) {
                        WinSetWindowText(hwndFrame, tr(STR_WIN_SUCCESS));
                        WinMessageBox(HWND_DESKTOP, hwndFrame,
                            tr(STR_DLG_SUCCESS_BODY),
                            tr(STR_DLG_SUCCESS_TITLE),
                            0, MB_INFORMATION | MB_OK | MB_MOVEABLE | MB_SYSTEMMODAL);
                    } else {
                        WinSetWindowText(hwndFrame, tr(STR_WIN_GAMEOVER));
                        failed(x, y, hwndClient);
                    }
                }
            } else {
                BmpDraw(hps, bitmaps[BM_EMPTY-BM_0], TS+x*CS, TS+y*CS, CS, CS);
                bombs++;
                sprintf(buf, "%03d", bombs);
                WriteString(hps, XS*CS-48, YS*CS+44, buf, CLR_RED, CLR_BLACK);
                WinSetWindowText(hwndFrame, tr(STR_TITLE));
            }
            WinReleasePS(hps);
            break;

        case WM_ERASEBACKGROUND:
            return (MRESULT)TRUE;

        case WM_COMMAND:
            switch (SHORT1FROMMP(mp1)) {

                case IDM_NEWGAME:
                    WinStopTimer(hab, hwnd, 99);
                    start_game();
                    break;

                case IDM_PAUSE:
                    if (gameover) break;
                    gamePaused = !gamePaused;
                    WinCheckMenuItem(hwndMenu, IDM_PAUSE, gamePaused);
                    if (gamePaused)
                        WinStopTimer(hab, hwnd, 99);
                    else
                        WinStartTimer(hab, hwnd, 99, 1000L);
                    break;

                case IDM_QUIT:
                    if (gameover) break;
                    gameover = 1;
                    gamePaused = 0;
                    WinStopTimer(hab, hwnd, 99);
                    WinCheckMenuItem(hwndMenu, IDM_PAUSE, FALSE);
                    WinSetWindowText(hwndFrame, tr(STR_WIN_GAMEOVER));
                    failed(-1, -1, hwndClient);
                    break;

                case IDM_EXIT:
                    if (saveonexit) save_settings();
                    WinPostMsg(hwnd, WM_QUIT, (MPARAM)0, (MPARAM)0);
                    break;

                case IDM_NOVICE:
                    detaillevel = 0;
                    WinCheckMenuItem(hwndMenu, IDM_NOVICE, TRUE);
                    WinCheckMenuItem(hwndMenu, IDM_NORMAL, FALSE);
                    WinCheckMenuItem(hwndMenu, IDM_PROFY,  FALSE);
                    WinStopTimer(hab, hwnd, 99);
                    start_game();
                    break;

                case IDM_NORMAL:
                    detaillevel = 1;
                    WinCheckMenuItem(hwndMenu, IDM_NOVICE, FALSE);
                    WinCheckMenuItem(hwndMenu, IDM_NORMAL, TRUE);
                    WinCheckMenuItem(hwndMenu, IDM_PROFY,  FALSE);
                    WinStopTimer(hab, hwnd, 99);
                    start_game();
                    break;

                case IDM_PROFY:
                    detaillevel = 2;
                    WinCheckMenuItem(hwndMenu, IDM_NOVICE, FALSE);
                    WinCheckMenuItem(hwndMenu, IDM_NORMAL, FALSE);
                    WinCheckMenuItem(hwndMenu, IDM_PROFY,  TRUE);
                    WinStopTimer(hab, hwnd, 99);
                    start_game();
                    break;

                case IDM_TIMER:
                    bShowTimer = !bShowTimer;
                    WinCheckMenuItem(hwndMenu, IDM_TIMER, bShowTimer);
                    content(hwndClient, 0);
                    break;

                case IDM_BACKGRND:
                    bBackgrndRun = !bBackgrndRun;
                    WinCheckMenuItem(hwndMenu, IDM_BACKGRND, bBackgrndRun);
                    break;

                case IDM_FRAME:
                    ToggleFrame();
                    break;

                case IDM_SAVEONEXIT:
                    saveonexit = !saveonexit;
                    WinCheckMenuItem(hwndMenu, IDM_SAVEONEXIT, saveonexit);
                    break;

                case IDM_LANG_EN:
                case IDM_LANG_ES:
                case IDM_LANG_NL:
                case IDM_LANG_DE:
                case IDM_LANG_FR:
                case IDM_LANG_IT:
                    set_language(SHORT1FROMMP(mp1) - IDM_LANG_EN);
                    break;

                case IDM_ABOUT:
                    WinDlgBox(HWND_DESKTOP, hwndFrame, AboutDlgProc,
                              NULLHANDLE, IDD_ABOUT, NULL);
                    break;
            }
            break;

        case WM_PAINT:
            content(hwndClient, 1);
            break;

        case WM_CLOSE:
            if (saveonexit) save_settings();
            WinPostMsg(hwnd, WM_QUIT, (MPARAM)0, (MPARAM)0);
            break;

        default:
            return WinDefWindowProc(hwnd, msg, mp1, mp2);
    }
    return (MRESULT)FALSE;
}

/* ------------------------------------------------------------------ */
/* Frame subclass: pause timer on app deactivation                    */
/* ------------------------------------------------------------------ */

static PFNWP pfnOldFrameProc = NULL;

static MRESULT EXPENTRY SubclassFrameProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    if (msg == WM_ACTIVATE && !bBackgrndRun) {
        if (!SHORT1FROMMP(mp1)) {
            /* app is losing activation */
            if (!gameover && !gamePaused) {
                WinStopTimer(hab, hwndClient, 99);
                gamePaused   = 1;
                bFocusPaused = 1;
                WinCheckMenuItem(hwndMenu, IDM_PAUSE, TRUE);
            }
        } else if (bFocusPaused) {
            /* app is gaining activation; resume if we paused it */
            if (!gameover && gamePaused) {
                WinStartTimer(hab, hwndClient, 99, 1000L);
                gamePaused = 0;
                WinCheckMenuItem(hwndMenu, IDM_PAUSE, FALSE);
            }
            bFocusPaused = 0;
        }
    }
    return pfnOldFrameProc(hwnd, msg, mp1, mp2);
}

/* ------------------------------------------------------------------ */
/* main                                                                */
/* ------------------------------------------------------------------ */

INT main(VOID)
{
    HMQ   hmq;
    QMSG  qmsg;
    ULONG flCreate;

    hab = WinInitialize(0);
    hmq = WinCreateMsgQueue(hab, 0);

    load_settings();

    WinRegisterClass(hab, (PSZ)"MineSweeper2",
                     (PFNWP)MyWindowProc, CS_SIZEREDRAW, 0);

    flCreate = FCF_TASKLIST | FCF_SYSMENU   | FCF_ICON    |
               FCF_MINBUTTON | FCF_MENU     | FCF_TITLEBAR |
               FCF_BORDER;

    hwndFrame = WinCreateStdWindow(
                    HWND_DESKTOP, 0, &flCreate,
                    "MineSweeper2", "",
                    0, (HMODULE)0L, ID_WINDOW, &hwndClient);

    LoadBitmaps();

    lMenuHeight = WinQuerySysValue(HWND_DESKTOP, SV_CYMENU) +
                  WinQuerySysValue(HWND_DESKTOP, SV_CYTITLEBAR);

    hwndMenu = WinWindowFromID(hwndFrame, FID_MENU);

    /* Capture frame-control handles */
    hwndTitleBar = WinWindowFromID(hwndFrame, FID_TITLEBAR);
    hwndSysMenu  = WinWindowFromID(hwndFrame, FID_SYSMENU);
    hwndMinMax   = WinWindowFromID(hwndFrame, FID_MINMAX);
    hwndMenuBar  = WinWindowFromID(hwndFrame, FID_MENU);
    hwndObject   = WinCreateWindow(HWND_OBJECT, WC_FRAME, "",
                       0L, 0,0,0,0, NULLHANDLE, HWND_TOP, 0, NULL, NULL);

    /* Keyboard accelerators */
    WinSetAccelTable(hab,
                     WinLoadAccelTable(hab, NULLHANDLE, ID_WINDOW),
                     hwndFrame);

    /* Frame subclass for app-level activation tracking */
    pfnOldFrameProc = WinSubclassWindow(hwndFrame, SubclassFrameProc);

    /* Apply saved settings to menu checkmarks */
    WinCheckMenuItem(hwndMenu, IDM_SAVEONEXIT, saveonexit);
    WinCheckMenuItem(hwndMenu, IDM_TIMER,      bShowTimer);
    WinCheckMenuItem(hwndMenu, IDM_BACKGRND,   bBackgrndRun);
    switch (detaillevel) {
        case 0:
            WinCheckMenuItem(hwndMenu, IDM_NOVICE, TRUE);
            WinCheckMenuItem(hwndMenu, IDM_NORMAL, FALSE);
            WinCheckMenuItem(hwndMenu, IDM_PROFY,  FALSE);
            break;
        case 2:
            WinCheckMenuItem(hwndMenu, IDM_NOVICE, FALSE);
            WinCheckMenuItem(hwndMenu, IDM_NORMAL, FALSE);
            WinCheckMenuItem(hwndMenu, IDM_PROFY,  TRUE);
            break;
        default:
            WinCheckMenuItem(hwndMenu, IDM_NOVICE, FALSE);
            WinCheckMenuItem(hwndMenu, IDM_NORMAL, TRUE);
            WinCheckMenuItem(hwndMenu, IDM_PROFY,  FALSE);
            break;
    }

    set_language(current_lang);

    /* Show window sized to initial level */
    WinSetWindowPos(hwndFrame, HWND_TOP,
                    100, 60,
                    XS*CS+2*TS+2, YS*CS+6*TS-1+lMenuHeight,
                    SWP_ACTIVATE | SWP_SHOW | SWP_SIZE | SWP_MOVE);

    while (WinGetMsg(hab, &qmsg, 0L, 0, 0))
        WinDispatchMsg(hab, &qmsg);

    if (hwndObject != NULLHANDLE) WinDestroyWindow(hwndObject);
    WinDestroyWindow(hwndFrame);
    WinDestroyMsgQueue(hmq);
    WinTerminate(hab);

    return 1;
}
