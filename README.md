# Mine Sweeper/2 for ArcaOS / eComStation / OS/2

Mine Sweeper/2 is a Minesweeper clone for the OS/2 Presentation Manager.
Originally written by Dmitry Zaharov in 1999.

## Version

1.5

## License

BSD 3-Clause  see `doc/LICENSE.txt`

## Features

- Three difficulty levels: Novice (8x8), Normal (16x16), Profy (32x20)
- Countdown timer with 10-minute game limit
- Mine counter
- 6-language UI: English, Spanish, Dutch, German, French, Italian
- All graphics and game window scaled 2x for better readability
- Timer pauses automatically on focus loss (when Background Run is off)
- Frame Controls toggle (Ctrl+F) for borderless play; window auto-resizes
- Background Run toggle (Ctrl+B)
- Settings saved to MINE.cfg

## Compile Tools

- OpenWatcom 2.0 (`wmake`, `wcc386`, `wlink`, `wrc`)
- OS/2 Toolkit 4.5

## Build

```
compile-wat.cmd
```

Output: `bin\mine.exe`

## Requirements

- OS/2 Warp 4, eComStation, or ArcaOS
- 32-bit Presentation Manager

## Authors

- Original: Dmitry Zaharov  1999
- OS/2 port: OS2World community  2026

## Links

- OS2World: https://www.os2world.com
