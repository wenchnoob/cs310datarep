#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "hex.h"
#include "binary.h"

char * dec_to_hex_string(int dec) {
	int size = (int)(log(dec)/log(16)) + 1;
	char * hex = ((char *)malloc(size * sizeof(char)));
	char * shex = hex;
	char * bin = dec_to_bin_string(dec, size * 4);
	
	int i = 0;
	char * nibble = (char *) malloc(4 * sizeof(char));
	int val;
	for (; i < size; i++) {
		strncpy(nibble, bin + i * 4, 4);
		val = bin_to_dec(nibble);
		*hex = val < 10 ? '0' + val : 'A' + val % 10;
		hex++;
	}
	*hex = '\0';
	
	return shex;
}

int hex_to_dec(char * hex) {
	int amount = 0;
	
	int size = strlen(hex);
	int i = 0;
	for (; i < size; i++) {
		amount += (int)pow(16, size - (i+1)) * 
				(*(hex+i) - 'A' < 0? *(hex+i) - '0': *(hex+i) - 'A' + 10);
	}
	
	return amount;
}
