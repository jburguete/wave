WAVE
====

A software to solve wave propagations on hyperbolic equations with different
numerical schemes.

AUTHORS
-------

* Javier Burguete Tolosa (jburguete@eead.csic.es)

REQUIRED
--------

* gcc or clang (to compile the source code)
* autoconf (to generate the Makefile in different systems)
* automake (to detect the system)
* pkg-config (to find the libraries to compile)
* gettext (to work with different international locales and languages)
* glib (extended utilities of C to work with data, lists, random numbers, ...)
* png (to work with PNG files)
* gtk+ (to make the interface windows)
* freeglut (interaction with OpenGL to draw graphics)
* [jb](https://github.com/jburguete/jb.git) (utility library of J. Burguete)

FILES
-----

* configure.ac: configure generator.
* Makefile.in: Makefile generator.
* TODO: List of tasks TO DO (in spanish)
* *.h: Header files.
* *.c: Source files.
* locale/es/LC_MESSAGES/*.po: spanish language files.
* locale/fr/LC_MESSAGES/*.po: french language files.

BUILDING INSTRUCTIONS
---------------------

Debian Linux 7.6
________________
Debian kFreeBSD 7.6
___________________
Debian Hurd 7.6
_______________
FreeBSD 10.0
___________
NetBSD 6.1.5 (with modular xorg)
____________

* download the latest [JB library](https://github.com/jburguete/jb)
* cd version (i.e. cd 1.3.2)
* link the latest JB library to jb (i.e. ln -s PATH_TO_THE_JB_LIBRARY/1.6.0 jb)
* aclocal
* autoconf
* automake --add-missing
* ./configure
* make
* strip wave (optional: to make a final version)

Microsoft Windows 7 32 bits
___________________________
Microsoft Windows XP 32 bits
____________________________

* install [MSYS/MinGW](http://www.mingw.org) and the required libraries and
utilities. You can follow detailed instructions in
[MinGW-64-Make](https://github.com/jburguete/MinGW-64-Make)
* download the latest [JB library](https://github.com/jburguete/jb)
* cd version (i.e. cd 1.3.2)
* link the latest JB library to jb (i.e. ln -s PATH_TO_THE_JB_LIBRARY/1.6.0 jb)
* aclocal
* autoconf
* automake --add-missing
* configure
* make
* strip wave (optional: to make a final version)

Microsoft Windows 7 64 bits
___________________________
Microsoft Windows XP 64 bits
____________________________

* install [MSYS/MinGW](http://www.mingw.org) and the required libraries and
utilities. You can follow detailed instructions in
[MinGW-64-Make](https://github.com/jburguete/MinGW-64-Make)
* download the latest [JB library](https://github.com/jburguete/jb)
* cd version (i.e. cd 1.3.2)
* link the latest JB library to jb (i.e. ln -s PATH_TO_THE_JB_LIBRARY/1.6.0 jb)
* aclocal
* autoconf
* automake --add-missing
* configure --host=x86_64-w64-mingw32
* make
* strip wave (optional: to make a final version)

OpenBSD 5.5
___________

* download the latest [JB library](https://github.com/jburguete/jb)
* cd version (i.e. cd 1.3.2)
* link the latest JB library to jb (i.e. ln -s PATH_TO_THE_JB_LIBRARY/1.6.0 jb)
* export AUTOMAKE_VERSION=1.13 AUTOCONF_VERSION=2.69
* aclocal
* autoconf
* automake --add-missing
* ./configure
* make
* strip wave (optional: to make a final version)
