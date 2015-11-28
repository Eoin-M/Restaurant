#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"
#include "string.h"
#include "time.h"

#ifdef _WIN32
	#include "conio.h"
	#include "windows.h"
#endif

#ifdef unix
	int _getch();
	#define Sleep(x) usleep(x * 1000)
#endif

enum openingHours { opening = 1600, closing = 2300 }; //opening and closing hours

void administrator();
void staff();
void updateBookings();
int onlyInt(int max);
float onlyPosFloat();

//password.c
int checkPassword();
void changePassword();

//tables.c
void selectTable(int choice);
void showAvailableTables();
void showAllTables();
void editTables();
void addTable();
void removeTable();
void editTableNumber();
int searchTables(FILE *ptr, int number);

struct date {
	int day;
	int month;
	int year;
	int time; //time input as 4 digit number, e.g. 1430 -> half past 2
	char name[81];
};

struct date date;

struct tabledata {
	int number; //table number. If table number is 0, it is deleted
	int seats; //number of seats at a table
	int inUse; //1 for in use, 0 for free
	char order[200][81];
	struct date booking[200]; //date, time and name place holder for a booking. NULL in all variables for not booked
};

struct tabledata table;

//bookings.c
struct date temp;

void completeBooking();
void editBooking();
void addBooking();
void staffAddBooking();
void editBookingDetails();
void removeBooking();
void showFutureBookings();
void tableBookings();
void dateBookings();
int showAvailableBookingTables(FILE *ptr, int tempSeats);
int countBookings();
int findBooking(FILE *ptr); //returns the position of the booking in the array, or -1 to quit
int printBookings();
int getDate();
char* onlyDate();
int getTime();
int checkBooking(); //returns 1 if booking already exists
void deleteBooking(i);
void sortBookings(); //current booking data
int showAvailableNearDates(FILE *ptr, int tempSeats);

//waiters.c
void editWaitors();

//menu.c
void printMenu();//function to print menu
int readMenu(FILE *ptr);//function to read menu 
void writeMenu(FILE *ptr, int count);//function to write menu
void printDish(int id, int current);//function to print the dish from the .dat file
void editMenu();//function to edit menu
void addMenu();//function to add a dish to a menu
void updateMenu();//function to update the price of the dish
void removeMenu();//function to remove a dish off/from menu

struct menudata {//structure used throughout the menu.c
	int ID;
	char name[81];
	float price;
	char type[20]; // S for starter, M for main, D for dessert, s for Side dishes, d for drinks
	char spec[20]; //VE for vegetarian,N for nuts,GF for gluten free, VG for vegan, DF for dairy free
};
struct menudata *menu; //the way structure is mentioned in menu.c

//order.c
void order(int items);
void receipt();
void screen();