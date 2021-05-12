#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "octal.h"
#include "binary.h"

char * dec_to_oct_string(int dec) {
	int size = (int)(log(dec)/log(8)) + 1;
	char * oct = ((char *)malloc(size * sizeof(char)));
	char * soct = oct;
	char * bin = dec_to_bin_string(dec, size * 3);
	
	int i = 0;
	char * threes = (char *) malloc(3 * sizeof(char));
	int val;
	for (; i < size; i++) {
		strncpy(threes, bin + i * 3, 3);
		val = bin_to_dec(threes);
		*oct = '0' + val;
		oct++;
	}
	*oct = '\0';
	
	return soct;
}

int oct_to_dec(char * oct) {
	int amount = 0;
	
	int size = strlen(oct);
	int i = 0;
	for (; i < size; i++) {
		amount += (int)pow(8, size - (i+1)) * (*(oct+i) - '0');
	}
	
	return amount;
}

