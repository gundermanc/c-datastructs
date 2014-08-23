#
# C-Datastructs Library Makefile
# (C) 2013 Christian Gunderman
# ..because I ACTUALLY built a Makefile this time
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this program.  If not, see
# <http://www.gnu.org/licenses/>.
#
# Contact Email: gundermanc@gmail.com
#
CC = gcc
AR = ar
ARFLAGS = rcs
INCDIR = include
OBJDIR = objs

CFLAGS  = -Wall -I include
LIBCFLAGS = $(CFLAGS) -o $(OBJDIR)/$@
SRCDIR = src


# builds library with debug symbols + test application
all: c89-debugapp

############## builds #############
c89-debugapp: CFLAGS += -g
c89-debugapp: CFLAGS += -std=c89
c89-debugapp: debugapp

c89-debuglib: CFLAGS += -g
c89-debuglib: c89-releaselib

c89-releaselib: CFLAGS += -std=c89
c89-releaselib: library

cpp-debugapp: CFLAGS += -g -std=gnu++11
cpp-debugapp: CC = g++
cpp-debugapp: debugapp

cpp-debuglib: CFLAGS += -g -std=gnu++11
cpp-debuglib: cpp-releaselib

cpp-releaselib: CC = g++ -std=gnu++11
cpp-releaselib: cpp-library
##############        #############

# builds the testing application
debugapp: c89-debugapp

c89-debugapp: library
	$(CC) $(CFLAGS) -o testapp test_app.c lib.a

# builds the testing application
cpp-debugapp: cpp-library
	$(CC) $(CFLAGS) -o testapp test_app.c lib.a

# build just the static library
library: stk.o ll.o sb.o ht.o set.o lookup3.o
	$(AR) $(ARFLAGS) lib.a $(OBJDIR)/stk.o $(OBJDIR)/ll.o $(OBJDIR)/sb.o \
	$(OBJDIR)/ht.o $(OBJDIR)/lookup3.o $(OBJDIR)/set.o

# build just the static cpp library
cpp-library: stk.o ll.o sb.o ht.o set.o lookup3.o stack.o
	$(AR) $(ARFLAGS) lib.a $(OBJDIR)/stk.o $(OBJDIR)/ll.o $(OBJDIR)/sb.o \
	$(OBJDIR)/ht.o $(OBJDIR)/lookup3.o $(OBJDIR)/set.o $(OBJDIR)/stack.o

# build the file system
buildfs:
	mkdir -p $(OBJDIR)

# build stack object
stk.o: buildfs $(SRCDIR)/stk.c
	$(CC) $(LIBCFLAGS) -c $(SRCDIR)/stk.c

# build cpp stack object
stack.o: buildfs $(SRCDIR)/Stack.cc
	$(CC) $(LIBCFLAGS) -c $(SRCDIR)/Stack.cc

# build linked list object
ll.o: buildfs $(SRCDIR)/ll.c
	$(CC) $(LIBCFLAGS) -c $(SRCDIR)/ll.c

# build stringbuffer object
sb.o: buildfs ll.o $(SRCDIR)/sb.c
	$(CC) $(LIBCFLAGS) -c $(SRCDIR)/sb.c

# build hashtable object
ht.o: buildfs lookup3.o $(SRCDIR)/ht.c
	$(CC) $(LIBCFLAGS) -c $(SRCDIR)/ht.c

# build hashset object
set.o: ht.o $(SRCDIR)/set.c
	$(CC) $(LIBCFLAGS) -c $(SRCDIR)/set.c

# build lookup3 object
lookup3.o: buildfs $(SRCDIR)/lookup3.c
	$(CC) $(LIBCFLAGS) -c $(SRCDIR)/lookup3.c

# remove all binaries and annoying Emacs Backups
clean:
	$(RM) lib.a testapp $(SRCDIR)/*~ $(INCDIR)/*~ *~
	$(RM) -rf objs
