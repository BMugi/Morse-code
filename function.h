#ifndef ENCODE_H_INCLUDED
#define ENCODE_H_INCLUDED
#include <stdio.h>
typedef struct letter_morse
{
    char letter;
    char morse[7]; // there at most 7 characters in morse alphabet
    int l;
}
Trnslt;


typedef struct btree
{
    char letter;
    struct btree *right,*left;
} btree;

char *read_long_line();

void morse (char *text,Trnslt *s);

void code (Trnslt *s);

btree *insert (btree *root,char c,char *s);

void search (btree *root,char *s);

btree *Allinsert (btree *root,Trnslt *s);

void decode (Trnslt *s);

void statisticEncd();

void statisticDecd (Trnslt *s);

void playGame (Trnslt *s);

char *read_from_file (char *filename);

void encode_file (Trnslt *s);

void morse_file (char *text,Trnslt *s, char* saving_file);

void decode_file (Trnslt *s);

void search_file (btree *root,char *s, char *saving_file);

#endif // ENCODE_H_INCLUDED
