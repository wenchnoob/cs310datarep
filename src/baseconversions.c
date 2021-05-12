#include <stdio.h>
#include "binary.h"
#include "hex.h"
#include "octal.h"
#include "baseconversions.h"

int main() {
	
	int exit = 0;
	int choice;
	char repr[255];
	int val;
	
	while(!exit) {
		printhelp();
		scanf(" %d", &choice);
		printf("\n");
		switch(choice) {
			case -1:
				exit = 1;
				break;
			case 0:
				printf("Input the val: ");
				scanf(" %d", &val);
				printf("\nThe conversion is: %s", dec_to_bin_string(val, 0));
				break;
			case 1:
				printf("Input the val: ");
				scanf(" %d", &val);
				printf("\nThe conversion is: %s", dec_to_oct_string(val));
				break;
			case 2:
				printf("Input the val: ");
				scanf(" %d", &val);
				printf("\nThe conversion is: %s", dec_to_hex_string(val));
				break;
			case 3:
				printf("Input the val: ");
				scanf(" %s", repr);
				printf("\nThe conversion is: %d", bin_to_dec(repr));
				break;
			case 4:
				printf("Input the val: ");
				scanf(" %s", repr);
				printf("\nThe conversion is: %d", oct_to_dec(repr));
				break;
			case 5:
				printf("Input the val: ");
				scanf(" %s", repr);
				printf("\nThe conversion is: %d", hex_to_dec(repr));
				break;
			default:
				printf("Option not supported");
				break;
		}
		printf("\n\n");
	}
	
	return 0;
}

void printhelp() {
	printf(
	"This application allows for base conversions. Options are:\n\
	Input a 0 to convert a base 10 number to binary.\n\
	Input a 1 to convert a base 10 number to octal.\n\
	Input a 2 to convert a base 10 number to hexadecimal.\n\
	Input a 3 to convert a binary number to base 10.\n\
	Input a 4 to convert a octal number to base 10.\n\
	Input a 5 to convert a hexadecimal number to base 10.\n\
	Input a -1 to exit.\n\
What would you like to do?: "
	);
}

