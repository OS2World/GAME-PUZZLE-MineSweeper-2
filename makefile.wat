
# OpenWatcom 2.0 makefile for Mine Sweeper/2
# Build: wmake -f makefile.wat

WATCOM  = $(%WATCOM)

CC      = wcc386
WLINK   = wlink
WRC     = wrc

OUT     = bin

CFLAGS  = -bt=os2 -mf -5 -fpi -Oaxt -W3 -ze -d0 -i=$(%OS2TK)\h -i=src
LFLAGS  = option quiet, map=bin\mine.map
RCFLAGS = -r -bt=os2
BINDWF  = -q -bt=os2

EXE = $(OUT)\mine.exe
RES = $(OUT)\mine.res

all : $(EXE) .symbolic

$(OUT) :
	@if not exist $(OUT) mkdir $(OUT)

$(EXE) : $(OUT)\mine.obj $(RES) $(OUT)
	$(WLINK) $(LFLAGS) system os2v2 pm option stack=65536,heapsize=4096 name $@ file $(OUT)\mine.obj
	$(WRC) $(BINDWF) -fe=$@ $(RES) $@

$(RES) : src\mine.rc src\mine.h $(OUT)
	$(WRC) $(RCFLAGS) -fo=$@ -i=src src\mine.rc

$(OUT)\mine.obj : src\mine.c src\mine.h src\lang.h $(OUT)
	$(CC) $(CFLAGS) -fo=$@ src\mine.c

clean : .symbolic
	@if exist $(OUT)\mine.obj del $(OUT)\mine.obj >nul
	@if exist $(RES) del $(RES) >nul
	@if exist $(OUT)\mine.map del $(OUT)\mine.map >nul
	@if exist $(EXE) del $(EXE) >nul
