#include "AnalyseSyntaxe.h"


void buffer_char(char in_char)
{
    token_buffer.tab[token_buffer.nb++] = in_char;
    //printf ("\n BUFFER_CHAR: %s\n", token_buffer.tab);
}

void lexical_error(int c)
{
    printf("\n\t  La Ligne de L'erreur est :  %d  \n\t L'Erreur Apres le signe   %c \n\n" ,nbLigne,c);
}


void clear_buffer()
{
    token_buffer.nb = 0;
    int i = 0;
    for (i = 0; i<max; i++) token_buffer.tab[i] = '\0';
}

token check_reserved()
{
    //printf ("\n-CHECK: %s (colon:%d)\n", token_buffer.tab, nbColon);
    if(strcmp(token_buffer.tab,"BEGIN")==0) return BEGIN;
    else if(strcmp(token_buffer.tab,"END")==0) return END;
    else if(strcmp(token_buffer.tab,"READ")==0) return READ;
    else if(strcmp(token_buffer.tab,"WRITE")==0) return WRITE;
    else
        return ID;
}

void getInt(int int_char)
{
    char c;
    buffer_char(int_char);
    for( c=fgetc(fichier) ;  isdigit(c) ; c=fgetc(fichier) )
    {
        buffer_char(c);
        nbColon++;
        //printf ("\n --> Lu: %c (colon:%d)", c, nbColon);
    }
    ungetc(c,fichier);
    if(c != '.')
        nbColon--;
    //printf ("\n --> Lu[Sortie]: %c (colon:%d)", c, nbColon);
}

void getChaine(int int_char)
{
    char c;
    buffer_char(int_char);   //isalnum: is alphanumerique (lettre & chiffre)
    for( c=fgetc(fichier) ; isalnum(c) || c=='_' ; c=fgetc(fichier))
    {
        buffer_char(c);
        nbColon++;
        //printf ("\n --> Lu: %c (colon:%d)", c, nbColon);
    }
    //printf ("\n --> Lu[Sortie]: %c (colon:%d)", c, nbColon);
    ungetc(c,fichier);
    ///nbColon--;
}

token scanner()
{
    int in_char , c ;

    clear_buffer();

    if(feof(fichier))  return SCANEOF;
    //Tand qu'on a pas arriver à la fin du fichie
    while( (in_char = fgetc(fichier))  != EOF)
    {

        nbColon++;
        //printf ("\n --> Lu: %c (colon:%d)", in_char, nbColon);
        //Si c'est un retour à la ligne
        if(in_char=='\n')
        {
            //printf ("\n ON A  TROUVER UN RETOUR , (nbColon:%d) (nbLigne:%d)", nbColon, nbLigne);
            nbLigne++;
            nbColon = 0;
        }
        //Si c'est un espace ou une tabulation
        else if((isspace(in_char)) || (in_char == '\t'))
        {
            //nbColon++;
            continue;
        }
        //Si c'est un caractere alphabetique (une lettre)
        else if (isalpha(in_char))
        {
            getChaine(in_char);
            //printf ("\n###BUFFER: %s", token_buffer.tab);
            strcpy(aide,token_buffer.tab);

            return check_reserved();
        }
        //Si c'est un nombre
        else if(isdigit(in_char))
        {
///
            getInt(in_char);
            //printf ("\n###BUFFER: %s", token_buffer.tab);
            in_char=fgetc(fichier);
            //printf ("\n## %c", in_char);
            nbColon++;
            if(in_char=='.') //Le case où XX.XX
            {
                buffer_char(in_char);
                c=fgetc(fichier);
                nbColon++;
                if(isdigit(c))
                {
                    //printf ("\n --> Lu: %c (colon:%d)", c, nbColon);
                    getInt(c);
                    c = fgetc(fichier);
                    nbColon++;
                    if (c == '.')   lexical_error(c); //Le cas où XX.XX.XX
                    else if (c == 'e') //Le cas où XX.XXeXX
                    {
                        getInt(c);
                        c = fgetc(fichier);
                        nbColon++;
                        ///nbColon--;
                    }
                    if (c!='='&&c!='+'&&c!='-'&&c!='/'&&c!=';'&&c!='*' && c!=' ' && c!=')' && c!='(' && c!=',')
                        lexical_error(c);
                    else
                    {
                        //printf ("\n ON A TROUVER #####: %c", c);
                        ungetc(c, fichier);
                        ///nbColon--;
                    }

                   // ungetc(c,fichier);
                    strcpy(aide,token_buffer.tab);
                    return FLOATLITERAL;
                }
                else if (c == 'e') //Le cas où XX.eXX
                {
                    getInt(c);
                    c = fgetc(fichier);
                    nbColon++;
                    if (c!='='&&c!='+'&&c!='-'&&c!='/'&&c!=';'&&c!='*' && c!=' ' && c!=')' && c!='(' && c!=',')
                        lexical_error(c);
                }
                else if (c!='='&&c!='+'&&c!='-'&&c!='/'&&c!=';'&&c!='*' && c!=' ' && c!=')' && c!='(' && c!=',')
                        lexical_error(c);
                else
                {
                    ungetc(c, fichier);
                    ///nbColon--;
                }
                strcpy(aide,token_buffer.tab);
                return FLOATLITERAL;
            }
            else if (in_char == 'e') //Cas où XXeXX
            {
                getInt(c);
                //ungetc(c,fichier);
            }
            else if (in_char!='='&&in_char!='+'&&in_char!='-'&&in_char!='/'&&in_char!=';'&&in_char!='*' && in_char!=' ' && in_char!=')' && in_char!='(' && in_char!=',')
                lexical_error(in_char);
            else
            {
                ungetc(in_char, fichier);
                ///nbColon--;
            }
            strcpy(aide,token_buffer.tab);
            return INTLITERAL;
        }
        else if(in_char == '.')
        {
            buffer_char(in_char);
            c=fgetc(fichier);
            nbColon++;
            if(isalpha(c))
            {
                getInt(c);
               lexical_error(in_char); // le cas d'erreur
            }
            else if (c!='='&&c!='+'&&c!='-'&&c!='/'&&c!=';'&&c!='*' && c!=' ' && c!=')' && c!='(' && c!=',')
                        lexical_error(c);
            else
            {
                //ungetc(c,f);
                strcpy(aide,token_buffer.tab);
                return FLOATLITERAL; // le cas ou on a .XX
            }
        }
        else if (in_char == '(')
        {
            //nbColon++;
            strcpy(aide,"(");
            return LPAREN;
        }
        else if (in_char == ')')
        {
            //nbColon++;
            strcpy(aide,")");
            return RPAREN;
        }
        else if (in_char == ';')
        {
            //nbColon++;
            strcpy(aide,";");
            return SEMICOLON;
        }
        else if (in_char == ',')
        {
            //nbColon++;
            strcpy(aide,",");
            return COMMA;
        }
        else if (in_char == '+')
        {
            //nbColon++;
            strcpy(aide,"+");
            return PLUSOP;
        }
        else if (in_char == ':')
        {
            //nbColon++;
            c=fgetc(fichier);
            nbColon++;
            if(c == '=')
            {
                strcpy(aide,"=");
                return ASSIGNOP;
            }
            else
            {

                ungetc(c,fichier);
                nbColon--;
                lexical_error(in_char);
            }
        }
        else if (in_char == '-')
        {
            //nbColon++;
            c=fgetc(fichier);
            nbColon++;
            if( c == '-')
            {
                do
                {
                    //nbLigne ++ ;
                    in_char = fgetc(fichier);
                    //nbColon++;
                    //printf ("\n Passer par la !!!!!!!!");
                }
                while(in_char != '\n');
                nbLigne++;
                nbColon = 0;
            }
            else
            {
                ungetc(c,fichier);
                nbColon--;
                strcpy(aide, "-");
                return MINUSOP;
            }
        } else lexical_error(in_char);
    }
    return SCANEOF;
}



void FuntMain()
{
    //printf ("%f", .51);
    int ret ; // ---- Resultat de la fonction Scanner
    char * document = "programme.txt" ; // --- Fichier de lecture
    char * document_2 = "resultat.txt" ; // ---- Fichier de stockage
    fichier =  fopen (document, "r"); // ----- Ouvrire le fichier en mode lecture

    fichier_sortie =  fopen (document_2, "w+"); // ----- Le fichier de stockage de resultat


    // ---- Si le fichier n'existe pas

    if(fichier == NULL)
    {
      printf (" ERREUR ! Fichier N'existe Pas  ");

    }
    // ----- Sinon
    else
    {
     printf("\n ****************** L'Analyse Lexiqual **************** : \n\n ");
        while( ( ret = scanner() ) != 0) // ---- Tant que c'est pas la fin de fichier ==> # de scaneof
        {

        switch(ret)
        {
            //  Ecriture dans fichier resultat
             case 1 :  fprintf(fichier_sortie,"%s"," END");
                       break;
             case 2 :  fprintf(fichier_sortie,"%s"," READ");
                        break;
             case 3 :  fprintf(fichier_sortie,"%s"," WRITE");
                        break;
             case 4 :  fprintf(fichier_sortie,"%s"," BEGIN");
                       break;
             case 5 :  fprintf(fichier_sortie,"%s"," ID");
                       //printf("ID \t");
                       break;
             case 6 :  fprintf(fichier_sortie,"%s"," RPAREN");
                       //printf("RPAREN \t");
                       break;
             case 7 :  fprintf(fichier_sortie,"%s"," PLUSOP");
                       //printf("PLUSOP \t");
                       break;
             case 8 :  fprintf(fichier_sortie,"%s"," MINUSOP");
                       //printf("MINUSOP \t");
                       break;
             case 9 :  fprintf(fichier_sortie,"%s"," INTLITERAL");
                       //printf("INTLITERAL \t");
                       break;
             case 10 :  fprintf(fichier_sortie,"%s"," SEMICOLON");
                        //printf("SEMICOLON \t");
                        break;
             case 11 :  fprintf(fichier_sortie,"%s"," COMA");
                        //printf("COMA \t");
                        break;
             case 12 :  fprintf(fichier_sortie,"%s"," ASSIGNOP");
                        //printf("ASSIGNOP \t");
                        break;
             case 13 :  fprintf(fichier_sortie,"%s"," LPAREN");
                        //printf("LPAREN \t");
                        break;
             case 14 :  fprintf(fichier_sortie,"%s"," FLOATLITERAL");
                        //printf("FLOATLITERAL \t");
                        break;

        }
        }
        fprintf(fichier_sortie, "%s", " SCANEOF");
        fclose(fichier_sortie);
        FILE *file;
        file = fopen("resultat.txt", "r");
        if (file)
        {
            char c;
            while ((c = getc(file)) != EOF)
                putchar(c);
            fclose(file);
        }
    }
}

void lire_fichier()
{
            FILE *file;
        file = fopen("programme.txt", "r");
        if (file)
        {
            char c;
            while ((c = getc(file)) != EOF)
                putchar(c);
            fclose(file);
        }
}

void menu()
{
    int c=-1;

    while(c!=0)
    {
    printf(" \t\t\t\t\n =============       Veuillez choisir une action      ======================== \n");
    printf("\n 1 : Afficher le code source");
    printf("\n 2 : Effectuer l'analyse lexical");
    printf("\n 3 : Effectuer l'analyse syntaxique");
    printf("\n 0 : Quitter le programme\n");
    scanf("%d",&c);
       switch(c)
    {
        case 1: printf(" Le code source : \n");
        lire_fichier();
        break;
        case 2 :  //printf("Le resultat de l'analyse lexical : \n");
        FuntMain();
        break;
        case 3 :  //printf("Le resultat de l'analyse syntaxique : \n");
        system_goal();
        break;
        case 4: system("cls");
        break;
        case 0 : break;
        default : printf("Choix incorrecte");
        break;
    }


    }

}















int main()
{
    //FuntMain();
    //return 0;
    //printf("%s",);

    current_token = 15;
    fichier = fopen("programme.txt","r");
    menu();

    return 0;
}
