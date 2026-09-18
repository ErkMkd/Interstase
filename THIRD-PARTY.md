# Third-party components

The MIT license in [LICENSE](LICENSE) covers only the code written for this
demo. The following third-party components are vendored in this repository or
required to build and run it, and remain under their own licenses.

## Vendored in this repository

### GLEW — OpenGL Extension Wrangler Library

- Files: `glew.c`, `glew.h`
- Copyright (C) 2008-2015 Nigel Stewart, (C) 2002-2008 Milan Ikits,
  (C) 2002-2008 Marcelo E. Magallon, (C) 2002 Lev Povalahev
- License: Modified BSD (3-clause), with the Mesa 3-D and Khronos/SGI Free
  Software License B — the full texts are in the header of `glew.h`.
- https://glew.sourceforge.net/

### GLFW

- File: `glfw3.dll` (redistributable binary)
- Copyright (C) 2002-2006 Marcus Geelnard, (C) 2006-2015 Camilla Löwy
- License: zlib/libpng
- https://www.glfw.org/

### libpng

- File: `libpng12.dll` (redistributable binary)
- Copyright (C) 1995-2015 The PNG Reference Library Authors et al.
- License: libpng License (zlib-style)
- http://www.libpng.org/pub/png/libpng.html

### BASS audio library — **not open source**

- File: `bass.dll` (redistributable binary)
- Copyright (C) 1999-2015 Un4seen Developments Ltd.
- License: **proprietary**. BASS is free for non-commercial use only; a
  commercial licence must be bought from Un4seen Developments. It may be
  redistributed with the demo, but it is *not* covered by the MIT license and
  may not be relicensed.
- https://www.un4seen.com/

## Required at build time (not redistributed here)

These are linked by `Interstase.cbp` but their sources and import libraries are
not included in this repository:

| Library | License |
| --- | --- |
| libjpeg (`jpeg.lib`, `jpeglib.h`) | IJG License (BSD-like) |
| libtiff (`libtiff.lib`, `tiffio.h`) | libtiff License (BSD-like) |
| zlib (`libz.a`) | zlib License |
| BASS headers (`bass/bass.h`) | proprietary, see above |
| OpenGL / GLU / GDI / WinMM (`opengl32`, `glu32`, `gdi32`, `winmm`) | system libraries (Microsoft Windows) |

## Practical consequence

Because `bass.dll` is proprietary and free only for non-commercial use, a
**commercial** redistribution of the demo as a whole requires either a BASS
commercial licence or replacing the audio playback layer
(`musique.cpp` / `Musique.h`) with an alternative such as libsndfile + OpenAL,
miniaudio or SDL_mixer. The MIT-licensed source code itself carries no such
restriction.
