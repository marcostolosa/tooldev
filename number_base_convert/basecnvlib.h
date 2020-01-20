#ifndef H_BASECNVLIB
#define H_BASECNVLIB

struct args {
	char *bin;
	char *bout;
	char *num_in;
	char *num_out;
};

struct args* convBase(struct args*);
struct args* decConv(struct args*);
void binConv(struct args*);
struct args* hexadecConv(struct args*);
struct args* octConv(struct args*);
char* reverseString(char*);

#endif