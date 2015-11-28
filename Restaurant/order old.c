//strtok(), array for each food(starter, main etc...), int option[20][20], memory allocation malloc
//do array with i and strtok so that it loops automatically
//error check for the space, add tip to waiter, add money to table
#include "declare.h"


void order(int items)	//Change to int order(FILE *pt) later
{
	int i = 0;	//for looping
	int j = 0;
	char orders[300];
	int *ORDER;	//all the IDs that are ordered
	int length;
	int count = 1;
	int reorder, payopt;	//payoption
	int passtries = 3;
	int tipNoTip;
	char pass[5];
	
	float tippedTotal = 0;
	
	float tip;
	float total = 0;	//overall total
	

	printf("\n===============================================================================\n");
	printf("\t\t\t\t Welcome!\n");
	printf("===============================================================================\n");

	//printf("Asci char %c", 176); 

	do{
		total = 0;	//reinitialising the wariables for the do/while loop
		count = 1;
		ORDER = NULL;
		*orders = "";
		printf("\nPlease enter all the things you would like to order \nSeperate orders by a space. ");
		fflush(stdin);
		gets(orders);

		length = strlen(orders);	//finds the length of the thing user entered and there'll be 1 less space than the num so by that we can find out how many # were ordered
		for (i = 0; i < length; i++)
		{
			if (orders[i] == ' '){ count++; }
		}

		ORDER = malloc(count * sizeof(int));	//allocates memory
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
					total += (menu + j)->price;
					printf("%d %-40s %.2f\n", (menu + j)->ID, (menu + j)->name, (menu + j)->price);
				}	//end if
			}	//end for
		}	//end for
		printf("=======================");
		printf("\nSUBTOTAL = %.2f \n", total);	//  ****** checker **********
		printf("=======================\n");

		printf("** If you're unhappy with your order you can choose to reenter your purchase.\nWould you like to use this option now? 1-Yes 2-No  ");
		fflush(stdin);
		scanf("%d", &reorder);	
		free(ORDER);
		
	} while (reorder == 1);	//end do (89 and 121) ************* breaks here ******************
	
	if (reorder == 2)	//if they're happy with their order and want to continue
	{
		printf("\nChoose your payment method: 1-Credit Card 2-Cash   ");
		scanf("%d", &payopt);
		
		

		if (payopt == 1)	//pays by card, no tip
		{
			printf("\nInsert your card\n");
			Sleep(2000);
			printf(". . . Please wait. . . \n");
			Sleep(3000);
			printf(". . . don't remove your card from the machine . . . \n");
			Sleep(7000);

			printf("Insert your pincode: ");
			scanf("%s", &pass);
			if (strlen(pass) != 4) { printf("\nWrong! You have %d attempts remaining", passtries--); }
		
			else if (strlen(pass)==4)
			{
				Sleep(3000);
				printf("\n-----------------------------------------------------------------\n");
				printf("The total of E%.2f has been taken from your card\n", total);
				printf("\nThank you for ordering at The Still Undecided\n"); 
			}	//end else
		}	//end if

		else if (payopt == 2)	//pays by cash, have to add the tip to the waiter and the table
		{
			printf("\n================================\n");
			printf("The total adds up to %.2f \n", total);
			printf("================================\n");
			printf("** Would you like to give us a tip? 1-Yes 2-No");
			scanf("%d", &tipNoTip);

			switch (tipNoTip)
			{
			case 1:	//wants to tip
				printf("\nWhat amount would you like to tip us? ");
				scanf("%f", &tip);
				tippedTotal = total + tip;
				printf("\nYou have contributed E%.2f to The Still Undecided.", tip);
				printf("\n========================= \nYour new Total is: %.2f \n===========================", tippedTotal);
				printf("\nThank you for ordering at The Still Undecided ");
				break;

			case 2:	//no tip
				printf("The total is €%.2f", total);
				printf("\nThank you for ordering at The Still Undecided ");
				break;

			default:	//invalid choice
				printf("\nYou have entered an invalid choice!"); 
				break;
			}	//end switch
		}	//end else
	}	//end if
	screen();
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

}