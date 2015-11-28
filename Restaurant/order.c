#include "declare.h"

void order(int items)	
{
	int i = 0;	//for looping
	int j = 0;
	char orders[300];	//all the dishes the user orders
	int *ORDER;	//all the IDs that are ordered
	int length;	//length of the order
	int count = 1;
	int reorder, payopt;	//payoption
	
	
	float tippedTotal = 0;
	
	float tip;
	float total = 0;	//overall total
	

	printf("\n===============================================================================\n");
	printf("\t\t\t\t Welcome!\n");
	printf("===============================================================================\n");

	//printf("Asci char %c", 176); 

	do{
		total = 0;	//reinitialising the variables for the do/while loop
		count = 1;
		//ORDER = NULL;
		//*orders = "";
		printf("\nPlease enter all the items you would like to order \nSeperate orders by a space. ");
		fflush(stdin);
		gets(orders);	//the order, comma delimited

		length = strlen(orders);	//finds the length of the thing user entered and there'll be 1 less space than the num so by that we can find out how many # were ordered
		
		for (i = 0; i < length; i++)
		{
			if (orders[i] == ' '){ count++; }	//counts the number of dishes the user ordered
		}

		if (orders[length - 1] == ' ')	//error checking for if the last char is a space
		{
			//if the last char is a space
			count--;
		} 
		

		ORDER = calloc(sizeof(int), count);	//allocates array memory
		*ORDER = atoi(strtok(orders, " "));	//strtok() breaks up the order numbers at every told delimeter, atoi() changes str to int

		for (i = 1; i < count; i++)
		{
			*(ORDER + i) = atoi(strtok(NULL, " "));	//acts as a break point preventing infinite loops
		}	//end while

		printf("\nYou have ordered the following: \n");
		printf("------------------------------------------------------------------------------\n");

		for (i = 0; i < count; i++)
		{
			for (j = 0; j < items; j++)
			{
				if (*(ORDER + i) == (menu + j)->ID) {
					total += (menu + j)->price;	//calculates the total
					printf("%d %-40s %.2f\n", (menu + j)->ID, (menu + j)->name, (menu + j)->price);	//prints out the dish details that the user ordered to confinm the order
				}	//end if
			}	//end for
		}	//end for

		printf("\n** If you're unhappy with your order you can choose to reenter your purchase.\nWould you like to use this option now? 1-Yes 2-No  ");
		fflush(stdin);
		reorder = onlyInt(1);

		if (reorder == 1) { free(ORDER); continue; }
		for (i = 0; i < count; i++)
		{
			for (j = 0; j < items; j++)
			{
				if (*(ORDER + i) == (menu + j)->ID) {
					strcpy(table.order[i], (menu + j)->name);	//puts it into the table
				}	//end if
			}	//end for
		}
		free(ORDER);
		
	} while (reorder == 1);	
	if (reorder != 1)
	{
		printf("\nPress any key to return to main menu...");
		fflush(stdin);
		_getch();	//proceed to receipt, wont show any character but will simply continue
	}
}	//end function


void screen()
{
	
	printf("\n\n   ||||||  ||   ||  ||||||      ||||  ||||||  ||  ||      ||    \n");
	printf("     ||    ||   ||  ||         ||       ||    ||  ||      ||    \n");
	printf("     ||    |||||||  |||||       |||     ||    ||  ||      ||    \n");
	printf("     ||    ||   ||  ||            ||    ||    ||  ||      ||    \n");
	printf("     ||    ||   ||  ||||||     ||||     ||    ||  ||||||  ||||||\n");
	printf("\n");
	printf(" ||   ||  ||||   ||  ||||||    ||||||   |||||  ||  ||||||    |||||||  ||||||\n");
	printf(" ||   ||  || ||  ||  ||   ||   ||      ||      ||  ||   ||   ||       ||   ||\n");
	printf(" ||   ||  ||  || ||  ||    ||  |||||  ||       ||  ||    ||  |||||    ||    ||\n");
	printf(" ||   ||  ||   ||||  ||   ||   ||      ||      ||  ||   ||   ||       ||   ||\n");
	printf("  |||||   ||    |||  ||||||    ||||||   |||||  ||  ||||||    |||||||  |||||| \n");
	
	printf("\nPress any key to return to main menu...");
	fflush(stdin);
	_getch();	//upon pressing any key the user is returned to the main menu

}