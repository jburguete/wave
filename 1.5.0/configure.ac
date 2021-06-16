# To work in:
#	- Debian 7.7 (Linux, kFreeBSD or Hurd)
#	- FreeBSD 10.0
#	- Microsoft Windows XP 32 bits
#	- Microsoft Windows 7 32 bits
#	- NetBSD (with modular xorg)
#
# aclocal
# autoconf
# automake --add-missing
# ./configure
# make

# To work in Microsoft Windows (64 bits):
#
# aclocal
# autoconf
# automake --add-missing
# ./configure --host=x86_64-w64-mingw32
# make

# To work in OpenBSD:
# AUTOCONF_VERSION=2.69 aclocal-1.11
# autoconf-2.69
# CPPFLAGS='-I/usr/local/include -I/usr/X11R6/include' LDFLAGS='-L/usr/local/lib -L/usr/X11R6/lib' ./configure
# make

# To work in DragonflyBSD:
# setenv CPPFLAGS '-I/usr/pkg/include -I/usr/pkg/include/gettext'
# setenv CFLAGS '-I/usr/pkg/include -I/usr/pkg/include/gettext'
# setenv LDFLAGS '-L/usr/pkg/lib'
# aclocal
# autoconf
# ./configure
# make

AC_PREREQ([2.67])
AC_INIT(wave, [1.5.0])

# Checks for operative systems
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE
AC_MSG_NOTICE([System: $build])
AC_MSG_NOTICE([System: $host])
if test $host = "x86_64-w64-mingw32"; then
	AC_MSG_NOTICE([Microsoft Windows 64 bits])
	CPPFLAGS="$CPPFLAGS -I/mingw/include"
	AC_SUBST(ccflags, "-march=corei7 -mfpmath=sse -msse3 -mwindows")
	AC_SUBST(libgcc, "libgcc_s_seh-1")
	win=1
	windir=win64
elif test $host = "i686-pc-mingw32"; then
	AC_MSG_NOTICE([Microsoft Windows 32 bits])
	LDFLAGS="$LDFLAGS -lglut -lopengl32"
	CPPFLAGS="$CPPFLAGS -I/mingw/include"
	AC_SUBST(ccflags, "-march=pentium4 -mfpmath=sse -msse2 -mwindows")
	AC_SUBST(libgcc, "libgcc_s_sjlj-1")
	win=1
	windir=win32
elif test `uname -s` = "NetBSD"; then
	AC_MSG_NOTICE([NetBSD])
	LDFLAGS="$LDFLAGS -L/usr/pkg/lib"
	CPPFLAGS="$CPPFLAGS -I/usr/pkg/include"
	PATH="/usr/pkg/gcc48/bin:$PATH"
	win=0
elif test `uname -s` = "OpenBSD"; then
	AC_MSG_NOTICE([NetBSD])
	LDFLAGS="$LDFLAGS -L/usr/X11R6/lib"
	CPPFLAGS="$CPPFLAGS -I/usr/X11R6/include"
	win=0
else
	win=0
fi
if test $win = 0; then
	AC_MSG_NOTICE([Unix type system])
	AC_SUBST(latex, "pdflatex -shell-escape")
	AC_SUBST(objdir, [./])
	AC_SUBST(bindir, [./])
	AC_SUBST(localdir, [po/])
	AC_SUBST(ccflags, [-march=native])
else
	AC_CHECK_TOOL(WINDRES, windres)
	AC_SUBST(latex, "pdflatex -enable-write18")
	AC_SUBST(sysdir, [/mingw/])
	AC_SUBST(objdir, [$windir/obj/])
	AC_SUBST(bindir, [$windir/bin/])
	AC_SUBST(etcdir, [$windir/etc/])
	AC_SUBST(libdir, [$windir/lib/])
	AC_SUBST(sharedir, [$windir/share/])
	AC_SUBST(basedir, [$windir/])
	AC_SUBST(icon, [$windir/obj/logo.o])
fi

# Checks for programs
AC_PROG_CC([egcc gcc clang])
AC_PROG_MAKE_SET
AC_LANG([C])
PKG_PROG_PKG_CONFIG
AC_MSG_NOTICE([Compiler: $CC])
$CC -v

# Checks for compiler flags
compiler=$CC
CC="$CC -flto"
AC_TRY_LINK([], [int main(int argc, char **argv){return 0;}], [lto=1], [lto=0])
CC=$compiler
if test $lto = 0; then
	AC_MSG_NOTICE([checking for -flto... no])
	AC_SUBST(LTO, [])
else
	AC_MSG_NOTICE([checking for -flto... yes])
	AC_SUBST(LTO, [-flto])
fi

# Checks for libraries.
AC_SEARCH_LIBS([pow], [m])
PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([GTHREAD], [gthread-2.0])
AC_SEARCH_LIBS([textdomain], [intl])
PKG_CHECK_MODULES([GTOP], [libgtop-2.0])
PKG_CHECK_MODULES([PNG], [libpng])
PKG_CHECK_MODULES([FREETYPE], [freetype2])
PKG_CHECK_MODULES([GLEW], [glew])
AC_MSG_NOTICE([GTK+ version 3])
PKG_CHECK_MODULES([GTK], [gtk+-3.0])
AC_SEARCH_LIBS([glEnd], [GL opengl32])
AC_ARG_WITH([vulkan],
	AS_HELP_STRING([--with-vulkan],
		[Build graphic window with Vulkan support]))
if test "x$with_vulkan" = "xyes"; then
	PKG_CHECK_MODULES([VULKAN], [vulkan],
		AC_DEFINE([HAVE_VULKAN], [1])
		AC_SUBST(VULKAN, "1"),
		AC_MSG_NOTICE([No vulkan])
		AC_SUBST(VULKAN, "0"))
fi

# Checks for GtkGLArea, FreeGLUT, GLFW or SDL to draw the graphic window
AC_ARG_WITH([gtkglarea],
	AS_HELP_STRING([--with-gtkglarea],
		[Build graphic window with GtkGLArea]))
AC_ARG_WITH([freeglut],
	AS_HELP_STRING([--with-freeglut], [Build graphic window with FreeGLUT]))
AC_ARG_WITH([glfw],
	AS_HELP_STRING([--with-glfw], [Build graphic window with GLFW]))
AC_ARG_WITH([sdl],
	AS_HELP_STRING([--with-sdl], [Build graphic window with SDL]))
if test "x$with_freeglut" = "xyes"; then
	AC_SUBST(GRAPHIC, "-DHAVE_FREEGLUT=1")
	AC_SEARCH_LIBS([glutLeaveMainLoop], [freeglut glut], glut="yes", glut="no")
else
	if test "x$with_glfw" = "xyes"; then
		AC_SUBST(GRAPHIC, "-DHAVE_GLFW=1")
		PKG_CHECK_MODULES([GLFW], [glfw3])
	else
		if test "x$with_sdl" = "xyes"; then
			AC_SUBST(GRAPHIC, "-DHAVE_SDL=1")
			PKG_CHECK_MODULES([SDL], [sdl2])
		else
			AC_SUBST(GRAPHIC, "-DHAVE_GTKGLAREA=1")
		fi
	fi
fi

# Check for font
AC_MSG_NOTICE([Searching font...])
font=`find /usr -type f -iname freemono.otf`
if test "x$font" = "x"; then
	font=`find /usr -type f -iname freemono.ttf`
	if test "x$font" = "x"; then
		AC_MSG_ERROR([FreeMono font not found])
	fi
fi
AC_MSG_NOTICE([Font: $font])
AC_SUBST(FONT, "-DFONT=\\\"$font\\\"")

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h locale.h sys/param.h libintl.h GL/freeglut.h])

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_MKTIME
AC_CHECK_FUNCS([floor pow sqrt setlocale])

# Checks for types
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T

# Output
#AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
