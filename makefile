./a.out : main.o token.o type.o creat_xls.o
	gcc main.o token.o type.o creat_xls.o
main.o : main.c
	gcc -c main.c
token.o : token.c
	gcc -c token.c
type.o : type.c
	gcc -c type.c
creat_xls.o : creat_xls.c
	gcc -c creat_xls.c