strtest: sem.ctrl
	gcc semctrl.c -o semcrtl
	gcc semclient.c -o semclient
run:
	./a.out
