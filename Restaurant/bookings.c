#include "declare.h"

void completeBooking()
{
	FILE *ptr;
	int delete; //booking to be deleted

	ptr = fopen("tables.dat", "rb+");

	if (ptr == NULL)
	{
		puts("File not found, or not populated!");
		return;
	}

	if ((delete = findBooking(ptr)) == -1) return;

	deleteBooking(delete);

	table.inUse = 1; //should already be marked as in use but in case error

	fseek(ptr, (table.number - 1) * sizeof(struct tabledata), SEEK_SET);
	fwrite(&table, sizeof(struct tabledata), 1, ptr);
	fclose(ptr);

	puts("Complete!");
	printf("Press any key to return to main menu...");
	fflush(stdin);
	_getch();
}

void editBooking()
{
	int choice = 1;

	while (1) //infinite loop, broken by entering 0
	{
		if (choice >= 1 && choice <= 3) puts("\n1.  Add a booking\n2.  Edit a Booking\n3.  Remove a Booking\n0.  Return to main menu");
		fflush(stdin);
		choice = onlyInt(1);

		switch (choice){
		case 0: return;
		case 1: addBooking(); break;
		case 2: editBookingDetails(); break;
		case 3: removeBooking(); break;
		default: puts("That is not one of the options!");
		}
	}
}

void addBooking()
{
	FILE *ptr;
	int flag;
	int tempSeats;

	ptr = fopen("tables.dat", "rb+");

	if (ptr == NULL)
	{
		puts("File not found!\n");
		return;
	}

	puts("Enter '0' at anytime to quit.\nEnter the name that the booking is under:");
	fflush(stdin);
	gets(temp.name);
	if (!strcmp(temp.name, "0")) { fclose(ptr); return; }

	printf("Enter the number of seats required: ");
	fflush(stdin);
	tempSeats = onlyInt(4);
	if (tempSeats == 0) { fclose(ptr); return; }

	do
	{
		if (getDate()) { fclose(ptr); return; } //function that error checks the users date input
		if (getTime()) { fclose(ptr); return; } //function that error checks the users date input
		if ((flag = showAvailableBookingTables(ptr, tempSeats)) == 1) { fclose(ptr); return; } //function checks date against database
	} while (flag == 0);

	sortBookings(); //function places the bookings in oreder by date/time
	fseek(ptr, (table.number - 1) * sizeof(struct tabledata), SEEK_SET);
	fwrite(&table, sizeof(struct tabledata), 1, ptr);
	puts("Booking Complete");
	fclose(ptr);
}

void staffAddBooking()
{
	addBooking();

	printf("Press any key to return to main menu..."); //will clear screen after this
	fflush(stdin);
	_getch();
}

void editBookingDetails()
{
	int i; //loop counter
	FILE *ptr;
	int tempSeats;
	char changesChar[10];
	int len;
	int *changesInt;
	int edit; //current booking that is being edited
	int flag;

	ptr = fopen("tables.dat", "rb+");

	if (ptr == NULL)
	{
		puts("File not found, or not populated!");
		return;
	}

	if ((edit = findBooking(ptr)) == -1) return;

	temp = table.booking[edit];
	tempSeats = table.seats;

	do
	{
		puts("Enter an option to change.\nFor multiple changes seperate each entry by a space (e.g. 1 3 4)");
		puts("1. Change Name\n2. Change number of people\n3. Change Date\n4. Change Time\n0. Exit");
		fflush(stdin);
		gets(changesChar);
		len = (strlen(changesChar) + 1) / 2; //find how many variables in the user entered string

		changesInt = malloc(len * sizeof(int));

		*changesInt = atoi(strtok(changesChar, " "));

		for (i = 1; i < len; i++)
		{
			*(changesInt + i) = atoi(strtok(NULL, " "));
		}

		for (i = 0; i < len; i++)
		{
			switch (*(changesInt + i)){
			case 0: return;
			case 1:
				puts("Enter the new name that the booking is under:");
				fflush(stdin);
				gets(temp.name);
				strcpy(table.booking[edit].name, temp.name);
				break;
			case 2:
				printf("Enter the number of seats required: ");
				fflush(stdin);
				tempSeats = onlyInt(4);
				break;
			case 3: if (getDate()) { fclose(ptr); return; }
					else break;
			case 4: if (getTime()) { fclose(ptr); return; }
					else break;
			}
		}
		if (*changesInt == 1 && len == 1) break; //if the user only wants to change the name

		if (checkBooking() || tempSeats != table.seats) //if the current table is not available at the wanted booking time, or the user changes the number of people at the table
		{
			deleteBooking(edit);
			fseek(ptr, (table.number - 1) * sizeof(struct tabledata), SEEK_SET);
			fwrite(&table, sizeof(struct tabledata), 1, ptr);
			if ((flag = showAvailableBookingTables(ptr, tempSeats)) == 1) { fclose(ptr); return; }
		}
		else deleteBooking(edit);
		sortBookings();
	} while (flag == 0);

	fseek(ptr, (table.number - 1) * sizeof(struct tabledata), SEEK_SET);
	fwrite(&table, sizeof(struct tabledata), 1, ptr);

	puts("Booking successfully edited!\n");
	fclose(ptr);
}

void removeBooking()
{
	char choice;
	FILE *ptr;
	int delete; //booking to be deleted

	ptr = fopen("tables.dat", "rb+");

	if (ptr == NULL)
	{
		puts("File not found, or not populated!");
		return;
	}

	if ((delete = findBooking(ptr)) == -1) return;

	printf("Are you sure you wish to delete this booking (y/n)? ");
	fflush(stdin);
	choice = getchar();

	choice = tolower(choice);
	if (choice != 'y') return;

	deleteBooking(delete);

	puts("Booking deleted!\n");

	fseek(ptr, (table.number - 1) * sizeof(struct tabledata), SEEK_SET);
	fwrite(&table, sizeof(struct tabledata), 1, ptr);
	fclose(ptr);
}

void showFutureBookings()
{
	int choice = 1;

	while (1) //infinite loop, broken by entering 0
	{
		if (choice >= 1 && choice <= 3) puts("\n1.  Display Future Bookings on a table\n2.  Display Future Bookings up to a date\n0.  Return to main menu");
		fflush(stdin);
		choice = onlyInt(1);

		switch (choice){
		case 0: return;
		case 1: tableBookings(); break;
		case 2: dateBookings(); break;
		default: puts("That is not one of the options!");
		}
	}

}

void tableBookings()
{
	FILE *ptr;
	int i; //loop counter
	int total; //total no. of bookings

	if ((ptr = fopen("tables.dat", "rb")) == NULL)
	{
		puts("Error opening file!");
		return;
	}

	printf("Enter table number, or '0' to return: ");
	table.number = onlyInt(4);
	if (table.number == 0) return;

	fseek(ptr, (table.number - 1) * sizeof(struct tabledata), SEEK_SET);
	if (fread(&table, sizeof(struct tabledata), 1, ptr) == NULL)
	{
		puts("Table not found!\n");
		return;
	}

	total = countBookings();

	printf("%-3s %-6s %-24s %-9s %s\n", "#", "Seats", "Name", "Date", "Time");
	printf("%-3s %-6s %-24s %-9s %s\n", "-", "-----", "----", "----", "----");

	for (i = 0; i < total; i++)
	{
		printf("%-3d %-6d %-24s %02d/%02d/%d  %d\n", i + 1, table.seats, table.booking[i].name,
			table.booking[i].day, table.booking[i].month, table.booking[i].year, table.booking[i].time);
	}
	puts("");
}

void dateBookings()
{
	FILE *ptr;
	int i;
	int counter = 1;
	int total;
	char tempDate[10];

	if ((ptr = fopen("tables.dat", "rb")) == NULL)
	{
		puts("Error opening file!");
		return;
	}

	if (getDate()) return;

	printf("%-3s %-9s %-6s %-24s %-9s %s\n", "#", "Table No", "Seats", "Name", "Date", "Time");
	printf("%-3s %-9s %-6s %-24s %-9s %s\n", "-", "--------", "-----", "----", "----", "----");

	while (fread(&table, sizeof(struct tabledata), 1, ptr) != NULL)
	{
		total = countBookings();

		for (i = 0; i < total; i++)
		{ //checking bookings against user inputted date
			if (table.booking[i].day > temp.day && table.booking[i].month >= temp.month && table.booking[i].year >= temp.year) break;
			else if (table.booking[i].month > temp.month && table.booking[i].year >= temp.year) break;
			else if (table.booking[i].year > temp.year) break;

			printf("%-3d %-9d %-6d %-24s %02d/%02d/%d  %d\n", counter, table.number, table.seats, table.booking[i].name,
				table.booking[i].day, table.booking[i].month, table.booking[i].year, table.booking[i].time);

			counter++;
		}
	}
	puts("");
}

int showAvailableBookingTables(FILE *ptr, int tempSeats)
{
	int i, count = 0; //loop counter
	int tableNumbers[5]; //check against all available tables

	for (i = 0; i <= 3; i++) //look for free tables with most 3 more seats than necessary
	{
		fseek(ptr, 0, SEEK_SET);
		while (fread(&table, sizeof(struct tabledata), 1, ptr) != NULL)
		{
			if (table.seats == tempSeats + i && checkBooking() == 0)
			{
				printf("Table #%d is free with %d seats\n", table.number, table.seats);
				tableNumbers[count] = table.number;
				count++;
			}
			if (count == 5) break; //don't need to print out more than 5 options for a table
		}
		if (count == 5) break;
	}
	if (count == 0)
	{
		puts("Sorry, no availble tables at this time!");
		return 0;
	}
	while (1)
	{
		printf("\nChoose a table: ");
		fflush(stdin);
		table.number = onlyInt(4);
		if (table.number == 0) return 1;
		for (i = 0; i < count; i++)
		{
			if (table.number == tableNumbers[i]) { i = -1; break; }
		}
		if (i == -1) break;
		puts("That was not one of the available tables!");
	}
	fseek(ptr, (table.number - 1) * sizeof(struct tabledata), SEEK_SET);
	fread(&table, sizeof(struct tabledata), 1, ptr); //re-read selected table

	return 2;
}

int countBookings()
{
	int i; //counter

	for (i = 0; table.booking[i].day != NULL; i++);
	return i;
}

int findBooking(FILE *ptr)
{
	int i, j = 0, k = 0; //loop counters
	int total; //total number of bookings on the selected table
	int countTables[20];
	int countMatches[20][50]; //[table number][booking number]
	int choice = 0;
	char tempName[81];

	do
	{
		puts("Enter the name that the booking was entered under, or '0' to quit.");
		fflush(stdin);
		gets(tempName);

		if (tempName[0] == '0') return -1;

		while (fread(&table, sizeof(struct tabledata), 1, ptr) != NULL)
		{
			total = countBookings();
			for (i = 0; i < total; i++)
			{
				if (strstr(table.booking[i].name, tempName))
				{
					if (k == 0) //first time a booking match is found for a table
					{
						countTables[j] = table.number;
					}
					countMatches[j][k] = i;
					k++;
				}
			}
			if (k > 0) j++;
			k = 0;
		}
		if (j == 1)
		{
			fseek(ptr, (countTables[0] - 1) * sizeof(struct tabledata), SEEK_SET);
			fread(&table, sizeof(struct tabledata), 1, ptr);

			printf("%-9s %-6s %-24s %-9s %s\n", "Table No", "Seats", "Name", "Date", "Time");
			printf("%-9s %-6s %-24s %-9s %s\n", "--------", "-----", "--------", "--------", "----");
			printf("%-9d %-6d %-24s %02d/%02d/%d  %d\n\n", table.number, table.seats, table.booking[countMatches[0][0]].name,
				table.booking[countMatches[0][0]].day, table.booking[countMatches[0][0]].month, table.booking[countMatches[0][0]].year,
				table.booking[countMatches[0][0]].time);

			return countMatches[0][0];
		}

		if (j == 0) puts("Booking not found!\n"); //use best match from previuos lab
	} while (j == 0);

	puts("More than one booking found:");
	printf("%-3s %-9s %-6s %-24s %-9s %s\n", "#", "Table No", "Seats", "Name", "Date", "Time");
	printf("%-3s %-9s %-6s %-24s %-9s %s\n", "-", "--------", "-----", "----", "----", "----");

	for (i = 0; i < j; i++)
	{
		fseek(ptr, (countTables[i] - 1) * sizeof(struct tabledata), SEEK_SET);
		fread(&table, sizeof(struct tabledata), 1, ptr);

		for (k = 0; countMatches[i][k] >= 0; k++)
		{
			choice++;
			printf("%-3d %-9d %-6d %-24s %02d/%02d/%d  %d\n", choice, table.number, table.seats, table.booking[countMatches[i][k]].name,
				table.booking[countMatches[i][k]].day, table.booking[countMatches[i][k]].month, table.booking[countMatches[i][k]].year,
				table.booking[countMatches[i][k]].time);
		}
	}
	while (1)
	{
		printf("\nEnter choice: ");
		fflush(stdin);
		choice = onlyInt(1);

		for (i = 0; i < j; i++)
		{
			fseek(ptr, (countTables[i] - 1) * sizeof(struct tabledata), SEEK_SET);
			fread(&table, sizeof(struct tabledata), 1, ptr);

			for (k = 0; countMatches[i][k] >= 0; k++)
			{
				if (choice == 1) return countMatches[i][k];
				choice--;
			}
		}
		puts("That was not one of the options!");
	}
}

int printBookings()
{
	int i; //loop counter
	int total = countBookings();

	if (total == 0)
	{
		puts("No future bookings on this table currently.\n");
		return 1;
	}

	printf("\n");
	printf("%-25s%-8s\t\t%s\n", "Name", "Date", "Time");
	printf("%-25s%-8s\t\t%s\n", "----", "----", "----");

	for (i = 0; i < total; i++)
	{
		printf("%-25s%02d/%02d/%d\t\t%d\n", table.booking[i].name, table.booking[i].day,
			table.booking[i].month, table.booking[i].year, table.booking[i].time);
	}
	printf("\n");
	return 0;
}

int getDate()
{
	int flag;
	char tempDate[81];

	do
	{
		printf("Enter the date (dd/mm/yy) or '0' to return: ");
		strcpy(tempDate, onlyDate());
		if (!strcmp(tempDate, "0")) return 1;

		temp.day = atoi(strtok(tempDate, "/"));
		temp.month = atoi(strtok(NULL, "/"));
		temp.year = atoi(strtok(NULL, "/"));

		if (temp.day < 1 || temp.day > 31 || temp.month < 1 || temp.month > 12
			|| temp.year < date.year || temp.year > date.year + 1)
		{
			puts("Invalid Date!");
			flag = 1;
		}
		else if (temp.day < date.day && temp.month == date.month && temp.year == date.year)
		{
			puts("Date has already passed!");
			flag = 1;
		}
		else if (temp.month < date.month && temp.year == date.year)
		{
			puts("Date has already passed!");
			flag = 1;
		}
		else flag = 0;
	} while (flag == 1);

	return 0;
}

char* onlyDate()
{
	char input[200];
	int i = 0, j = 0;

	puts("");
	while (1)
	{
		if (i == 2 || i == 5)
		{
			input[i] = '/';
			printf("%c", '/');
			i++;
			continue;
		}
		fflush(stdin);
		input[i] = _getch();
		//if (input[i] == 3) return;
		if ((input[i] == 13 || input[i] == 10) && ((i == 8) || (i == 1 && input[i - 1] == '0'))) { input[i] = 0; break; }
		if ((input[i - 1] >= '0' && input[i - 1] <= '9') && input[i] == '/')
		{
			if (i > 4) continue;
			if (i >= 8) continue;
			input[i] = input[i - 1];
			input[i - 1] = '0';
			input[i + 1] = '/';
			input[i + 3] = 8;
			i += 3;
		}
		if (input[i] == 8 || input[i] == 127)
		{
			if (i == 0) continue;
			i--;
			printf("\r"); //move cursor to begininning of line
			for (j = 0; j <= i; j++)
			{
				printf(" ");
			}
			if (i == 2 || i == 5) i--;
			printf("\r");
			input[i] = 0;
			printf("%s", input);
		}
		if (i == 8) continue;
		if (input[i] < '0' || input[i] > '9') { input[i] = 0; continue; }
		else
		{
			printf("%c", input[i]);
			i++;
		}
	}
	puts("");
	return input;
}

int getTime()
{
	int flag;

	do
	{
		printf("Enter time e.g.(1630): ");
		fflush(stdin);
		temp.time = onlyInt(4);
		if (temp.time == 0) return 1;
		if (temp.time < opening || temp.time > closing - 200 ||
			(temp.day == date.day && temp.month == date.month && temp.year == date.year && temp.time < date.time - 200)) //can't book on same day two hours before dinner
		{
			puts("Invalid Time!");
			flag = 1;
		}
		else flag = 0;
	} while (flag == 1);

	return 0;
}

int checkBooking()
{
	int i; //loop counter
	int total = countBookings(); //total no. of bookings

	for (i = 0; i < total; i++)
	{
		if (table.booking[i].day == temp.day && table.booking[i].month == temp.month
			&& table.booking[i].year == temp.year && table.booking[i].time + 200 > temp.time
			&& table.booking[i].time < temp.time + 200) //booking can't be within 2 hours (200) of another booking
		{
			return 1;
		}
	}

	return 0;
}

void deleteBooking(i)
{
	int total = countBookings();

	for (; i < total; i++)
	{
		table.booking[i] = table.booking[i + 1];
	}
}

void sortBookings()
{
	int total; //total number of booking
	int i; //loop counter

	total = countBookings();

	for (i = 0; i < total; i++)
	{
		if (table.booking[i].day >= temp.day && table.booking[i].month >= temp.month
			&& table.booking[i].year >= temp.year && table.booking[i].time > temp.time) break;

		else if (table.booking[i].day > temp.day && table.booking[i].month >= temp.month
			&& table.booking[i].year >= temp.year) break;

		else if (table.booking[i].month > temp.month && table.booking[i].year >= temp.year) break;

		else if (table.booking[i].year > temp.year) break;
	}

	for (; total >= i; total--)
	{
		table.booking[total + 1] = table.booking[total];
	}

	table.booking[i] = temp;
}