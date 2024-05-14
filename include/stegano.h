#ifndef STEGANO_H
#define STEGANO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// reading size of image file
int size_of_image(FILE *fp1);

// Entering Secret Message to file
void secret_text(FILE *fp2);

// Size Of The Secret Message
int secret_text_size(FILE *fp2);

// bits of text file
int get_bit(char byte, int bit);

// encription of message
void stega_encrypt(FILE *fp1, FILE *fp2, FILE *fp3);

// Encription For Strings
void string_encrypt(char *str, FILE *fp1, FILE *fp3);

// Encription For Numbers
void size_encrypt(int num, FILE *fp1, FILE *fp3);

/* decryption of sizes */
void size_decryption(FILE *pf1, int *size_txt);

/* decryption of strings*/
void string_decryption(FILE *pf1, char *strng, int size);

/* decryption of secret message*/
void secret_decryption(int size_txt, FILE *pf1, FILE *pf2);

int Encoding(char *argv_2, char *argv_3, char *argv_4);

int Decode(char *argv_2, char *argv_4);

#endif
