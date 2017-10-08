#ifndef ANALYSESYNTAXIQUE_H_INCLUDED
#define ANALYSESYNTAXIQUE_H_INCLUDED
#include "header.h"
token current_token;

void system_goal();
void program();
void inst_list();
int inst();
int id_list();
int expresion();
int expr_list();
int add_opp();
int prim();
int match(token);
int syntax_error(token,char*);
token next_token();
char* token_to_char(token tok);
char aide[100];


void system_goal()
{
    current_token=scanner();
    program();
    match(SCANEOF);
}

/*
program: verifie que le prg commence par BEGIN, par la suite une liste d'insctuction et fini par un END
*/

void program()
{
    match(BEGIN);
    inst_list();
    match(END);
}

/*
inst_list: verifie si c'est une liste d'instruction(une instruction ou plusieurs)
*/
void inst_list()
{

    inst(); // instruction obligatoire
    while(1)// ou plusieurs
    {
        switch(next_token())// on recupere le token courant
        {   // les instructions commencent tjr par un ID ou READ ou WRITE
            case ID:
            case READ:
            case WRITE: inst(); // Dans tt ces cas[ID, READ ou WRITE], il faut executer inst()
                        break;
            default :
                return; //sinon c'est pas une inst, on sort de la fct inst_list
        }
    }
}

/*
inst: verifie si c'est une instruction(les instructions commence par ID ou READ ou WRITE, et chaqu'un a un chemin à parcourir)
*/
int inst()
{
    token tok=next_token();//on recupere le token courant

    switch(tok)
    {
        case ID:// instruction => ID ASSIGNOP <exp> ;
            if(match(ID))   return 1; //match retourne 0 si le token courant est celui expected, si match retourn 1 (c'est une erreur), du coup retourn 1 et on sort de la fct inst
            if(match(ASSIGNOP)) return 1;
            if(expression())    return 1;
            if(match(SEMICOLON))    return 1;
            break;
        case READ://instruction => READ LPAREN <list_id> RPAREN ;
            if(match(READ)) return 1;
            if(match(LPAREN))   return 1;
            if(id_list())   return 1;
            if(match(RPAREN))   return 1;
            if(match(SEMICOLON))    return 1;
            break;
        case WRITE://instruction => WRITE LPAREN <list_exp> RPAREN ;
             if(match(WRITE))   return 1;
             if(match(LPAREN))  return 1;
             if(expr_list())    return 1;
             if(match(RPAREN))  return 1;
             if(match(SEMICOLON))   return 1;
             break;
        default :
            if (syntax_error(tok,"INSTRUCTION"))  return 1;
            break;
    }
    return 0;
}

/*
id_list: verifie si c'est une list d'id( une liste d'id se compose d'id ID et un ensemble de (COMMA et ID))
*/

int id_list()
{

    if(match(ID))   return 1; //si ce n'est pas un ID on retourn 1 (une erreur)
    while(next_token()==COMMA)  //si le current token est un COMMA on parcours
    {
        if(match(COMMA))    return 1; //on verifie si c'est un COMMA et on passe au token suivant
        if(match(ID))   return 1;//on verifie si c'est un ID et on passe au token suivant
    }
    return 0;//retourn 0 => sans erreur
}

/*
expression: verifie si c'est une exprssion(une expression se compose de prim et un ensemble de (+ ou - et prim)
*/

int expression()
{

    token t;
    if(prim())  return 1; //on verifie ici le prim obligatoire, si c'est une erreur on retourn 1

    for(t=next_token();t==PLUSOP || t== MINUSOP;t=next_token()) //tand que current token est un - ou +
    {

        if(add_opp())   return 1; //verifie c'est un + ou -
        if(prim()) //puis verifier si c'est un prim
        {

            break;
        }
    }
    return 0;
}

/*
exp_list: verifie si c'est une list d'expression (une  list d'exp se compose d'une exp obligatoire et un ensemble de (COMMA et <exp>)
*/

int expr_list()
{

    if(expression())    return 1; //verifier 1ere exp obligatoire
    while(next_token()== COMMA)
    {
        if(match(COMMA))    return 1;
        if(expression())    return 1;
    }
    return 0;
}

/*
add_opp(): verifie si c'est une operation + ou -
*/
int add_opp()
{
    token t=next_token();// recup le token courant

    if(t==PLUSOP || t== MINUSOP) //si c'est un token
    {
        if(match(t))    return 1; //verifie le token et on passe au token suivant, sinon une erreur 1
    }
    else
    {
        if(syntax_error(t,"OPERATOR")) return 1; //sinon c'est pas un token => afficher l'erreur et retourn 1
    }

    return 0;//si il n y a aucune erreur on retourn 0
}

/*
token_to_char: on retourne le token recu en format chaine de caractere
*/
char* token_to_char(token tok)
{
        char chaine[100];

        switch(tok)
        {

             case 1 :  strcpy(chaine,"END");
                       break;
             case 2 :  strcpy(chaine,"READ");
                        break;
             case 3 :  strcpy(chaine,"WRITE");
                        break;
             case 4 :  strcpy(chaine,"BEGIN");
                       break;
             case 5 :  strcpy(chaine,"ID");
                       break;
             case 6 :  strcpy(chaine,"RPAREN");
                       break;
             case 7 :  strcpy(chaine,"PLUSOP");
                       break;
             case 8 :  strcpy(chaine,"MINUSOP");
                       break;
             case 9 :  strcpy(chaine,"INTLITERAL");
                       break;
             case 10 :  strcpy(chaine,"SEMICOLON");
                        break;
             case 11 :  strcpy(chaine,"COMMA");
                        break;
             case 12 :  strcpy(chaine,"ASSIGNOP");
                        break;
             case 13 :  strcpy(chaine,"LPAREN");
                        break;
             case 14 :  strcpy(chaine,"FLOATLITERAL");
                        break;
             default:
                printf ("\n erreur de syntaxe unknown");
        }

        return chaine;
}

/*
sauter_ligne: on saute à la ligne suivante
*/
void sauter_ligne()
{
    token t;


    do
    {
        t = scanner();

    }while (t!= SEMICOLON && t!=END && t!=SCANEOF);//tand que c'est pas un ; et pas un END et pas un scaneof => on continue à lire

    if(t!=END && t!=SCANEOF)
        t = scanner();//on recupere le 1ere token qui se trouve dans la ligne suivante
    current_token = t; //on change le current token avec celui qu'on a trouvé (le 1ere token qui se trouve dans la ligne suivante)

}

/*
syntax_error: affiche le msg erreur
*/

int syntax_error(token a,char* raison)
 {
        char chaine[100];
        char chaine2[100];
        strcpy(chaine2,raison);

        switch(a)
        {

             case 1 :  strcpy(chaine,"END");
                       break;
             case 2 :  strcpy(chaine,"READ");
                        break;
             case 3 :  strcpy(chaine,"WRITE");
                        break;
             case 4 :  strcpy(chaine,"BEGIN");
                       break;
             case 5 :  strcpy(chaine,"ID");
                       break;
             case 6 :  strcpy(chaine,"RPAREN");
                       break;
             case 7 :  strcpy(chaine,"PLUSOP");
                       break;
             case 8 :  strcpy(chaine,"MINUSOP");
                       break;
             case 9 :  strcpy(chaine,"INTLITERAL");
                       break;
             case 10 :  strcpy(chaine,"SEMICOLON");
                        break;
             case 11 :  strcpy(chaine,"COMMA");
                        break;
             case 12 :  strcpy(chaine,"ASSIGNOP");
                        break;
             case 13 :  strcpy(chaine,"LPAREN");
                        break;
             case 14 :  strcpy(chaine,"FLOATLITERAL");
                        break;
             default:
                printf ("\n erreur de syntaxe unknown");
        }

     printf ("\nerror: expected %s before '%s' (ligne:%d) (colon:%d)\n",chaine2,aide,nbLigne,nbColon);

     sauter_ligne();//lorsqu'on detecte une erreur, on passe à la ligne suivante
     return 1;
 }

int prim()
{
    token tok=next_token();//Recuperer le current token

    switch(tok)
    {
        case LPAREN: //prim => LPAREN <exp> RPAREN
            if(match(LPAREN))   return 1;
            if(expression())    return 1;
            if(match(RPAREN))   return 1;
            break;

        case ID: // prim => ID
            if(match(ID))   return 1;
        break;
        case INTLITERAL: // prim => INT
            if(match(INTLITERAL))   return 1;
            break;
        case FLOATLITERAL: //prim => FLOAT
            if(match(FLOATLITERAL)) return 1;
            break;
        default: //sinon une erreur
            if(syntax_error(tok,"PRIMITIVE"))   return 1;
        break;
    }
  return 0;
}


/*
Match: compare le token courant(current_token) avec celui attendu(t)
*/
int match(token t)
{

    if(t!=current_token)
    {   //si c'est diff => erreur
        if(syntax_error(current_token,token_to_char(t))) return 1;//si c'est une erreur on return 1
    }
    //sinon il est correct, on passe au token souvant
    current_token=scanner();

    return 0;//si c'est correct on retourn 0
}

/*
next_token: retourne le token courant
*/
token next_token()
{
    return(current_token);
}



#endif // ANALYSESYNTAXIQUE_H_INCLUDED

