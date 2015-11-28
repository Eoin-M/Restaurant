/* reads from keypress, doesn't echo */
#ifdef unix
	#include <termios.h>
	#include <unistd.h>
	#include <unistd.h>
	int _getch(void) //custom _getch() for UNIX Compilers
	{
		struct termios oldattr, newattr;
		int ch;
		tcgetattr(STDIN_FILENO, &oldattr);
		newattr = oldattr;
		newattr.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
		ch = getchar();
		tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
		return ch;
	}
#endif

void CLRSCR()
{
	#ifdef _WIN32
		system("cls");
	#endif

	#ifdef unix
		system("clear");
	#endif
}