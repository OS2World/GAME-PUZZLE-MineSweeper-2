//      ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
//      ³ OS/2 Mine Sweeper (C) Dmitry Zaharov, 1999. All Rights Reserved. ³
//      ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

#define INCL_WIN
#define INCL_GPI
#define INCL_DOS

#include <os2.h>			/* PM header file		*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include "mine.h"			/* Resource symbolic identifiers*/

// ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
// ³ Function Prototypes ³
// ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ


MRESULT EXPENTRY MyWindowProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
VOID		 LoadBitmaps(void);


// ÚÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
// ³ Global vars ³
// ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

HAB	hab;
HWND	hwndClient,hwndFrame;
HBITMAP	bitmaps[BM_LAST-BM_0+1];
CHAR	pull[32][20];
CHAR	mark[32][20];
CHAR	fill[32][20];
CHAR	show[32][20];
INT	gameover=0,bombs,XS=16,YS=16,bomborigin,timer=1;
ULONG	timer_count=0;
LONG	lMenuHight;

// ÚÄÄÄÄÄÄ¿
// ³ main ³
// ÀÄÄÄÄÄÄÙ

INT main (VOID)
{
  HMQ  hmq;				/* Message queue handle		*/
  QMSG qmsg;				/* Message from message queue	*/
  ULONG flCreate;			/* Window creation control flags*/

  hab = WinInitialize(0);

  hmq = WinCreateMsgQueue( hab, 0 );

  WinRegisterClass(			/* Register window class	*/
     hab,				/* Anchor block handle		*/
     (PSZ)"MyWindow",			/* Window class name		*/
     (PFNWP)MyWindowProc,		/* Address of window procedure	*/
     CS_SIZEREDRAW,			/* Class style			*/
     0					/* No extra window words	*/
     );

  flCreate =  FCF_TASKLIST  | FCF_SYSMENU   | FCF_ICON     | 
              FCF_MINBUTTON | FCF_MENU      | FCF_TITLEBAR |
              FCF_BORDER ;

  hwndFrame = WinCreateStdWindow(     
	       HWND_DESKTOP,		/* Desktop window is parent	*/
	       0,			/* STD. window styles		*/
	       &flCreate,		/* Frame control flag		*/
	       "MyWindow",		/* Client window class name	*/
	       "",			/* No window text		*/
	       0,			/* No special class style	*/
	       (HMODULE)0L,		/* Resource is in .EXE file     */
	       ID_WINDOW,		/* Frame window identifier	*/
	       &hwndClient		/* Client window handle		*/
	       );

  LoadBitmaps();

  WinSetWindowText(hwndFrame, "Mine Sweeper/2");

  lMenuHight = WinQuerySysValue(HWND_DESKTOP,SV_CYMENU) +
               WinQuerySysValue(HWND_DESKTOP,SV_CYTITLEBAR);

  WinSetWindowPos( hwndFrame,		/* Shows and activates frame	*/
		   HWND_TOP,
		   100, 60, XS*16+18,
		   YS*16+47+lMenuHight,
		   SWP_ACTIVATE | SWP_SHOW |
		   SWP_SIZE     | SWP_MOVE );

  while( WinGetMsg( hab, &qmsg, 0L, 0, 0 ) )
    WinDispatchMsg( hab, &qmsg );

  WinDestroyWindow(hwndFrame);		 /* Tidy up...			 */
  WinDestroyMsgQueue( hmq );		 /* Tidy up...			 */
  WinTerminate( hab );			 /* Terminate the application	 */

  return( 1 );
} /* End of main */

// ÚÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
// ³ Subroutines ³
// ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

// initializing arrays & global vars

void  seet(int count)
{
   int x,y;

   srand((unsigned)clock());
   memset(&pull[0][0],0,640);
   memset(&mark[0][0],0,640);
   memset(&show[0][0],0,640);
   bombs=count;bomborigin=bombs;
   while(count>0)
      {
         x=rand()%XS;
         y=rand()%YS;
         if(!pull[x][y]){
            pull[x][y]=1;
            count--;
            }

	}
   }

// mine counter near specificied cell

int counter(int x,int y)
{
   int c=0,i,j;
for(i=0;i<3;i++)
   for(j=0;j<3;j++)
      {
         if(((x+i-1)>=0) && ((x+i-1)<XS) && ((y+j-1)>=0) && ((y+j-1)<YS) &&
            (pull[x+i-1][y+j-1]==1))c++;
         }
return c;
}

// total mines conter

int calcbombs()
{
int i,j,a=0;
for(i=0;i<XS;i++) for(j=0;j<YS;j++) if(mark[i][j])a++;
return (bomborigin-a);
}

// string writer

void  WriteString(HPS hps,int x,int y,char *s,int fg,int bg)
{
POINTL pt;
FATTRS fat;
      pt.x = x; pt.y = y;		/* Set the text coordinates,	*/
      GpiSetColor( hps, fg );	       /* colour of the text,	*/
      GpiSetBackColor( hps, bg );  /* its background and	*/
      GpiSetBackMix( hps, BM_OVERPAINT );      /* how it mixes,		*/
					       /* and draw the string...*/
      fat.usRecordLength = sizeof(FATTRS);
      fat.fsSelection = 0;//FATTR_SEL_ITALIC;
      fat.lMatch = 0L;
      fat.idRegistry = 0;
      fat.usCodePage = 866;
      fat.lMaxBaselineExt = 18L;
      fat.lAveCharWidth = 8L;
      fat.fsType = 0;
      fat.fsFontUse = 0;
      strcpy(fat.szFacename,"System VIO");
      GpiCreateLogFont(hps,NULL,1L,&fat);
      GpiSetCharSet(hps,1L);
      GpiCharStringAt( hps, &pt, (LONG)strlen( s ), s );
      GpiDeleteSetId( hps, 1L);
}

// clock

void showtimer(HPS hps)
{
   char buf[20];
   sprintf(buf,"%ld:%02ld    ",timer_count/60,timer_count%60);
   WriteString(hps,10,YS*16+22, buf,CLR_YELLOW,CLR_BLACK);
}

// main paint procedure

void content(HWND hwnd,int p)
{
CHAR buf[10];
HPS hps;
INT i,j;
POINTL pt;
RECTL rc;

/* draw border */
if(!p)hps=WinGetPS(hwnd);
 else hps=WinBeginPaint( hwnd, 0L, &rc );

for(i=0;i<YS*2+4;i++)
   {
      pt.x=0;pt.y=8+i*8;
   WinDrawBitmap(hps, bitmaps[BM_B2-BM_0], NULL, &pt, 0, 0, DBM_NORMAL);
      pt.x=XS*16+8;
   WinDrawBitmap(hps, bitmaps[BM_B2-BM_0], NULL, &pt, 0, 0, DBM_NORMAL);
      }
for(i=0;i<XS*2;i++)
   {
      pt.x=i*8+8;pt.y=0;
   WinDrawBitmap(hps, bitmaps[BM_B6-BM_0], NULL, &pt, 0, 0, DBM_NORMAL);
      pt.y=YS*16+8;
   WinDrawBitmap(hps, bitmaps[BM_B6-BM_0], NULL, &pt, 0, 0, DBM_NORMAL);
      pt.y=(YS+2)*16+8;
   WinDrawBitmap(hps, bitmaps[BM_B6-BM_0], NULL, &pt, 0, 0, DBM_NORMAL);
      }
pt.x=0;pt.y=0;
WinDrawBitmap(hps, bitmaps[BM_B5-BM_0], NULL, &pt, 0, 0, DBM_NORMAL);
pt.x=XS*16+8;
WinDrawBitmap(hps, bitmaps[BM_B4-BM_0], NULL, &pt, 0, 0, DBM_NORMAL);
pt.y=(YS+2)*16+8;
WinDrawBitmap(hps, bitmaps[BM_B3-BM_0], NULL, &pt, 0, 0, DBM_NORMAL);
pt.x=0;
WinDrawBitmap(hps, bitmaps[BM_B1-BM_0], NULL, &pt, 0, 0, DBM_NORMAL);
pt.y=YS*16+8;
WinDrawBitmap(hps, bitmaps[BM_B7-BM_0], NULL, &pt, 0, 0, DBM_NORMAL);
pt.x=XS*16+8;
WinDrawBitmap(hps, bitmaps[BM_B8-BM_0], NULL, &pt, 0, 0, DBM_NORMAL);
pt.x=8;
pt.y=YS*16+16;
GpiSetCurrentPosition(hps,&pt);
pt.x=7+XS*16;
pt.y=YS*16+39;
GpiSetColor(hps,CLR_BLACK);
GpiSetBackColor(hps,CLR_BLACK);
GpiBox(hps,DRO_FILL,&pt,0L,0L);
if(!timer)
   {
if(XS==8)WriteString(hps,10,YS*16+22, "EASY  ",CLR_GREEN,CLR_BLACK);
if(XS==16)WriteString(hps,10,YS*16+22,"NORMAL",CLR_GREEN,CLR_BLACK);
if(XS==32)WriteString(hps,10,YS*16+22,"PROFY ",CLR_GREEN,CLR_BLACK);
   }else showtimer(hps);
sprintf(buf,"%03d",bombs);
WriteString(hps,XS*16-24,YS*16+22,buf,CLR_RED,CLR_BLACK);
for(i=0;i<XS;i++)
for(j=0;j<YS;j++)
   {
      pt.x=8+i*16;pt.y=8+j*16;
if(!show[i][j])
   {
      if(!mark[i][j])
WinDrawBitmap(hps, bitmaps[BM_EMPTY-BM_0], NULL, &pt, 0, 0, DBM_NORMAL);
else WinDrawBitmap(hps, bitmaps[BM_CHECK-BM_0], NULL, &pt, 0, 0, DBM_NORMAL);
   }
         else        WinDrawBitmap(hps, bitmaps[counter(i,j)], NULL, &pt, 0, 0, DBM_NORMAL);
}
if(!p)WinReleasePS(hps);
 else WinEndPaint(hps);

}

// all safe checking

int checkdone()
{
   if(!memcmp(&pull[0][0],&mark[0][0],XS*YS))return 0;
      else return 1;
}

// gameover!

void failed(int x,int y,HWND hwnd)
{
HPS hps;
int i,j;
POINTL pt;
hps=WinGetPS(hwnd);
for(i=0;i<XS;i++)
for(j=0;j<YS;j++)
   {
pt.x=8+i*16;
pt.y=8+j*16;
if((i==x) && (j==y))WinDrawBitmap(hps, bitmaps[BM_FAIL-BM_0], NULL, &pt, 0, 0, DBM_NORMAL);
   else {
      if((pull[i][j]==0)&& (mark[i][j]==1))
	 WinDrawBitmap(hps, bitmaps[BM_ILL-BM_0], NULL, &pt, 0, 0, DBM_NORMAL);
      if(pull[i][j]==1)
         WinDrawBitmap(hps, bitmaps[BM_BOMB-BM_0], NULL, &pt, 0, 0, DBM_NORMAL);
      }
}
WinReleasePS(hps);
}

// filler

void fills(int x,int y,HWND hwnd)
{                                   
   int i,j,o=0,a,b,c;
   POINTL pt;
   HPS hps;
   memset(&fill[0][0],0,640);
   fill[x][y]=1;
if(counter(x,y)>0){
   pt.x=8+x*16;pt.y=8+y*16;
   show[x][y]=1;
   hps=WinGetPS(hwnd);
   WinDrawBitmap( hps, bitmaps[counter(x,y)], NULL, &pt, 0, 0, DBM_NORMAL );
   WinReleasePS(hps);
   return;
   }
   o=1;
   hps=WinGetPS(hwnd);
   pt.x=8+x*16;pt.y=8+y*16;  
   WinDrawBitmap( hps, bitmaps[counter(x,y)], NULL, &pt, 0, 0, DBM_NORMAL );
   mark[x][y]=0;
   show[x][y]=1;
   while(o>0)
   {
         o=0;
      for(i=0;i<XS;i++)
      for(j=0;j<YS;j++)
      {
          if(fill[i][j]==1)
          {
          for(a=0;a<3;a++)
          for(b=0;b<3;b++)
          {
             if(((i+a-1)>=0)&&((i+a-1)<XS)&&((j+b-1)>=0)&&((j+b-1)<YS)&&
                !fill[i+a-1][j+b-1])
		{
                c=counter(i+a-1,j+b-1);
                fill[i+a-1][j+b-1]=c+1;
                show[i+a-1][j+b-1]=1;
     		pt.x=8+(i+a-1)*16;pt.y=8+(j+b-1)*16;
                WinDrawBitmap( hps, bitmaps[c], NULL, &pt, 0, 0, DBM_NORMAL );
                mark[i+a-1][j+b-1]=0;
                if(!c)o++;
		}
          }
          }
      }
   }
   bombs=calcbombs();
   WinReleasePS(hps);
}

// load bitmap resources

void  LoadBitmaps(void)
{
   HPS stubHps;
   int i;
   stubHps = WinGetPS(hwndClient);
   for( i=BM_0; i<=BM_LAST; i++)
   bitmaps[i-BM_0] = GpiLoadBitmap( stubHps, NULLHANDLE,(ULONG) i , 0, 0 );
   WinReleasePS(stubHps);
}

// ÚÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
// ³ Window PROC ³
// ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

MRESULT EXPENTRY MyWindowProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
      INT    x,y;
      HPS    hps;			/* Presentation Space handle	*/
      POINTL pt;			/* String screen coordinates	*/
      CHAR   buf[80];
      HWND   hwndMenu;
  switch( msg )
  {
    case WM_TIMER:
        if(timer&&(!gameover))
        {
	timer_count+=1L;
        if(timer_count>=600L)
        {
           gameover=1;
              WinMessageBox( HWND_DESKTOP,
		   hwndFrame,
		   "Time-delay bomb blow-up!",
		   "GAME OVER",
		   0,
		   MB_INFORMATION | MB_OK | MB_MOVEABLE | MB_SYSTEMMODAL);
              WinSetWindowText(hwndFrame,"Mine Sweeper/2 - GAME OVER");
              failed(-1,-1,hwndClient);
        }
        hps=WinGetPS(hwnd);
        showtimer(hps);
        WinReleasePS(hps);
        }
        break;
    case WM_CREATE:
        seet(40);
        gameover=0;
	WinStartTimer(hab,hwnd,99,1000L);
        break;   
     case WM_BUTTON1DBLCLK:
        if(!gameover)break;
           WinSetWindowText(hwndFrame, "Mine Sweeper/2");
           gameover=0;timer_count=0L;
           if(XS==8)seet(10);
           if(XS==16)seet(40);
           if(XS==32)seet(100);
           content(hwndClient,0);
        break;
     case WM_BUTTON1DOWN:
        if(gameover)break;
	WinQueryPointerPos(HWND_DESKTOP, &pt);          
	WinMapWindowPoints(HWND_DESKTOP,hwndClient,&pt,1);
        if(pt.x<8 || pt.y<8 || pt.x>XS*16+7 || pt.y>YS*16+7) break;
        x=(pt.x-8)/16;y=(pt.y-8)/16;
        if(show[x][y])break;
        if(mark[x][y])break;
        /*pt.x=pt.x-pt.x%16;*/
        /*pt.y=pt.y-pt.y%16;*/
        if(pull[x][y]){
        failed(x,y,hwndClient); 
        gameover=1;WinSetWindowText(hwndFrame,"Mine Sweeper/2 - GAME OVER");
        }else fills(x,y,hwndClient);
        break;
     case WM_BUTTON2DOWN:
        if(gameover)break;
	WinQueryPointerPos(HWND_DESKTOP, &pt);
	WinMapWindowPoints(HWND_DESKTOP,hwndClient,&pt,1);
        if(pt.x<8 || pt.y<8 || pt.x>XS*16+7 || pt.y>YS*16+7) break;
        x=(pt.x-8)/16;y=(pt.y-8)/16;
        if(show[x][y])break;
        mark[x][y]=!mark[x][y];
        pt.x=x*16+8;
        pt.y=y*16+8;
        hps=WinGetPS(hwndClient);
        if(mark[x][y])
           {
           WinDrawBitmap(hps, bitmaps[BM_CHECK-BM_0], NULL, &pt, 0, 0, DBM_NORMAL );
           bombs--;
           sprintf(buf,"%03d",bombs);
           WriteString(hps,XS*16-24,YS*16+22,buf,CLR_RED,CLR_BLACK);
           WinSetWindowText(hwndFrame,"Mine Sweeper/2");
           if(!bombs)
           {
           gameover=1;
           if(!checkdone()){WinSetWindowText(hwndFrame,"Mine Sweeper/2 - SUCCESSFULL!");
              WinMessageBox( HWND_DESKTOP,
		   hwndFrame,
		   "You safe all mines!",
		   "Information",
		   0,
		   MB_INFORMATION | MB_OK | MB_MOVEABLE | MB_SYSTEMMODAL);
		}
           else{
              WinSetWindowText(hwndFrame,"Mine Sweeper/2 - GAME OVER");
              failed(x,y,hwndClient);
              }
           }
           }
        else
           {
        WinDrawBitmap(hps, bitmaps[BM_EMPTY-BM_0], NULL, &pt, 0, 0, DBM_NORMAL );
           bombs++;
           sprintf(buf,"%03d",bombs);
           WriteString(hps,XS*16-24,YS*16+22,buf,CLR_RED,CLR_BLACK);
           WinSetWindowText(hwndFrame,"Mine Sweeper/2");
           }
        WinReleasePS(hps);
        break;

    case WM_ERASEBACKGROUND:
	return (MRESULT)( TRUE );

    case WM_COMMAND:
      	switch(SHORT1FROMMP(mp1))
	{
	case M_NEWEASY:
        WinSetWindowText(hwndFrame, "Mine Sweeper/2");
	gameover=0;XS=8;YS=8;

        WinSetWindowPos( hwndFrame,	/* Shows and activates frame	*/
		   HWND_TOP,		/* window at position 100, 100, */
		   0, 0, XS*16+18, YS*16+47+lMenuHight,	/* and size 200, 200.		*/
		   SWP_ACTIVATE | SWP_SHOW | SWP_SIZE);

        seet(10);
        content(hwndClient,0);
        timer_count=0L;
        break;
       	case M_NEWMEDI:
        WinSetWindowText(hwndFrame, "Mine Sweeper/2");
	gameover=0;XS=16;YS=16;

        WinSetWindowPos( hwndFrame,	/* Shows and activates frame	*/
		   HWND_TOP,		/* window at position 100, 100, */
		   0, 0, XS*16+18, YS*16+47+lMenuHight,	/* and size 200, 200.		*/
		   SWP_ACTIVATE | SWP_SHOW | SWP_SIZE);

        seet(40);
        content(hwndClient,0);
        timer_count=0L;
        break;
      	case M_NEWHARD:
        WinSetWindowText(hwndFrame, "Mine Sweeper/2");
	gameover=0;XS=32;YS=20;
        WinSetWindowPos( hwndFrame,	/* Shows and activates frame	*/
		   HWND_TOP,		/* window at position 100, 100, */
		   0, 0, XS*16+18, YS*16+47+lMenuHight,	/* and size 200, 200.		*/
		   SWP_ACTIVATE | SWP_SHOW | SWP_SIZE);
        seet(100);
        content(hwndClient,0);
        timer_count=0L;
        break;
     case M_TIMER:
        timer=!timer;
        hwndMenu=WinWindowFromID(hwndFrame,FID_MENU);
        if(timer)WinCheckMenuItem(hwndMenu,M_TIMER,TRUE);
        else     WinCheckMenuItem(hwndMenu,M_TIMER,FALSE);
        content(hwndClient,0);
        break;
     case M_ABOUT:
        WinDlgBox(HWND_DESKTOP,hwndFrame,NULL,NULLHANDLE,200,NULL);
        break;
     case M_EXIT:
        WinPostMsg( hwnd, WM_QUIT, (MPARAM)0,(MPARAM)0 );/* Cause termination*/
        break;
	default:  
	break;
    	}
	break;
    case WM_PAINT:
	content(hwndClient,1);
        if(gameover)failed(x,y,hwndClient);
      break;
    case WM_CLOSE:
	WinPostMsg( hwnd, WM_QUIT, (MPARAM)0,(MPARAM)0 );/* Cause termination*/
	break;
    default: 
	return WinDefWindowProc( hwnd, msg, mp1, mp2 );
}
return (MRESULT)FALSE;
}

// ÚÄÄÄÄÄ¿
// ³ END ³
// ÀÄÄÄÄÄÙ

