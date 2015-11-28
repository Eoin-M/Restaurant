#include "declare.h"

void selectTable(int option)
{
	int i; //loop counter
	FILE *ptr;

	ptr = fopen("tables.dat", "rb+");
	if (ptr == NULL)
	{
		puts("Database of tables not found. Please add a table first");
		printf("Press any key to return to main menu...");
		fflush(stdin);
		_getch();
		return;
	}

	printf("Enter the table number:");
	table.number = onlyInt(4);

	fseek(ptr, (table.number - 1) * sizeof(struct tabledata), SEEK_SET);
	if (fread(&table, sizeof(struct tabledata), 1, ptr) == NULL)
	{
		puts("No Such Table Found!");
		printf("Press any key to return to main menu...");
		fflush(stdin);
		_getch();
		return;
	}
	else fseek(ptr, (table.number - 1) * sizeof(struct tabledata), SEEK_SET); //move back to table for writing order
	if (option == 1) printMenu();
	else if (option == 2) receipt();
	fwrite(&table, sizeof(struct tabledata), 1, ptr);
	fclose(ptr);
}

void showAvailableTables()
{
	int i, count = 0; //loop counter
	int tableNumbers[5]; //check against all available tables
	int tempSeats;
	FILE *ptr;

	ptr = fopen("tables.dat", "rb+");
	if (ptr == NULL)
	{
		puts("Database of tables not found. Please add a table first");
		printf("Press any key to return to main menu...");
		fflush(stdin);
		_getch();
		return;
	}

	printf("Enter the number of seats required:");
	tempSeats = onlyInt(3);

	for (i = 0; i <= 3; i++) //look for free tables with most 3 more seats than necessary
	{
		fseek(ptr, 0, SEEK_SET);
		while (fread(&table, sizeof(struct tabledata), 1, ptr) != NULL)
		{
			if (table.seats == tempSeats + i && table.inUse == 0)
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
		puts("Sorry, no availble tables!");
		printf("Press any key to return to main menu...");
		fflush(stdin);
		_getch();
		fclose(ptr);
		return;
	}
	while (1)
	{
		printf("\nChoose a table: ");
		scanf("%d", &table.number);
		for (i = 0; i < count; i++)
		{
			if (table.number == tableNumbers[i]) { i = -1; break; }
		}
		if (i == -1) break;
		puts("That was not one of the available tables!");
	}
	fseek(ptr, (table.number - 1) * sizeof(struct tabledata), SEEK_SET);
	fread(&table, sizeof(struct tabledata), 1, ptr); //re-read selected table
	fseek(ptr, 0 - sizeof(struct tabledata), SEEK_SET); //move cursor to table for overwriting

	table.inUse = 1;
	printf("Table #%d now marked as in use.\n", table.number);
	fwrite(&table, sizeof(struct tabledata), 1, ptr);

	printf("Press any key to return to main menu...");
	fflush(stdin);
	_getch();

	fclose(ptr);
}

void showAllTables()
{
	FILE *ptr;

	ptr = fopen("tables.dat", "rb");
	if (ptr == NULL)
	{
		puts("Database of tables not found. Please add a table first");
		printf("Press any key to return to main menu...");
		fflush(stdin);
		_getch();
		return;
	}

	puts("Table no.\tNo. of Seats\tIn Use\t\tNo. of Bookings");
	puts("---------\t------------\t------\t\t---------------");

	fseek(ptr, 0, SEEK_SET);
	while (fread(&table, sizeof(struct tabledata), 1, ptr) != NULL)
	{
		if (table.number != 0)
		{
			printf("%d\t\t%d\t\t", table.number, table.seats);
			if (table.inUse == 0) printf("No\t\t");
			else if (table.inUse == 1) printf("Yes\t\t");
			printf("%d\n", countBookings());
		}
	}
	printf("Press any key to return to main menu...");
	fflush(stdin);
	_getch();
}

void editTables()
{
	int choice;

	while (1) //infinite loop, broken by entering 0
	{
		printf("\n1. Add a table\n2. Edit a table number\n3. Remove a table\n0. Return to main menu");
		choice = onlyInt(1);

		switch (choice){
		case 0: return;
		case 1: addTable(); break;
		case 2: editTableNumber(); break;
		case 3: removeTable(); break;
		default: puts("That is not one of the options!");
		}
	}
}

void addTable()
{
	int i;
	FILE *ptr;
	int tempTable; //local temporary number used to find if table number already exists

	ptr = fopen("tables.dat", "rb+");
	if (ptr == NULL)
	{
		ptr = fopen("tables.dat", "wb+");
		table.number = 0;
		for (i = 0; i < 100; i++)
		{
			fwrite(&table, sizeof(struct tabledata), 1, ptr); //file file with blank structures
		}
	}
	printf("\nEnter the new table number or '0' to exit:");
	do
	{
		tempTable = onlyInt(4);
		if (tempTable < 0) printf("Invalid input!\nEnter another table number or '0' to exit: ");
		else if (tempTable == 0) { fclose(ptr);  return; } //exit function when '0' is entered
		if (tempTable > 0)
		{
			i = searchTables(ptr, tempTable);
			if (i == 0)
			{
				printf("Table %d already exists.\nEnter another table number or '0' to exit: ", table.number);
			}
		}
	} while (i == 0);
	table.number = tempTable;
	do
	{
		printf("Enter number of seats at this table:");
		table.seats = onlyInt(3);
		if (table.seats < 1) puts("Invalid input!");
	} while (table.seats < 1);

	table.inUse = 0; //new tables are automatically not in use

	fseek(ptr, (table.number - 1) * sizeof(struct tabledata), SEEK_SET); //move cursor to table number location in file
	fwrite(&table, sizeof(struct tabledata), 1, ptr);
	puts("Table created.");
	fclose(ptr);
}

void editTableNumber()
{
	FILE *ptr;
	int tempTable; //temperary number to check against database
	int i;
	struct tabledata tempStruct;
	ptr = fopen("tables.dat", "rb+");
	if (ptr == NULL) { puts("No tables found. Please add a table first."); return; }
	printf("Enter the number of the table to edit: ");
	tempTable = onlyInt(4);

	if (searchTables(ptr, tempTable) != 0) //if searchTables doesn't return 0, table not found
	{
		puts("Table number does not exist!");
		fclose(ptr);
		return;
	}

	printf("Table #%d found\nSeats: %d\nBookings: %d\n", table.number, table.seats, countBookings());

	tempStruct = table;
	table.number = 0;
	table.seats = NULL; //change all values to NULL to 'delete'

	for (i = 0; table.booking[i].day != NULL; i++)
	{
		table.booking[i].day = NULL;
		table.booking[i].month = NULL;
		table.booking[i].year = NULL;
		table.booking[i].time = NULL;
	}
	fwrite(&table, sizeof(struct tabledata), 1, ptr);

	printf("Enter the new table number: ");

	while (1)
	{
		tempStruct.number = onlyInt(4);

		i = searchTables(ptr, tempStruct.number);
		if (i == 0)
		{
			printf("Table %d already exists.\nEnter another table number: ", table.number);
		}
		else break;
	}

	fseek(ptr, (tempStruct.number - 1) * sizeof(struct tabledata), SEEK_SET);
	fwrite(&tempStruct, sizeof(struct tabledata), 1, ptr);
	fclose(ptr);
}

void removeTable()
{
	FILE *ptr;
	int tempNumber = 0; //temperary number to check against database
	int i; //counter
	char choice;

	ptr = fopen("tables.dat", "rb+");
	if (ptr == NULL) { puts("No tables found."); return; }

	printf("Enter the number of the table to remove: ");
	fflush(stdin);
	scanf("%d", &tempNumber);

	if (searchTables(ptr, tempNumber) != 0) //if searchTables doesn't return 0, table not found
	{
		puts("Table number does not exist!");
		fclose(ptr);
		return;
	}

	printf("Table #%d found\nSeats: %d\nBookings: %d\nAre you sure you wish to delete it? <y/n> ", table.number, table.seats, countBookings());
	fflush(stdin);
	scanf("%c", &choice);
	choice = tolower(choice);

	if (choice == 'y')
	{
		table.number = 0;
		table.seats = NULL; //change all values to NULL to 'delete'

		for (i = 0; table.booking[i].day != NULL; i++)
		{
			table.booking[i].day = NULL;
			table.booking[i].month = NULL;
			table.booking[i].year = NULL;
			table.booking[i].time = NULL;
		}

		fseek(ptr, (table.number - 1) * sizeof(struct tabledata), SEEK_SET);
		fwrite(&table, sizeof(struct tabledata), 1, ptr);
		puts("Table deleted.");
	}
	else puts("Table not deleted.");

	fclose(ptr);
}

int searchTables(FILE *ptr, int number)
{
	fseek(ptr, 0, SEEK_SET); //move cursor to beginning of file for reading
	while (fread(&table, sizeof(struct tabledata), 1, ptr) != NULL)
	{
		if (table.number == number)
		{
			fseek(ptr, 0 - sizeof(struct tabledata), SEEK_CUR); //move cursor back to begininng of structure for overwriting
			number = 0; //indicates table is found
			break;
		}
	}
	return number;
}