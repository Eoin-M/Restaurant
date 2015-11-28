#include "declare.h"

void printMenu()
{
	FILE *ptr;//pointer
	int i = 1;
	int j;
	int count;
	if ((ptr = fopen("menu.dat", "rb+")) == NULL)//opening the file and if it doesn't open (== NULL) alert
	{
		puts("Menu cannot be opened");
		return;
	}

	count = readMenu(ptr);// reads through the whole file and also used in for loops

	printf("\n%-6s%-40s%-8s%-6s\n", "ID", "Name", "Price", "Specs");
	printf("%-6s%-40s%-8s%-6s\n", "--", "----", "-----", "-----");
	puts("Starters");
	puts("--------");

	for (j = 0; j < count; j++)//counter that goes through the file and searches for dishes that have S for type
	{
		if (!strcmp((menu + j)->type, "S"))
		{
			printDish(i, j);
			i++;
		}
	}
	printf("\n");
	puts("Main");
	puts("----");

	for (j = 0; j < count; j++)//counter that goes through the file and searches for dishes that have M for type
	{
		if (!strcmp((menu + j)->type, "M"))
		{
			printDish(i, j);
			i++;
		}
	}
	printf("\n");
	puts("Desserts");
	puts("--------");
	for (j = 0; j < count; j++)//counter that goes through the file and searches for dishes that have D for type
	{
		if (!strcmp((menu + j)->type, "D"))
		{
			printDish(i, j);
			i++;
		}
	}
	printf("\n");
	puts("Side Dishes");
	puts("-----------");
	for (j = 0; j < count; j++)//counter that goes through the file and searches for dishes that have s for type
	{
		if (!strcmp((menu + j)->type, "s"))
		{
			printDish(i, j);
			i++;
		}
	}
	printf("\n");
	puts("Drinks");
	puts("------");
	for (j = 0; j < count; j++)//counter that goes through the file and searches for dishes that have d for type
	{
		if (!strcmp((menu + j)->type, "d"))
		{
			printDish(i, j);
			i++;
		}
	}
	printf("\nVE - Vegetarian\t VG - Vegan\tN - Contains Nuts\tGF - Gluten Free\tDF - Dairy Free");

	order(count);
	writeMenu(ptr, count);
	fclose(ptr);
}

int readMenu(FILE *ptr)
{
	int count;
	int j;

	menu = malloc(sizeof(struct menudata));//allocates memory of size struct menudata
	for (count = 1; (fread(menu, sizeof(struct menudata), 1, ptr)) != NULL; count++);//reads line by line to get the count
	menu = realloc(menu, count * sizeof(struct menudata));//reallocates memory of the size taken in the previous step
	rewind(ptr);//puts the cursor to the beginning of the file
	for (j = 0; j < count; j++)//reads the file once
	{
		fread((menu + j), sizeof(struct menudata), 1, ptr);
	}
	return count;
}

void writeMenu(FILE *ptr, int count)
{
	int j;

	rewind(ptr);
	for (j = 0; j < count; j++)//writes down the menu.dat at once
	{
		fwrite((menu + j), sizeof(struct menudata), 1, ptr);
	}
	fclose(ptr);
	free(menu);
}

void printDish(int id, int current)
{
	(menu + current)->ID = id;//print function that prints out id, name, price, spec of the dish
	printf("%-6d%-40s%-8.2f%-6s\n", (menu + current)->ID, (menu + current)->name, (menu + current)->price, (menu + current)->spec);
}

void editMenu()
{
	int choice = 1;

	while (choice != 0)
	{
		puts("Enter one of the following:");
		puts("1. Add a dish");
		puts("2. Update a dish");
		puts("3. Remove a dish");
		printf("0. Return to main menu");
		choice = onlyInt(1);//the user is asked to pick one of the switch statements and this function only allows one int to be inputted

		switch (choice)
		{
			case 1:
			{
				addMenu();
				break;
			}
			case 2:
			{
				updateMenu();
				break;
			}
			case 3:
			{
				removeMenu();
				break;
			}
			case 0:
			{
				return;
			}
			default:
			{
				puts("That is not one of the options!\nPlease enter again:");
				break;
			}
		}
	}
}

void addMenu()
{
	FILE *ptr;
	int i;
	int j = 0;
	int count;
	char choice[4];
	char temp[81];


	if ((ptr = fopen("menu.dat", "ab+")) == NULL)//opening of the file
	{
		ptr = fopen("menu.dat", "wb");
	}
	menu = malloc(sizeof(struct menudata));
	count = readMenu(ptr);
	do
	{
		puts("Enter the name of the dish:");
		fflush(stdin);
		gets(temp);

		for (j = 0; j < count; j++)//this for loop looks through the .dat
		{
			if (!strcmp(temp, (menu + j)->name))//if the variable temp is matched with existing entry on the file
			{
				puts("That dish already exists\n");//the user is alerted
				printf("Do you want to add another dish (y/n)? ");
				fflush(stdin);
				scanf("%s", choice);//the user has a choice to add another dish or not
				choice[0] = tolower(choice[0]);

				if (choice[0] != 'y') return;//if the choice isn't yes this exits the loop

			}

		}
	} while (choice[0] == 'y');
	strcpy(menu->name, temp);
	printf("Enter the price of the dish:");
	menu->price = onlyPosFloat();//only allows float
	do
	{
		puts("Enter the type of dish:");
		fflush(stdin);
		gets(menu->type);
		for (i = 0; i < strlen(menu->type); i++)//for the length of menu->type
		{
			menu->type[i] = toupper(menu->type[i]);//put into lower case
		}
		if (strstr(menu->type, "STARTER"))//if the string is matched to menu->type
		{
			strcpy(menu->type, "S");//it is given a new value (easier to sort the menu) 
			for (i = 1; i < strlen("STARTER"); i++)
			{
				menu->type[i] = 0;//this deletes the other letters from the string (the other functions are similar to that)
			}
			break;
		}
		else if (strstr(menu->type, "MAIN"))
		{
			strcpy(menu->type, "M");
			for (i = 1; i < strlen("MAIN"); i++)
			{
				menu->type[i] = 0;
			}
			break;
		}
		else if (strstr(menu->type, "DESSERT"))
		{
			strcpy(menu->type, "D");
			for (i = 1; i < strlen("DESSERT"); i++)
			{
				menu->type[i] = 0;
			}
			break;
		}
		else if (strstr(menu->type, "SIDE"))
		{
			strcpy(menu->type, "s");
			for (i = 1; i < strlen("SIDE"); i++)
			{
				menu->type[i] = 0;
			}
			break;
		}
		else if (strstr(menu->type, "DRINK"))
		{
			strcpy(menu->type, "d");
			for (i = 1; i < strlen("DRINK"); i++)
			{
				menu->type[i] = 0;
			}
			break;
		}
		else
		{
			continue;//if the user inputs something else this loop is repeated
		}
	} while (1);
	puts("Enter the specifications of the dish(examples: VE, VG, N, GF, DF):");
	fflush(stdin);
	gets(menu->spec);
	for (i = 0; i < strlen(menu->spec); i++)
	{
		menu->spec[i] = toupper(menu->spec[i]);//translates into capital letters
	}
	menu->ID = 0;
	fwrite(menu, sizeof(struct menudata), 1, ptr);//written into the file
	free(menu);
	fclose(ptr);
}

void updateMenu()
{
	FILE *ptr;//file pointer
	char dish[81];//var that takes in the name of the dish to be updated
	int i = 0;
	int count;
	char temp[81];
	int k;
	if ((ptr = fopen("menu.dat", "rb+")) == NULL)//if the file cannot be opened, it alerts the user
	{
		puts("Menu cannot be opened");//alert
		return;//exits the function if file cannot be accessed
	}
	count = readMenu(ptr);
	printf("Enter the name of the dish you want to update:\n");
	fflush(stdin);
	gets(dish);
	for (k = 0; k < strlen(dish); k++)//for the length of the dish
	{
		dish[k] = tolower(dish[k]);//one character by character is changed to lowercase
	}
	for (i = 0; i<count; i++)
	{
		strcpy(temp, (menu + i)->name);//copies into the temp from (menu+i)->name
		_strlwr(temp);//puts the entire string to lower case
			
		if (strstr(temp, dish))//if the string is matched 
		{
			printf("%s's current price is %.2f\n", (menu+i)->name, (menu+i)->price);
			printf("Enter the new price:");
			(menu+i)->price = onlyPosFloat();
			break;
		}
	}
	if (i == count)//if the string is not matched
	{
		printf("Dish not found\n");
		return;
	}

	printf("%s's price has been changed to %.2f\n\n", (menu+i)->name, (menu+i)->price);
	writeMenu(ptr,count);

}

void removeMenu()
{
	int i;
	int k;
	int count;
	char choice[4];
	char dish[81];
	char temp[81];
	FILE *ptr;

	if ((ptr = fopen("menu.dat", "rb+")) == NULL)
	{
		puts("Menu cannot be opened");
		return;
	}

	count = readMenu(ptr);
	printf("Enter the name of the dish you want to remove:\n");
	fflush(stdin);
	gets(dish);
	for (i = 0; i < count; i++)
	{
		if (strstr((menu+i), dish))
		{
			printDish((menu + i)->ID, i);//prints the dish and its variables
			printf("Do you want to remove this dish (y/n)? ");
			fflush(stdin);
			scanf("%s", choice);
			choice[0] = tolower(choice[0]);//puts the first character to lowercase

			if (choice[0] != 'y') return;// the first character is not y it exits the loop

			for (; i < (count - 1); i++)//stops at the last 
			{
				*(menu + i) = *((menu + i) + 1);//replaces the line with the previous one so that where the i is now equals to i+1
			}
			memset((menu + i), 0, sizeof(struct menudata));//sets the memory that the pointer points at to null
			puts("Dish removed");
			break;
		}
	}
	if (i == count)//if the dish is not found
	{
		printf("Dish not found\n");
	}
	else
	{
		writeMenu(ptr, count - 1); return;//writes down the menu the size count less one hence deleting the last one 
	}

}