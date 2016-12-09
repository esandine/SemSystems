strtest: semctrl
	gcc semctrl.c -o semctrl
	gcc semclient.c -o semclient
run:
	./a.out
