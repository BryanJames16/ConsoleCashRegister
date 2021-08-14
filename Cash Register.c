/*

	**  Cash Register
	**  
	**  Developed by Group 5 ~ BSIT 1-3
	**  Start:    21/12/14 21:23
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
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 999

void fnCashRegister (void);
void scrHeader (void);
void fnReceipt (FILE *dtbReceipt, char strTotalCode [MAX_SIZE] [MAX_SIZE], char strTotalItems [MAX_SIZE] [MAX_SIZE], int intTotalQty [MAX_SIZE], int intStock, int intCount, double dblTotalPrice [MAX_SIZE], double dblGTotal, double dblCash);
char fnGetCode (char strCode [MAX_SIZE]);
int fnGetQty (FILE *dtbStock, FILE *dtbReWrite, int intQty, int intStock, char strItemName [MAX_SIZE], char strCode [MAX_SIZE], double dblPrice);

int main (void)
	{
		
		fnCashRegister ();
		
		return (EXIT_SUCCESS);
	} // int main (void)
	

void fnCashRegister (void) {
		
		reStart:;
		FILE *dtbItem, *dtbStock, *dtbReWrite, *dtbReceipt;
		
		char charEnCor = '\0', charMore = '\0', charRestart = '\0', strCode [MAX_SIZE] = {"\0"}, strItemCode [MAX_SIZE] = {"\0"}, strItemName [MAX_SIZE] = {"\0"}, strStockCheck [MAX_SIZE] = {"\0"}, strTotalItems [MAX_SIZE] [MAX_SIZE] = {{"\0"}, {"\0"}}, strTotalCode [MAX_SIZE] [MAX_SIZE] = {{"\0"}, {"\0"}};
		int intQty = 0, intStock = 0, intCount = 0, intSearch = 0, intLength = 0, intCounter = 0, intTotalQty [MAX_SIZE] = {0};
		double dblPrice = 0, dblCash = 0, dblGTotal = 0, dblTotalPrice [MAX_SIZE] = {0};
		
		progStart:;
		fflush (stdin);
		fflush (stdout);
		system ("cls");
		
		// Check If Database Files Exists - Start
		dtbItem = fopen ("Item.txt", "r");
		dtbStock = fopen ("Stock.txt", "r");
			if (fgets (strItemCode, MAX_SIZE, dtbItem) == '\0' || fgets (strItemCode, MAX_SIZE, dtbItem) ==  "") {
				printf("\n\nAn Error has occured! Item Database has not yet been created!");
				getch ();
				exit (EXIT_FAILURE);
			} // if (fgets (strItemCode, MAX_SIZE, dtbItem) == '\0' || fgets (strItemCode, MAX_SIZE, dtbItem) ==  "")
			
			if (fgets (strItemCode, MAX_SIZE, dtbStock) == '\0' || fgets (strItemCode, MAX_SIZE, dtbStock) ==  "") {
				printf("\n\nAn Error has occured! Stock Database has not yet been created!");
				getch ();
				exit (EXIT_FAILURE);
			} // if (fgets (strItemCode, MAX_SIZE, dtbStock) == '\0' || fgets (strItemCode, MAX_SIZE, dtbStock) ==  "")
		fclose (dtbStock);
		fclose (dtbItem);
		// Check If Database Files Exists - End
		
		// Get Item Name and Price - Start
		scrHeader ();
		strCode [MAX_SIZE] = fnGetCode (strCode);
		dtbItem = fopen ("Item.txt", "r");
			while (intSearch != 1) {
				
				if (strncmp (strStockCheck, strCode, 8) != 0) {		// If not matched, then the program will fgets ()
					
					if (fgets (strStockCheck, MAX_SIZE, dtbItem) == NULL) {
						
						printf ("\tNo Valid Records has been found!");
						getch();
						fclose (dtbItem);
						goto progStart;
						
					} // if (fgets (strStockCheck, MAX_SIZE, dtbItem) == NULL)
					
				} // if (strncmp (strStockCheck, strCode, 8) != 0)
				else if (strncmp (strStockCheck, strCode, 8) == 0) {
					break;
				} // else if (strncmp (strStockCheck, strCode, 8) == 0)
				
			} // while (intSearch != 1)
			fflush (stdin);			// Clear all buffers in file stream
			fflush (stdout);
			sscanf (strStockCheck, "%8[^,], %[^0-9] %lf", strCode, strItemName, &dblPrice);   // assign each striong and integer to another variable
			intLength = strlen (strItemName);
			strItemName[intLength - 2] = '\0';
		fclose (dtbItem);
		// Get Item Name and Price - End
		
		// Get Item Stock - Start
		dtbStock = fopen ("Stock.txt", "r");
			while (intSearch != 1) {
				
				fgets (strStockCheck, MAX_SIZE, dtbStock);
				if (strncmp (strStockCheck, strCode, 8) == 0) {
					
					break;
					
				} // if (strncmp (strStockCheck, strCode, 8) == 0)
			} // while (intSearch != 1)
			
			sscanf(strStockCheck, "%[^,], %d", strCode, &intStock);		// separate stock code from the number of stocks
		fclose (dtbStock);
		// Get Item Stock - End
		
		intQty = fnGetQty (dtbStock, dtbReWrite, intQty, intStock, strItemName, strCode, dblPrice);
		
		if (intQty == 0) {
			goto progStart;
		} // if (intQty == 0)
		
		askCor:;
		fflush (stdin);
		printf("\n\tEntries Correct? (y/n):\t  ");
		scanf("%c", &charEnCor);
		
		charEnCor = tolower (charEnCor);
		switch (charEnCor) {
			case 'y' : 
				// Print edited data to another file and remove the older ones
				intStock = intStock - intQty;
				dtbStock = fopen ("Stock.txt", "r");
					dtbReWrite = fopen ("temp.txt", "a+");
					
						while (fgets (strStockCheck, MAX_SIZE, dtbStock) != '\0') {
							if (strncmp (strStockCheck, strCode, 8) != 0) {
								fprintf(dtbReWrite, "%s", strStockCheck);
							} // if (strncmp (strStockCheck, strCode, 8) != 0)
							else {
								fprintf (dtbReWrite, "%s, %d\n", strCode, intStock);
							} // else
						} // while (fgets (strStockCheck, MAX_SIZE, dtbStock) != '\0')
					
					fclose (dtbReWrite);
				fclose (dtbStock);
				
				remove ("Stock.txt");
				rename ("temp.txt", "Stock.txt");
				break;
			case 'n' : goto progStart; break;
			default: printf("\nInvalid Entry! Please try again!\n"); getch(); goto askCor;
		} // switch (charEnCor)
		
		
		strcpy (strTotalCode [intCount], strCode);
		strcpy (strTotalItems [intCount], strItemName);
		intTotalQty [intCount] = intQty;
		dblTotalPrice [intCount] = dblPrice;
		
		
		askMore:;
		fflush (stdin);
		printf("\tOrder more items? (y/n):  ");
		scanf("%c", &charMore);
		
		charMore = tolower (charMore);
		switch (charMore) {
			case 'y' : intCount++; goto progStart; break;
			case 'n' : break;
			default: printf("\nInvalid Entry! Please try again!\n"); getch(); goto askMore;
		} // switch (charMore)
		
		cashRec:;
		printf ("\tEnter recieved cash:  ");
		scanf ("%lf", &dblCash);
		
		for (intCounter = 0; intCounter < intCount + 1; intCounter++) {
			dblGTotal = dblGTotal + (dblTotalPrice [intCounter] * intTotalQty [intCounter]);
		}
		
		if (dblCash < dblGTotal) {
			printf ("\tEntered amount is smaller than the total price of items!\n");
			getch ();
			goto cashRec;
		}
		
		fnReceipt (dtbReceipt, strTotalCode, strTotalItems, intTotalQty, intStock, intCount, dblTotalPrice, dblGTotal, dblCash);
		getch();
		cashRegRest:;
		fflush (stdin);
		printf("\n\nRestart Cash Register? (y/n)\n");
		scanf("%c", &charRestart);
		
		charRestart = tolower (charRestart);
		switch (charRestart) {
			
			case 'y': goto reStart; break;
			case 'n': printf ("\t\t\t\tGOODBYE!\n"); getch (); break;
			default: printf("Invalid Entry! Please try again!"); getch (); goto cashRegRest;
			
		} // switch (charRestart)
		
	return;
	
} // void fnCashRegister (void)

void scrHeader (void) {
	
	printf("\t\t           ___   __   ____  _  _\n");      
    printf("\t\t          / __) / _\\ / ___)/ )( \\\n");            
    printf("\t\t         ( (__ /    \\\\___ \\) __ (\n");            
    printf("\t\t          \\___)\\_/\\_/(____/\\_)(_/\n");            
 	printf("\t\t ____  ____  ___  __  ____  ____  ____  ____\n");
	printf("\t\t(  _ \\(  __)/ __)(  )/ ___)(_  _)(  __)(  _ \\\n");
	printf("\t\t )   / ) _)( (_ \\ )( \\___ \\  )(   ) _)  )   /\n");
	printf("\t\t(__\\_)(____)\\___/(__)(____/ (__) (____)(__\\_)\n\n");
	
	printf("\t\t\t   +------------------+\n");
	printf("\t\t\t   |    Chum Bucket   |\n");
	printf("\t\t\t   +------------------+\n\n");
	
	return;
} // void scrHeader (void)

void fnReceipt (FILE *dtbReceipt, char strTotalCode [MAX_SIZE] [MAX_SIZE], char strTotalItems [MAX_SIZE] [MAX_SIZE], int intTotalQty [MAX_SIZE], int intStock, int intCount, double dblTotalPrice [MAX_SIZE], double dblGTotal, double dblCash) {
	
	int intCounter1 = 0, intItems = 0;
	double dblChange = 0;
	struct tm *dtmNow;
	time_t dtmRec;
	
	dtmRec = time (NULL);
	dtmNow = localtime (&dtmRec);
	dblGTotal = 0.00;	// initialized to prevent solution errors
	
	dtbReceipt = fopen ("Receipt.txt", "a");
	
	// Print Reciept to Console - Start
	system ("cls");
	fflush (stdout);
	fflush (stdin);
	printf("+--------------------------------------------------------------+\n");
	printf("                            Chum Bucket                         \n");
	printf("                Chum Bucket Store Warehouse - PUP\n");
	printf("               By: Chum Bucket Retail Systems inc.\n");
	printf("                5th Floor, East Wing, Main Bldg.\n");
	printf("                 Sta. Mesa, Manila, Philippines\n\n");
	printf("             Terminal: PUP-P1(1)  Serial: PLG574QJWS\n");
	printf("       Transaction: 02A56DV  Date: %s", ctime(&dtmRec));
	printf("                   Cashier: Adriano Villagracia\n\n");
	
	for (intCounter1 = 0; intCounter1 < intCount + 1; intCounter1++) {
		printf("  Item Code: %s\n", strTotalCode [intCounter1]);
		printf("  Item Name: %s\n", strTotalItems [intCounter1]);
		printf("  Price: Php %.2lf x %d\n\n", dblTotalPrice [intCounter1], intTotalQty [intCounter1]);
		dblGTotal = dblGTotal + (dblTotalPrice [intCounter1] * intTotalQty [intCounter1]);
		intItems = intItems + intTotalQty [intCounter1];
	} // for (intCounter1 = 0; intCounter1 < intCount + 1; intCounter1++)
	
	dblChange = dblCash - dblGTotal;
	
	printf(" --------------------------------------------------------\n");
	printf(" \t  %d     Item(s)            Php %.2lf\n", intItems, dblGTotal);
	printf(" \t          Cash             Php %.2lf\n", dblCash);
	printf(" \t                           -----------\n");
	printf(" \t                 CHANGE    Php %.2lf\n\n", dblChange);
	printf("             Thank you for shopping! Please come again!\n");
	printf("                This serves as your official Receipt.\n");
	printf("+--------------------------------------------------------------+\n");
	// Print Receipt to Console - End
	
	// Print Receipt to File - Start
	dblGTotal = 0.00; // initialize to prevent solution errors
	fprintf(dtbReceipt, "[Receipt Printing - start]\n");
	fprintf(dtbReceipt, "+--------------------------------------------------------------+\n");
	fprintf(dtbReceipt, "                            Chum Bucket                         \n");
	fprintf(dtbReceipt, "                Chum Bucket Store Warehouse - PUP\n");
	fprintf(dtbReceipt, "               By: Chum Bucket Retail Systems inc.\n");
	fprintf(dtbReceipt, "                5th Floor, South Wing, Main Bldg.\n");
	fprintf(dtbReceipt, "                 Sta. Mesa, Manila, Philippines\n\n");
	fprintf(dtbReceipt, "             Terminal: PUP-P1(1)  Serial: PLG574QJWS\n");
	fprintf(dtbReceipt, "       Transaction: 02A56DV  Date: %s", ctime(&dtmRec));
	fprintf(dtbReceipt, "                   Cashier: Adriano Villagracia\n\n");
	
	for (intCounter1 = 0; intCounter1 < intCount + 1; intCounter1++) {
		fprintf(dtbReceipt, "  Item Code: %s\n", strTotalCode [intCounter1]);
		fprintf(dtbReceipt, "  Item Name: %s\n", strTotalItems [intCounter1]);
		fprintf(dtbReceipt, "  Price: Php %.2lf x %d\n\n", dblTotalPrice [intCounter1], intTotalQty [intCounter1]);
		dblGTotal = dblGTotal + (dblTotalPrice [intCounter1] * intTotalQty [intCounter1]);
	} // for (intCounter1 = 0; intCounter1 < intCount + 1; intCounter1++)
	
	dblChange = dblCash - dblGTotal;
	
	fprintf(dtbReceipt, " --------------------------------------------------------\n");
	fprintf(dtbReceipt, " \t  %d     Item(s)             Php %.2lf\n", intItems, dblGTotal);
	fprintf(dtbReceipt, " \t          Cash              Php %.2lf\n", dblCash);
	fprintf(dtbReceipt, " \t                            -----------\n");
	fprintf(dtbReceipt, " \t                  CHANGE    Php %.2lf\n\n", dblChange);
	fprintf(dtbReceipt, "             Thank you for shopping! Please come again!\n");
	fprintf(dtbReceipt, "                This serves as your official Receipt.\n");
	fprintf(dtbReceipt, "+--------------------------------------------------------------+\n");
	fprintf(dtbReceipt, "[Receipt Printing - end]\n\n");
	// Print Receipt to File - Start
	
	fclose (dtbReceipt);
	
	return;
	
} // void fnReceipt (FILE *dtbReceipt, char strTotalCode [MAX_SIZE] [MAX_SIZE], char strTotalItems [MAX_SIZE] [MAX_SIZE], int intTotalQty [MAX_SIZE], int intStock, int intCount, double dblTotalPrice [MAX_SIZE])

char fnGetCode (char strCode [MAX_SIZE]) {
	
	fflush (stdin);
	fflush (stdout);
	printf("\tCode:      ");		// Get Item code from the user
	gets(strCode);
	
	return (strCode [MAX_SIZE]);
} // char fnGetCode (char strCode [MAX_SIZE])

int fnGetQty (FILE *dtbStock, FILE *dtbReWrite, int intQty, int intStock, char strItemName [MAX_SIZE], char strCode [MAX_SIZE], double dblPrice) {
	
	getQuantity:;
	fflush (stdin);
	fflush (stdout);
	system ("cls");
	
	scrHeader ();
	printf("\tCode:      %s --> %s [Php %.2lf] (%d)\n", strCode, strItemName, dblPrice, intStock);
	
	if (intStock == 0) {
		printf("\tQuantity:  Sorry! There's no stock left for this item! Try another!\n");
		intQty = 0;
		getch ();
	} // if (intStock == 0)
	
	else {
		printf("\tQuantity:  ");		// Get Number of Items to be orderd from the user
		scanf("%d", &intQty);
		
		if (intQty > intStock) {
			printf ("\tError! Entered amount is greater than the inventory stock!\n");
			getch();
			goto getQuantity;
		} // if (intQty > intStock)
		
		if (intQty <= 0) {
			printf ("\tError! Enter a valid number!\n");
			getch();
			goto getQuantity;
		} // if (intQty <= 0)
	}
	
	return (intQty);
} // int fnGetQty (FILE *dtbStock, FILE *dtbReWrite, int intQty, int intStock, char strItemName [MAX_SIZE], char strCode [MAX_SIZE], double dblPrice)
