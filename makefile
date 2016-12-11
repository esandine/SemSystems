strtest: semctrl
	gcc semctrl.c -o semctrl
	gcc filctl.c -o filctrl
run:
	./filctrl
