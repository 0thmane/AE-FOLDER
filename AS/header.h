#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define max 100

typedef struct tableau
{
    char tab[max];
    int nb;
}tab;

FILE *fichier  ;
FILE *fichier_sortie ;
int nbLigne = 1;
int nbColon = 0;
tab token_buffer;


typedef enum
{
    SCANEOF,END,READ,WRITE,BEGIN,ID,RPAREN,PLUSOP,MINUSOP,INTLITERAL,SEMICOLON,COMMA,ASSIGNOP,LPAREN,FLOATLITERAL
}token;
void buffer_char(char);
void lexical_error(int);
void clear_buffer();
token check_reserved();
token scanner();
