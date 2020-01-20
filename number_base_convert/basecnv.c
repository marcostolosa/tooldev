#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "basecnvlib.h"


// params with a : after it requre an additional argument
static const char *optstring = "i:numo:";

void printHelp() {
	printf("\nBase conversion\nUsage:\n");
	printf("-i: input base\n-o: output base\ninput number\n");
	printf("Bases supported: decimal | d, binary | b, octal | o, hexadecimal | h\n");
	printf("eg. basecnv -i decimal -o octal 10\n\n");
}

int main(int argc, char *argv[]) {
	if(argc == 2 || argc < 6 || argc > 6 ) {
		printHelp();
		exit(0);
	}
	struct args arg_struct;
	int op =  getopt(argc, argv, optstring);
	while( op != -1 ) {
		switch(op) {
			case 'i': arg_struct.bin = optarg; break;
			case 'o': arg_struct.bout = optarg; break;
			default: printf("Too few parameters recieved"); printHelp(); exit(-1);
		}
		op = getopt(argc, argv, optstring);	
	}
	argc -= optind;
	argv += optind;

	if( argc > 1 ) {
		printf("Error: Too many parameters recieved\n");
		printHelp();
		exit(-1);
	}

	arg_struct.num_in	 = argv[0];

	//printf("\nbin: %s bout: %s num_in: %s, num_out: %s\n", arg_struct.bin, arg_struct.bout, arg_struct.num_in, arg_struct.num_out);

	if( !strcmp(arg_struct.bin, arg_struct.bout) ) {
		arg_struct.num_out = arg_struct.num_in;
		printf("\nbin: %s bout: %s num_in: %s\n", arg_struct.bin, arg_struct.bout, arg_struct.num_in);
		return 0;
	}

	// needed later for mem free
	// calling octal/hex/binary input, functions change struct vals
	// make a copy here to decide whether to free later 
	char *bin_cpy = arg_struct.bin;
	char *numin_cpy = malloc(strlen(arg_struct.num_in)+1);
	strcpy(numin_cpy, arg_struct.num_in);

	convBase(&arg_struct);
	
	printf("%s: %s, %s: %s\n", bin_cpy, numin_cpy, arg_struct.bout, arg_struct.num_out);
	free(numin_cpy);

	if(! strcmp(arg_struct.bout, "b") || ! strcmp(arg_struct.bout, "binary"))
		free(arg_struct.num_out);
	// octal required free
	if(! strcmp(arg_struct.bout, "o") || ! strcmp(arg_struct.bout, "octal"))
		free(arg_struct.num_out);
	// hexadecimal requires free
	else if(!strcmp(arg_struct.bout, "h") || ! strcmp(arg_struct.bout, "hexadecimal"))
		free(arg_struct.num_out);
	// binary, similar bases, error input - do not free ( because no alloc )


	// input base: octal, hex - need num_in freed
	if(! strcmp(bin_cpy, "b") || ! strcmp(bin_cpy, "binary"))
		free(arg_struct.num_in);
	// octal required free
	if(! strcmp(bin_cpy, "o") || ! strcmp(bin_cpy, "octal"))
		free(arg_struct.num_in);
	return 0;
}

