// any number is first converted to decimal, after which decimal converted to -o param

#include <stdio.h>
#include "basecnvlib.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

struct args* convBase(struct args *arg_struct) {
	if(!strcmp(arg_struct->bin, "decimal") || !strcmp(arg_struct->bin, "d"))
		 decConv(arg_struct);
	else if(!strcmp(arg_struct->bin, "binary") || !strcmp(arg_struct->bin, "b")) {
		binConv(arg_struct);
		convBase(arg_struct);
	}
	else if(!strcmp(arg_struct->bin, "hexadecimal") || !strcmp(arg_struct->bin, "h")) {
		hexadecConv(arg_struct);
		convBase(arg_struct);
	}
	else if(!strcmp(arg_struct->bin, "octal") || !strcmp(arg_struct->bin, "o")) {
		octConv(arg_struct);
		convBase(arg_struct);
	}
	else {
		printf("\nBase conversion\nUsage:\n");
		printf("-i: input base\n-o: output base\ninput number\n");
		printf("Bases supported: decimal | d, binary | b, octal | o, hexadecimal | h\n");
		printf("eg. basecnv -i decimal -o octal 10\n\n");
		exit(-1);
	}
	
	return arg_struct;
}


// decimal
struct args* decConv(struct args *arg_struct) {
	
	// to binary
	if(!strcmp(arg_struct->bout, "binary") || !strcmp(arg_struct->bout, "b")) {
		int t = atoi( arg_struct->num_in );
		arg_struct->num_out = malloc(1);
		if(t == 0) {
			arg_struct->num_out = "0";
			return arg_struct;
		}
		if(t == 1) {
			arg_struct->num_out = "1";
			return arg_struct;
		}

		while( t != 0 ) {
			arg_struct->num_out = (char *) realloc(arg_struct->num_out, sizeof(arg_struct->num_out) + 1);	
			t % 2 == 0 ? strcat(arg_struct->num_out, "0") : strcat(arg_struct->num_out, "1");
			t /= 2;
		}
		// strrev does not exist in linux string.h
		arg_struct->num_out = reverseString(arg_struct->num_out);
		return arg_struct;
	}


	else if(!strcmp(arg_struct->bout, "octal") || !strcmp(arg_struct->bout, "o")) {
		int t = atoi( arg_struct->num_in );
		char *t_cc = (char *) malloc(1);
		arg_struct->num_out = malloc(1);
		
		while( t != 0 ) {
			arg_struct->num_out = (char *) realloc(arg_struct->num_out, sizeof(arg_struct->num_out) + 1);
			t_cc = (char *) realloc(t_cc, sizeof(arg_struct->num_out));	
			
			int t_c = t % 8;
			sprintf(t_cc, "%d", t_c);
			strcat(arg_struct->num_out, t_cc);
			t /= 8;
		}
		free(t_cc);
		
		// strrev does not exist in linux string.h
		arg_struct->num_out = reverseString(arg_struct->num_out);
		return arg_struct;
	}


	// to hexadecimal
	else if(!strcmp(arg_struct->bout, "hexadecimal") || !strcmp(arg_struct->bout, "h")) {
		int t = atoi( arg_struct->num_in );
		char *t_cc = (char *) malloc(1);
		arg_struct->num_out = malloc(1);

		while( t != 0 ) {
			arg_struct->num_out = (char *) realloc(arg_struct->num_out, sizeof(arg_struct->num_out) + 1);
			t_cc = (char *) realloc(t_cc, sizeof(arg_struct->num_out));	
			
			int t_c = t % 16;
			if( t_c <= 9 ) {
				sprintf(t_cc, "%d", t_c);
			}
			else {
				char temp_h;
				switch(t_c) {
					case 10: temp_h = 'A'; break;
					case 11: temp_h = 'B'; break;
					case 12: temp_h = 'C'; break;
					case 13: temp_h = 'D'; break;
					case 14: temp_h = 'E'; break;
					case 15: temp_h = 'F'; break;
				}
				sprintf(t_cc, "%c", temp_h);
			}

			strcat(arg_struct->num_out,  t_cc);
			t /= 16;
		}
		free(t_cc);

		// strrev does not exist in linux string.h
		arg_struct->num_out = reverseString(arg_struct->num_out);
		return arg_struct;
	}
	
	else if(!strcmp(arg_struct->bout, "decimal") || !strcmp(arg_struct->bout, "d")) {
		arg_struct->num_out = arg_struct->num_in;
		return arg_struct;
	}

	else {
		printf("\nBase conversion\nUsage:\n");
		printf("-i: input base\n-o: output base\ninput number\n");
		printf("Bases supported: decimal | d, binary | b, octal | o, hexadecimal | h\n");
		printf("eg. basecnv -i decimal -o octal 10\n\n");
		exit(-1);
	}

}


// binary - convert to decimal
void binConv(struct args *arg_struct) {
	// convert to decimal for input value num_in, then call decConv which will take decimal and convert
	long long int t = atoi(arg_struct->num_in);
	int i = 0;
	long long int dec_num_in = 0;

	while(t != 0) {
		int last_dig = t % 10;
		dec_num_in += last_dig * pow(2, i++);
		t /= 10;
	}
	arg_struct->num_in = (char *) malloc(sizeof(dec_num_in));
	sprintf(arg_struct->num_in, "%lld", dec_num_in);
	arg_struct->bin = "d";
	
}


// hexadecimal - convert to decimal
struct args* hexadecConv(struct args *arg_struct) {
	// need to get character from end to start of the string
	int i = 0, ind = 0;
	long long int dec_num_in = 0;
	for(i = 0; i < strlen(arg_struct->num_in); i++) {
		int t_cc;
		// 10-15 ie. A-F
		if(arg_struct->num_in[i] == 'A')
			t_cc = 10* pow(16, strlen(arg_struct->num_in)-1-i);
		else if(arg_struct->num_in[i] == 'B')
			t_cc = 11* pow(16, strlen(arg_struct->num_in)-1-i);
		else if(arg_struct->num_in[i] == 'C')
			t_cc = 12* pow(16, strlen(arg_struct->num_in)-1-i);
		else if(arg_struct->num_in[i] == 'D')
			t_cc = 13* pow(16, strlen(arg_struct->num_in)-1-i);
		else if( arg_struct->num_in[i] == 'E')
			t_cc = 14* pow(16, strlen(arg_struct->num_in)-1-i);
		else if( arg_struct->num_in[i] == 'F')
			t_cc = 15* pow(16, strlen(arg_struct->num_in)-1-i);
		// numbers 0-9
		else
			t_cc = (arg_struct->num_in[i]-48) * pow(16, strlen(arg_struct->num_in)-1-i);
	
		//ind += 1;
		dec_num_in += t_cc;	
	}
	sprintf(arg_struct->num_in, "%lld", dec_num_in);
	arg_struct->bin = "d";
	
}


// octal - convert to decimal
struct args* octConv(struct args *arg_struct) {
	// convert to octal for input value num_in, then call decConv which will take decimal and convert
	long long int t = atoi(arg_struct->num_in);
	int i = 0;
	long long int dec_num_in = 0;	
	
	while( t != 0 ) {
			int last_dig = t % 10;
			dec_num_in += last_dig * pow(8, i++);
			t /= 10;
	}

	arg_struct->num_in = (char *) malloc(sizeof(dec_num_in));
	sprintf(arg_struct->num_in, "%lld", dec_num_in);
	arg_struct->bin = "d";
}



char* reverseString(char *str) {
	char *p1, *p2;

    if (! str || ! *str)
        return str;
    for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2) {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }

      return str;
}