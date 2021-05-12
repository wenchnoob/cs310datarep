#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct fixed_point {
	char sign;
	char * whole;
	char * fractional;
} fixed_point;

char z_check(char * bin);
char ** fit_w(char * bin1, char * bin2);
char * dec_to_bin_string(int dec, int ensuresize);
char * dec_to_signed_bin_string(int dec);
int bin_to_dec(char * bin);
int signed_bin_to_dec(char * bin);
char * bin_add(char * bin1, char * bin2);
char * bin_neg(char * bin);
char * bin_not(char * bin);
char * bin_and(char * bin1, char * bin2);
char * bin_or(char * bin1, char * bin2);
char * bin_xor(char * bin1, char * bin2);
char * fixed_point_decimal_to_floating_point_bin(double decifloat, int efield, int significand);
fixed_point * fixed_point_decimal_to_fixed_point_bin(double decifloat, int precision);
double floating_point_bin_to_dec(char * repr, int efield);

int main() {
	
	double fract;
	
	printf("Input a fraction to be converted: ");
	scanf(" %lf", &fract);
	
	fixed_point * fp = fixed_point_decimal_to_fixed_point_bin(fract, 10);
	printf("From main\nBinary of whole component: %s\nBinary of fractional component: %s\nSign: %c\n", fp->whole, fp->fractional, fp->sign);
	
	
	char * repr = fixed_point_decimal_to_floating_point_bin(fract, 3, 5);
	printf("Repr: %s\n", repr);
	printf("Reverted: %lf\n", floating_point_bin_to_dec(repr, 3));
	
	return 0;
}

double floating_point_bin_to_dec(char * repr, int efield) {
	int sign = *repr == '0' ? 1 : -1;
	
	char * exp = (char *) malloc(efield * sizeof(char));
	*(exp+efield) = '\0';
	strncpy(exp, repr+1, efield);
	int e = bin_to_dec(exp);
	int bias = (int)pow(2, efield-1) - 1;
	e-=bias;
	
	char * whole = (char *) malloc((strlen(repr)-efield) * sizeof(char));
	*(whole+(strlen(repr)-efield)) = '\0';
	strncpy(whole, repr+efield, (strlen(repr)-efield));
	int w = bin_to_dec(whole);
	
	double total = w * pow(2, e);
	
	printf("e %d w %d", e, w);
	
	return sign * total;	
}

int bin_to_dec(char * bin) {
	int amount = 0;
	
	int size = strlen(bin);
	int i = 0;
	for (; i < size; i++) {
		amount += (int)pow(2, size - (i+1)) * (*(bin+i) - '0');
	}
	
	return amount;
}


char * fixed_point_decimal_to_floating_point_bin(double decifloat, int efield, int significand) {
	
	fixed_point * fp = fixed_point_decimal_to_fixed_point_bin(decifloat, significand);
	
	char * whole = fp->whole;
	char * fractional = fp->fractional;
	
	
	while (*whole != '\0' && *whole != '1') whole += 1;
	
	int bias = (int)pow(2, efield-1) - 1;
	int e = strlen(whole) - 1 + bias;
	
	char * exp = dec_to_bin_string(e, efield);
	
	char * ans = (char *) malloc(sizeof(char));
	*ans = fp->sign;
	*(ans+1) = '\0';
	
	if (strlen(whole) > 0) whole += 1;
	
	
	strcat(ans, exp);
	strcat(ans, whole);
	//char predeci[] = "0";
	//if (abs(decifloat) > 1)  predeci[0] = '1';
	//else predeci[0] = '0';
	//strcat(ans, predeci);
	strncat(ans, fractional, significand - strlen(whole));
	
	
	return ans;
}

fixed_point * fixed_point_decimal_to_fixed_point_bin(double decifloat, int precision) {
	char sign = decifloat < 0 ? '1' : '0';
	fixed_point * fp = (fixed_point*) malloc(sizeof(fixed_point));
	
	decifloat = decifloat < 0 ? -decifloat : decifloat;
	
	int whole = (int)decifloat;
	char * bin_whole = dec_to_bin_string(whole, 0);
	
	
	char * fractional = (char *) malloc(precision * sizeof(char));
	*(fractional+precision) = '\0';
	int i = 0;
	double fract = decifloat - whole;
	for (; i < precision; i++) {
		fract *= 2;
		if (fract >= 1) {
			*(fractional+i) = '1';
			fract -= 1;
		} else {
			*(fractional+i) = '0';
		}
	}
	
	fp->sign = sign;
	fp->whole = bin_whole;
	fp->fractional = fractional;
	return fp;
}

char* dec_to_bin_string(int dec, int ensuresize) {
	int size = (int)(log(dec)/log(2)) + 1;
	size = size < ensuresize ? ensuresize: size;
	char * bin = ((char *)malloc(size * sizeof(char))) + size;
	
	
	*bin = '\0';
	while(size) {
		size--;
		bin--;
		*bin = dec % 2 + '0';
		dec >>= 1;
	}
	
	return bin;
}

char* dec_to_signed_bin_string(int dec) {
	int min_w = dec < 0 ? (int)ceil(log(-dec)/log(2)) + 1 : (int)ceil(log(dec+1)/log(2)) + 1;
	char * x = dec_to_bin_string(dec < 0 ? -dec: dec, min_w);
	x = dec < 0 ? bin_neg(x): x;
	if (dec < 0) *(x+4)= '1';
	return dec < 0 ? x+4 : x;
}

char * bin_not(char * bin) {
	int size = strlen(bin);
	char * not = (char *) malloc(size * sizeof(char));
	*(not+size) = '\0';
	for (int i = 0; i < size; i++) {
		*(not+i) = *(bin+i) == '0' ? '1' : '0';
	}
	
	char zero[] = "0";
	return bin_add(not, zero);
}

char * bin_neg(char * bin) {
	char one[] = "1";
	return bin_add(bin_not(bin)+4, one);
}

char z_check(char * bin) {
	int size = strlen(bin);
	for (int i = 0; i < size; i++) {
		if (*(bin+i) == '1') return '0';
	}
	return '1';
}

// return the flags + the reult of the addition
// flag order NZVC
char * bin_add(char * bin1, char * bin2) {
	char ** fitted = fit_w(bin1, bin2);
	int size = strlen(*fitted);
	
	bin1 = *fitted;
	bin2 = *(fitted+1);

	char * ans = (char *) malloc((size + 4) * sizeof(char));
	
	
	int prev_carry = 0;
	int carry = 0;
	int val;
	for (int i = size - 1; i >= 0; i--) {
		prev_carry = carry;
		val = *(bin1 + i) - '0' + *(bin2 + i) - '0' + carry;
		switch(val) {
			case 0:
				carry = 0;
				*(ans+i+4) = '0';
				break;
			case 1:
				carry = 0;
				*(ans+i+4) = '1';
				break;
			case 2:
				carry = 1;
				*(ans+i+4) = '0';
				break;
			case 3:
				carry = 1;
				*(ans+i+4) = '1';
				break;
		}
	}
	
	// ensure the string is ended;
	*(ans+size+4) = '\0';
	
	
	*ans = *(ans+4) == '1' ? '1' : '0';
	*(ans+1) = z_check(ans+4);
	*(ans+2) = prev_carry ^ carry ? '1' : '0';
	*(ans+3) = carry ? '1' : '0';

	
	return ans;
}

char ** fit_w(char * bin1, char * bin2) {
	int size_bin1 = strlen(bin1);
	int size_bin2 = strlen(bin2);
	
	int size = size_bin1 > size_bin2 ? size_bin1 : size_bin2;
	
	
	char * _bin1_ = (char *) malloc(size * sizeof(char));
	char * _bin2_ = (char *) malloc(size * sizeof(char));
	
	memset(_bin1_, '0', size - size_bin1);
	memset(_bin2_, '0', size - size_bin2);
	 
	char * temp1 = _bin1_;
	char * temp2 = _bin2_;
	
	temp1 += size - size_bin1;
	temp2 += size - size_bin2;
	
	strncpy(temp1, bin1, size_bin1);
	strncpy(temp2, bin2, size_bin2);
	
	*(_bin1_ + size) = '\0';
	*(_bin2_ + size) = '\0';

	char ** new_bins = (char **) malloc(sizeof(char *)); 
	
	*new_bins = _bin1_;
	*(new_bins+1) = _bin2_;
	
	return new_bins;
}
