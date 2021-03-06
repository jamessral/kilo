/**
 * Inclues
 */
#include <iostream>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

/**
 * Defines
 */
#define CTRL_KEY(k) ((k) & 0x1f)

/**
 * Data
 */
struct termios orig_termios;

/**
 * Terminal
 */
void die(const char* s) {
	perror(s);
	exit(1);
}

void disableRawMode() {
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
		die("tsceattr");
}

void enableRawMode() {
	if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
	atexit(disableRawMode);

	struct termios raw = orig_termios;
	raw.c_iflag &= ~(BRKINT | ICRNL | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

/**
 * Init
 */
int main(void) {
	enableRawMode();

	while(true) {
		char c = '\0';
		if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) die("read");
		if (iscntrl(c)) {
			std::printf("%d\r\n", c);
		} else {
			std::printf("%d ('%c')\r\n", c, c);
		}

		if (c == 'q') break;
	}

	return 0;
}
