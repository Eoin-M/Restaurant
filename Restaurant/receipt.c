#include "declare.h"	//contains all the function prototypes 

void receipt()
{
		int payopt;	//card or cash
		int pass;	//pincode
		int tipNoTip;	//choice to tip
		int passtries = 3;	//errorchecking for pincode
		int i = 0, j = 0;	//counters
		float tip = 0;
		float tippedTotal = 0;	//overaall total if tip was given, total + tip
		int items;	//num of dishes on menu
		FILE *MenuPtr;
		char item[81];	//contains the order
		float total = 0;	//total to pay if not tipped

		MenuPtr = fopen("menu.dat", "rb");
		items = readMenu(MenuPtr);
		fclose(MenuPtr);

		strcpy(item, table.order[i]);	//copies the order from the table struct to the var so that reacipt can be given
		table.order[i][0] = 0;	//empties the table struct

		if (item[0] == '\0')	//if the table has no orders
		{
			puts("No order on this table currently!");
			printf("Press any key to return to main menu...");
			fflush(stdin);
			_getch();
			return;	//returns to the main menu
		}

		printf("\nYou have ordered the following: \n");
		printf("------------------------------------------------------------------------------\n");
		
		for (i = 1; item[0] != '\0'; i++)	//if there is an order in the struct
		{
			for (j = 0; j < items; j++)
			{
				if (!strcmp(item, (menu + j)->name)) {
					total += (menu + j)->price;	//a running total adding up price of each dish
					printf("%d %-40s %.2f\n", (menu + j)->ID, (menu + j)->name, (menu + j)->price);	//prints out all the dishes ordered
					break;
				}	//end if
			}	//end for
			strcpy(item, table.order[i]);
			table.order[i][0] = 0;
		}	//end for

		printf("=======================");
		printf("\nSUBTOTAL = %.2f \n", total);	
		printf("=======================\n");

		printf("\nChoose your payment method: 1-Credit Card 2-Cash   ");
		payopt = onlyInt(1);	//restricts the user t=to enter one integer only



		if (payopt == 1)	//pays by card, no tip
		{
			printf("\nInsert your card\n");
			Sleep(2000);	//sleeps for 2 secs
			printf(". . . Please wait. . . \n");
			Sleep(3000);
			printf(". . . Don't remove your card from the machine . . . \n");
			Sleep(7000);

			while (1)	//if paying by card
			{
				printf("Insert your pincode: ");
				pass = onlyInt(4);	//restricts the user to input only 4 integers
				if (!(pass - 1000 > 0 && pass - 9999 < 0)) { printf("\nWrong! You have %d attempts remaining", --passtries); }	//error checking
				else if(passtries ==0) { printf("You have no attempts remaining!"); return; }	//error checking
				else break;
			}

			if (passtries != 0)	//if the pincode is valid
			{
				Sleep(3000);
				printf("\n-----------------------------------------------------------------\n");
				printf("The total of E%.2f has been taken from your card\n", total);
				printf("\nThank you for ordering at \n");
			}	//end else
		}	//end if

		else if (payopt == 2)	//pays by cash
		{
			printf("\n================================\n");
			printf("The total adds up to %.2f \n", total);
			printf("================================\n");
			printf("** Would you like to give us a tip? 1-Yes 2-No");
			tipNoTip = onlyInt(1);	//restricts the user to input 1 int only

			switch (tipNoTip)
			{
			case 1:	//wants to tip
				printf("\nWhat amount would you like to tip us? ");
				tip = onlyPosFloat();	//restrits the user to only input a positive float
				tippedTotal = total + tip;	//overall total
				printf("\nYou have contributed %.2f to The Still Undecided.", tip);
				printf("\n========================= \nYour new Total is: %.2f \n===========================", tippedTotal);
				printf("\nThank you for ordering at ");
				break;

			case 2:	//no tip
				printf("The total is €%.2f", total);
				printf("\nThank you for ordering at ");
				break;

			default:	//invalid choice
				printf("\nYou have entered an invalid choice!");
				break;
			}	//end switch
		}	//end else
	
	screen();	//the end screen prints out
	
	table.inUse = 0;
}	//end function