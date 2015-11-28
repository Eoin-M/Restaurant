#include "declare.h"

int checkPassword()
{
	FILE *ptr;
	int i, j, k, m; //loop counters
	char userPassword[14];
	char password[81];
	char tempKey[5];
	int key, keyMod, calculation;

	ptr = fopen("password.dat", "rb");
	fread(&password, sizeof(password), 1, ptr);
	fclose(ptr);

	for (i = 2; i >= 0; i--)
	{
		if (i < 2) puts("Enter password:");
		k = 0;
		while (1)
		{
			fflush(stdin);
			userPassword[k] = _getch();
			if (userPassword[k] == 3) return;
			if ((userPassword[k] == 13 || userPassword[k] == 10) && k > 0) { userPassword[k] = 0; break; }
			if (userPassword[k] == 8 || userPassword[k] == 127)
			{
				if (k > 0) k--;
				printf("\r"); //move cursor to begininning of line
				for (j = 0; j <= k; j++)
				{
					printf(" ");
				}
				printf("\r");
				for (j = 0; j < k; j++) //print 1 less character than before
				{
					printf("*");
				}
				userPassword[k] = 0;
			}
			else if (userPassword[k] < 33 || userPassword[k] > 126) { userPassword[i] = 0; continue; }
			else
			{
				printf("*");
				k++;
			}
		}
		puts("");

		puts("Enter 4 digit key:");

		m = 0;
		while (1)
		{
			fflush(stdin);
			tempKey[m] = _getch();
			if (tempKey[m] == 3) return;
			if ((tempKey[m] == 13 || tempKey[m] == 10) && m == 4) { tempKey[m] = 0; break; }
			if (tempKey[m] == 8 || tempKey[m] == 127)
			{
				if (m > 0) m--;
				printf("\r"); //move cursor to begininning of line
				for (j = 0; j <= m; j++)
				{
					printf(" ");
				}
				printf("\r");
				for (j = 0; j < m; j++) //print 1 less character than before
				{
					printf("*");
				}
				tempKey[m] = 0;
			}
			if (m == 4) continue;
			else if (tempKey[m] < '0' || tempKey[m] > '9') { tempKey[m] = 0; continue; }
			else
			{
				printf("*");
				m++;
			}
		}
		puts("");
		key = atoi(tempKey);
		keyMod = key % 10; //extra encription

		for (j = 0; j < k; j++)
		{
			calculation = userPassword[j];
			calculation -= 33;
			calculation *= key;
			calculation += keyMod;
			calculation %= 94;
			calculation += 33;
			userPassword[j] = calculation;
		}
		userPassword[k] = 0; //ends string after necessary characters for strcmp
		if (!strcmp(userPassword, password)) return 1;
		if (i > 0) printf("Password and key combination is incorrect. %d attempts remaining.\n", i);
	}
	return 0;
}

void changePassword()
{
	int password;
	char newPassword[81];
	int newKey, keyMod; //keyMod used in encription process
	int calculation; //used in the encription process to generate encripted passcode
	int i; //loop counters
	int flag[3] = { 1, 1, 1 }; //check for valind password
	FILE *ptr;

	puts("Please re-enter the administrator password. 3 attempts remaining:");
	password = checkPassword();

	system("cls");
	if (password == 0) { puts("Password and Key combination incorrectly entered 3 times.\nProgram is exiting..."); return; }
	else puts("Password and key combination accepted.");

	puts("\nNew password must contain at least one uppercase letter, one lower case letter\nand one number, and be no less than 6 digits and no longer than 13 digits\nin length");
	while (flag[0] == 1 || flag[1] == 1 || flag[2] == 1)
	{
		flag[0] = 1; flag[1] = 1; flag[2] = 1;
		puts("Enter new password:");
		fflush(stdin);
		gets(newPassword);
		for (i = 0; i < strlen(newPassword); i++)
		{
			if (strlen(newPassword) > 13) { puts("Password must 13 digits or less"); break; }
			if (strlen(newPassword) < 6) { puts("Password must 6 digits or more"); break; }
			if (newPassword[i] >= 65 && newPassword[i] <= 90) flag[0] = 0; //uppercase letter
			else if (newPassword[i] >= 97 && newPassword[i] <= 122) flag[1] = 0; //lowercase letter
			else if (newPassword[i] >= 48 && newPassword[i] <= 57) flag[2] = 0; //number
			else if (newPassword[i] < 33 || newPassword[i] > 126)
			{
				printf("Character %c is unsupported!\n", newPassword[i]);
				flag[0] = 1; flag[1] = 1; flag[2] = 1; //reset flags
				break;
			}
		}
		if (i == strlen(newPassword) && flag[0] == 1) puts("Password must contain at least one uppercase letter!");
		if (i == strlen(newPassword) && flag[1] == 1) puts("Password must contain at least one lowercase letter!");
		if (i == strlen(newPassword) && flag[2] == 1) puts("Password must contain at least one number!");
	}
	puts("Password accepted");

	while (flag[0] == 0)
	{
		puts("\nNew key must be four digits in length, not beginning with '0'");
		puts("Enter new key:");
		newKey = onlyInt(4);
		if ((newKey - 1000) >= 0 && (newKey - 10000) < 0) flag[0] = 1; //key accepted
	}
	puts("Key accepted");
	puts("Password and Key combination has been changed.");

	keyMod = newKey % 10; //extra encription

	for (i = 0; i < strlen(newPassword); i++)
	{
		calculation = newPassword[i];
		calculation -= 33;
		calculation *= newKey;
		calculation += keyMod;
		calculation %= 94;
		calculation += 33;
		newPassword[i] = calculation;
	}
	ptr = fopen("password.dat", "wb");
	fwrite(&newPassword, sizeof(newPassword), 1, ptr);
	fclose(ptr);
	puts("Program needs to be restarted"); //restart program after changing password
}