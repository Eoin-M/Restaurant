// Restaurant.cpp : Manages operations for a restaurant
// Program can only be run on Windows due to use of Windows specific functions, system("cls"), _getch() {curses in unix may be able to use this function}

#include "declare.h"

void main()
{
	//printf("Menu Structure Size: %d\n", sizeof(struct menudata));
	//printf("Table Structure Size: %d\n\n\n", sizeof(struct tabledata));
	int choice;

	updateBookings();

	puts("Restaurant Management Program\n"); //temporary title
	printf("1. Administrator access\n2. Staff Access\n0. Exit");
	while (1)
	{
		choice = onlyInt(1);
		switch (choice){
		case 0: return;
		case 1: administrator(); return;
		case 2: staff(); return;
		default: printf("That is not one of the options!\nPlease enter again: ");
		}
	}
}

void administrator()
{

	int choice = 0;
	int password; //check to see if password entered correctly, 0 for incorrect, 1 for correct

	//Password: Password1!
	//key: 1234
	puts("Please enter the administrator password. 3 attempts remaining:");
	password = checkPassword();

	if (password == 0) { puts("Password and Key combination incorrectly entered 3 times.\nProgram is exiting..."); return; }

	while (1)
	{
		if (choice >= 0 && choice <= 11)
		{
			CLRSCR(); //clears window up until this point
			puts("-------------------------------------------------------");
			puts("1.  Make/Edit/Delete a booking");
			puts("2.  Complete a booking");
			puts("3.  Search for available tables by no. of seats");
			puts("4.  Display menu and take order");
			puts("5.  Display receipt");
			puts("6.  Show information on tables");
			puts("7.  Show future bookings");
			puts("8.  Edit Tables");
			puts("9.  Edit Menu");
			puts("10. Change Administrator password");
			puts("0.  Exit");
			printf("-------------------------------------------------------");
		}

		choice = onlyInt(2);
		switch (choice){
		case 0: return;
		case 1: editBooking(); break; //make a booking
		case 2: completeBooking(); break; //complete booking
		case 3: showAvailableTables(); break;
		case 4: selectTable(1); break; //display menu function
		case 5: selectTable(2); break; //display receipt function
		case 6: showAllTables(); break;
		case 7: showFutureBookings(); break;
		case 8: editTables(); break;
		case 9: editMenu(); break; //edit menu.dat file
		case 10: changePassword(); return; //program logs user out after changing password
		default: printf("That is not one of the options!\nPlease enter again: ");
		}
	}
}

void staff()
{
	int choice = 0;

	while (1)
	{
		if (choice >= 0 && choice <= 7)
		{
			CLRSCR(); //clears window up until this point
			puts("-------------------------------------------------------");
			puts("1.  Make a booking");
			puts("2.  Complete a booking");
			puts("3.  Search for available tables by no. of seats");
			puts("4.  Display menu and take order");
			puts("5.  Display receipt");
			puts("6.  Show information on tables");
			puts("7.  Show future bookings");
			puts("0.  Exit");
			printf("-------------------------------------------------------");
		}

		choice = onlyInt(1);
		switch (choice){
		case 0: return;
		case 1: staffAddBooking(); break; //make a booking
		case 2: completeBooking(); break; //complete booking
		case 3: showAvailableTables(); break;
		case 4: selectTable(1); break; //display menu function
		case 5: selectTable(2); break; //display receipt function
		case 6: showAllTables(); break;
		case 7: showFutureBookings(); break;
		default: printf("That is not one of the options!\nPlease enter again: ");
		}
	}
}

void updateBookings()
{
	FILE *ptr;
	time_t currentTime = time(NULL);
	struct tm *timeInfo;

	timeInfo = localtime(&currentTime);

	date.day = timeInfo->tm_mday;
	date.month = timeInfo->tm_mon;
	date.year = (timeInfo->tm_year) - 100;
	date.time = (timeInfo->tm_hour) * 100 + (timeInfo->tm_min); //date saved as 4 digit number
	date.name[0] = 0;

	//check if table is now reserved, 2 hours before reservation

	ptr = fopen("tables.dat", "rb+");

	if (ptr == NULL)
	{
		return;
	}

	temp = date;

	while ((fread(&table, sizeof(struct tabledata), 1, ptr)) != NULL)
	{
		if (table.booking[0].day == temp.day && table.booking[0].month == temp.month
			&& table.booking[0].year == temp.year && table.booking[0].time + 200 > temp.time
			&& table.booking[0].time < temp.time + 200)
		{
			table.inUse = 1;
			fseek(ptr, 0 - sizeof(struct tabledata), SEEK_SET);
			fwrite(&table, sizeof(struct tabledata), 1, ptr);
		}
		else if (table.booking[0].day == temp.day && table.booking[0].month == temp.month
			&& table.booking[0].year == temp.year && table.booking[0].time < temp.time + 200)
		{
			deleteBooking(0);
			table.inUse = 0;
			fseek(ptr, 0 - sizeof(struct tabledata), SEEK_SET);
			fwrite(&table, sizeof(struct tabledata), 1, ptr);
		}
	}
	fclose(ptr);
}

int onlyInt(int max)
{
	char input[200];
	int i = 0, j = 0;

	puts("");
	while (1)
	{
		fflush(stdin);
		input[i] = _getch();
		if (input[i] == 3) break;
		if ((input[i] == 13 || input[i] == 10) && i > 0) { input[i] = 0; break; }
		if (input[i] == 8 || input[i] == 127)
		{
			if (i > 0) i--;
			printf("\r"); //move cursor to begininning of line
			for (j = 0; j <= i; j++)
			{
				printf(" ");
			}
			printf("\r");
			input[i] = 0;
			printf("%s", input);
		}
		if (i == max) continue;
		else if (input[i] < '0' || input[i] > '9') { input[i] = 0; continue; }
		else
		{
			printf("%c", input[i]);
			i++;
		}
	}
	puts("");
	return atoi(input);
}

float onlyPosFloat()
{
	char input[200];
	int i = 0; int j = 0;
	int flag = 0;

	puts("");
	while (1)
	{
		fflush(stdin);
		input[i] = _getch();
		if (input[i] == 3) return;
		if ((input[i] == 13 || input[i] == 10) && i > 0) { input[i] = 0; break; }
		if (input[i] == 8 || input[i] == 127)
		{
			if (i > 0) i--;
			if (input[i] == '.') flag = 0;
			printf("\r"); //move cursor to begininning of line
			for (j = 0; j <= i; j++)
			{
				printf(" ");
			}
			printf("\r");
			input[i] = 0;
			printf("%s", input);
			continue;
		}
		if (input[i] == '.' && flag == 0) flag = 1;
		else if (input[i] < '0' || input[i] > '9') { input[i] = 0; continue; }
		printf("%c", input[i]);
		i++;
	}
	puts("");
	return atof(input);
}