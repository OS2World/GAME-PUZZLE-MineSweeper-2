Mine Sweeper/2 for ArcaOS / eComStation / OS/2
===============================================
Version 1.5

DESCRIPTION
-----------
Mine Sweeper/2 is a Minesweeper clone for OS/2 Presentation Manager.
Originally written by Dmitry Zaharov in 1999 for IBM VisualAge C++.
Ported to OpenWatcom 2.0 by the OS2World community in 2026.

Features:
  - Three difficulty levels: Novice (8x8), Normal (16x16), Profy (32x20)
  - Countdown timer with 10-minute game limit
  - Mine counter
  - 6-language UI: English, Spanish, Dutch, German, French, Italian
  - All graphics and game window scaled 2x for better readability
  - Timer pauses automatically on focus loss (when Background Run is off)
  - Frame Controls toggle (Ctrl+F) for borderless play; window auto-resizes
  - Background Run option (Ctrl+B)
  - Settings saved to MINE.cfg on exit

LICENSE
-------
  BSD 3-Clause License
  See doc\LICENSE.txt for the full license text.

REQUIREMENTS
------------
  - OS/2 Warp 4, eComStation, or ArcaOS
  - 32-bit Presentation Manager

INSTALLATION
------------
  Copy mine.exe to any folder and run it.

HOW TO PLAY
-----------
  Left-click  : Reveal a cell
  Right-click : Toggle mine flag on a cell
  Double-click: Start a new game (after game over)

  Find all mines without clicking on one.
  When all mine flags match the actual mines, you win.
  If 10 minutes pass without winning, the bomb explodes.

KEYBOARD SHORTCUTS
------------------
  Ctrl+N    New game
  Ctrl+P    Pause / resume game
  Ctrl+Q    Quit current game (stay open)
  Ctrl+X    Exit application
  Ctrl+B    Toggle background run
  Ctrl+F    Toggle frame controls (borderless mode)

COMPILING FROM SOURCE
---------------------
  Requirements:
    - OpenWatcom 2.0  (wmake, wcc386, wlink, wrc)
    - OS/2 Toolkit 4.5

  Build:
    compile-wat.cmd

  Output:
    bin\mine.exe

SETTINGS
--------
  Settings are stored in MINE.cfg in the current directory.
  Delete MINE.cfg to reset all settings to defaults.

  Keys saved: difficulty level, language, timer display,
              background-run flag, save-on-exit flag.

CREDITS
-------
  Original author:  Dmitry Zaharov (1999)
  OS/2 port:        OS2World community (2026)
  OS2World site:    https://www.os2world.com

LINKS
-----
- https://github.com/OS2World/GAME-PUZZLE-MineSweeper-2