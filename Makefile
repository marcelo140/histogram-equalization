serial: bmp.h bmp.c hist_serial.c
	$(CC) -O3 -std=c99 bmp.c hist_serial.c -o hist_serial
