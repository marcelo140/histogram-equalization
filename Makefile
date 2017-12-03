serial: bmp.h bmp.c hist_serial.c
	$(CC) -O3 -std=c99 -fopenmp bmp.c hist_serial.c -o hist_serial

parallel: bmp.h bmp.c hist_omp.c
	$(CC) -O3 -std=c99 -fopenmp bmp.c hist_omp.c -o hist_omp
