# This is the Makefile to use for Homework Assignment #2
# To use, at the prompt, type:
#
# 	make calc		# This will make executable calc
# or
#       make directories        # makes correct directories
#       make install            # copies files to correct place
#       make clean              # cleans up directory
#       make public            # installs assignment into public area

DIR=$(HOME)/../public/hw4

all:	calc

purify:	calc.o main.o mylib.o stack.o
	purify gcc -o calc -g calc.o main.o mylib.o stack.o

valgrind: calc
	valgrind --leak-check=full --read-var-info=yes \
	    --show-reachable=yes ./calc $(DEBUG)

calc:	calc.o main.o mylib.o stack.o
	gcc -o calc -g calc.o main.o mylib.o stack.o

calc.o:	calc.c calc.h mylib.h stack.h
	gcc -Wall -pedantic -c -g calc.c

mylib.o:	mylib.c mylib.h
	gcc -Wall -pedantic -c -g mylib.c

main.o:	main.c calc.h mylib.h stack.h
	gcc -Wall -pedantic -c -g main.c

stack.o:	stack.c stack.h
	gcc -Wall -pedantic -c -g stack.c

clean:
	rm -f core *.o calc 

new:
	make clean
	make

backup:	*.[ch] Makefile
	cp *.[ch] Makefile $(HOME)/hw4/backup

directories:
	mkdir $(HOME)/hw4
	mkdir $(HOME)/hw4/backup

install:
	cp $(DIR)/Makefile $(DIR)/*.h $(DIR)/*.empty \
	$(DIR)/main.c $(DIR)/mylib.c $(DIR)/.cproject $(DIR)/.project \
	$(DIR)/.hw4.vcproj $(DIR)/.hw4.sln $(HOME)/hw4

public:	
	make new

	if [ ! -d $(DIR) ]; then mkdir $(DIR); fi

	# lock public directory
	chmod 700 $(DIR)
	#
	# copy all files to directory
	cp .cproject .project .hw4.vcproj .hw4.sln  Makefile \
	    main.c mylib.c *.h *.empty calc $(DIR)
	#
	# strip executable of debugging information
	strip $(DIR)/calc
	#
	# set correct permissions on postscript output of assignments
	chmod 644 .cproject .project .hw4.vcproj .hw4.sln
	#
	# set correct permissions on source files (access to students now)
	chmod 644 $(DIR)/Makefile $(DIR)/*.[ch] $(DIR)/*.empty
	#
	# set restrictive permissions on solution source files (before due date)
	chmod 640 $(DIR)/mylib.c
	#
	# set correct permissions on solution source files (after hw1 due date)
	chmod 644 $(DIR)/mylib.c
	#
	# set correct permissions on executable
	chmod 711 $(DIR)/calc
	
	# unlock public directory for tutors
	chmod 750 $(DIR)
	
	# unlock public directory for all students
	chmod 755 $(DIR)
	#
	# rm -f $(HOME)/../cs12xzz/hw4/*
	# scp -p Makefile *.[ch] .cproject .project .hw4.vcproj .hw4.sln \
	#   $(DIR)/calc  cs12xzz@ieng6-202:~/hw4

	# move html files to public_html directory
	rm -r $(HOME)/public_html/hw4/
	mkdir $(HOME)/public_html/hw4
	cp *html $(HOME)/public_html/hw4
	chmod 744 $(HOME)/public_html/hw4/*html
	#
	# unlock public directory
	chmod go+rx $(DIR)
	chmod go+rx $(HOME)/public_html/hw4/


vimrc:
	cp $(DIR)/../vimrc $(HOME)/.vimrc

