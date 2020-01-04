
calendar.exe: calendar.c
	gcc -fnative-struct `pkg-config --cflags gtk+-1.3-win32-production` calendar.c -o calendar.exe `pkg-config --libs gtk+-1.3-win32-production`

clean: 
	rm -f calendar.exe
