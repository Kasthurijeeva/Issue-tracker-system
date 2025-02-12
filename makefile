issue_tracker.exe : main.o login.o issue.o
	gcc -o issue_tracker.exe main.o login.o issue.o
main.o:main.c
	gcc -c main.c
login.o:login.c
issue.o:issue.c
clean:
	rm *exe *.o