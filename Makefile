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
library: stack.o ll.o sb.o hashtable.o
	$(AR) $(ARFLAGS) lib.a $(OBJDIR)/stack.o $(OBJDIR)/ll.o $(OBJDIR)/sb.o \
	$(OBJDIR)/hashtable.o $(OBJDIR)/sha256.o

# build the debug library
buildfs:
	mkdir -p $(OBJDIR)

# build stack object
stack.o: buildfs $(SRCDIR)/stack.c
	$(CC) $(LIBCFLAGS) -c $(SRCDIR)/stack.c

# build linked list object
ll.o: buildfs $(SRCDIR)/ll.c
	$(CC) $(LIBCFLAGS) -c $(SRCDIR)/ll.c

# build stringbuffer object
sb.o: buildfs ll.o $(SRCDIR)/sb.c
	$(CC) $(LIBCFLAGS) -c $(SRCDIR)/sb.c

# build hashtable object
hashtable.o: buildfs ll.o sha256.o $(SRCDIR)/hashtable.c
	$(CC) $(LIBCFLAGS) -c $(SRCDIR)/hashtable.c

# build sha256 object
sha256.o: buildfs $(SRCDIR)/sha256.c
	$(CC) $(LIBCFLAGS) -c $(SRCDIR)/sha256.c

# remove all binaries and annoying Emacs Backups
clean:
	$(RM) lib.a testapp $(SRCDIR)/*~ *~
	$(RM) -rf objs