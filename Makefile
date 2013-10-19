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
CFLAGS  = -g -Wall -I include
LIBCFLAGS = $(CFLAGS) -o $(OBJDIR)/$@
SRCDIR = src


all: testapp

testapp: library
	$(CC) $(CFLAGS) -o testapp test_app.c lib.a

# build just the static library
library: stk.o ll.o sb.o ht.o
	$(AR) $(ARFLAGS) lib.a $(OBJDIR)/stk.o $(OBJDIR)/ll.o $(OBJDIR)/sb.o \
	$(OBJDIR)/ht.o $(OBJDIR)/lookup3.o

# build the file system
buildfs:
	mkdir -p $(OBJDIR)

# build stack object
stk.o: buildfs $(SRCDIR)/stk.c
	$(CC) $(LIBCFLAGS) -c $(SRCDIR)/stk.c

# build linked list object
ll.o: buildfs $(SRCDIR)/ll.c
	$(CC) $(LIBCFLAGS) -c $(SRCDIR)/ll.c

# build stringbuffer object
sb.o: buildfs ll.o $(SRCDIR)/sb.c
	$(CC) $(LIBCFLAGS) -c $(SRCDIR)/sb.c

# build hashtable object
ht.o: buildfs lookup3.o $(SRCDIR)/ht.c
	$(CC) $(LIBCFLAGS) -c $(SRCDIR)/ht.c

# build lookup3 object
lookup3.o: buildfs $(SRCDIR)/lookup3.c
	$(CC) $(LIBCFLAGS) -c $(SRCDIR)/lookup3.c

# remove all binaries and annoying Emacs Backups
clean:
	$(RM) lib.a testapp $(SRCDIR)/*~ $(INCDIR)/*~ *~
	$(RM) -rf objs
