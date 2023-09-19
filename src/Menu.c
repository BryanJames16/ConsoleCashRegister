/*

	**  Cash Register MENU
	**  
	**  Developed by Group 5 ~ BSIT 1-3
	**  Start:    23/12/14 18:05
	**  End:      29/12/14 21:39
	**  Modified: 25/01/15 19:55
	
*/

/*
	**  Group 5
	**  
	**  Khen Khen Gaviola
	**  Earl Dixon Geraldez
	**  Amiel Valerie Golosinda
	**  Bryan James Ilaga
	**  Charles Adrian Lana
*/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 999

int main (void)
	{
		
		FILE *dtbItem;
		char strStockCheck [MAX_SIZE] = {'\0'}, strStockName [MAX_SIZE] = {'\0'}, strCode [MAX_SIZE] = {'\0'};
		double dblPrice = 0.00;
		int intSearch  = 0;
		
		printf ("[Chum Bucket Menu]\n\n");
		
		dtbItem = fopen ("Item.txt", "r");
			while (intSearch != 1) {
				if (fgets (strStockCheck, MAX_SIZE, dtbItem) != NULL) {		// Get All data from file line by line
					sscanf (strStockCheck, "%8[^,], %[^,], %lf", strCode, strStockName, &dblPrice);		// Segregate data collected
					printf("Item Name: %s\nItem Price: %.2lf\nItem Code: %s\n\n", strStockName, dblPrice, strCode);		// Display Segregated Data
				} // if (fgets (strStockCheck, MAX_SIZE, dtbItem) != NULL)
				else {
					break;
				} // else
			} // while (intSearch != 1)
			printf ("Nothing Follows");
		fclose (dtbItem);
		
		getch();
		return (EXIT_SUCCESS);
		
	} // int main (void)

