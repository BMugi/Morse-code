#include "function.h"
#include <stdio.h>
#include <stdlib.h>


void printmenu()
{
    printf("\n");
    printf("\nOnly the above characters are allowed.\n"
           "\nChoose your option:\n"
           "1. Text into Morse\n"
           "2. Morse into text (Use a single and double space to separate letters and words)\n"
           "3. Play game!\n"
           "4. Encode file (Please write in correct form e.g. text.txt)\n"
           "5. Decode file (Please write in correct form e.g. text.txt)\n"
           "6. Exit\n");
}

/*this function will read input until newline. After use must free it because using dynamic array */
char *read_long_line()
{
    int cnt = 0;
    char *line = (char*) malloc(sizeof(char) * 1); // initializing dynamic array
    line[0] = '\0';
    char newchar;
    while (scanf("%c", &newchar) == 1 && newchar != '\n') //reading new character
    {
        // using while loop to do it several times
        char *newarr = (char*) malloc(sizeof(char) * (cnt+1+1)); //array extension by creating new one
        for (int i = 0; i < cnt; ++i)
            newarr[i] = line[i];               // copying from old array to new one
        newarr[cnt] = newchar;                 //initializing new char in the new created array
        newarr[cnt+1] = '\0';
        free(line);
        line = newarr;                         //redirecting line pointer point to new created array
        ++cnt;
    }                                          // repeating it until \n
    return line;                               // returning new extended array
}

/* this function will print corresponding Morse code for each letter of your text. */
void morse (char *text,Trnslt *s)
{
    FILE *f; // initializing f file
    f=fopen("encode.txt","a"); // opens f file
    if (text!=NULL)  //checking if file is successfully opened
    {
        int i,l=strlen(text);
        for (i=0; i!=l; ++i) // going through string that we take as parameter "text"
        {
            for (int j=0; j!=50; ++j) //going through the translate table
            {
                if (tolower(text[i])==s[j].letter) // using tolower function because user can use capital letter which is same to the lower in morse code
                {
                    printf("%s ",s[j].morse);       // printing on the screen corresponding Morse code
                    fprintf(f,"%s ",s[j].morse); // printing corresponding Morse code into file we opened for statistic purpose
                }
            }
            if (text[i]==' ')
            {
                printf(" "); //printing space
                fprintf(f," ");
            }
            if (i==strlen(text)-1)
                printf("\n"); // print new line
        }
        fprintf(f,"\n");
    }
    fclose(f); //close f file
}

/*this function is just combine of void morse() and char read_long_line() functions. Free array since it's dynamic array*/
void encode_text (Trnslt *s)
{
    char *text=read_long_line();    //initializing text to read_long_line function
    morse(text,s);                  //calling morse function
    free(text);                     //free dynamic array after use

}

// this function inserts nodes to the binary tree. Left node is . and right one is -
btree *insert (btree *root,char c,char *s)
{
    if (root==NULL)
    {
        root=(btree*)malloc(sizeof(btree)); //making new node in the tree
        root->letter=' '; //making root of the tree
        root->left=NULL;
        root->right=NULL;
    }
    if (s[0]!='\0')
    {
        if (s[0]=='-')
            root->right=insert(root->right,c,(s+1)); //moving through tree by right nodes
        if (s[0]=='.')
            root->left=insert(root->left,c,(s+1)); //moving through tree by left nodes
    }
    else if (s[0]=='\0')
        root->letter=c; //placing letter on the node
    return root;
}

// this function will insert all my alphabet which is in array of Trnslt.
btree *Allinsert (btree *root,Trnslt *s)
{
    for (int i=0; i!=50; ++i) //looping through translate table
    {
        root=insert(root,s[i].letter,s[i].morse);   //initializing letter into node
    }
    return root;
}
//this function searches morse code by binary tree and prints corresponding letter for that morse code on the screen and into the file.
void search (btree *root,char *s)
{
    int i=0; // counter of Morse code: dots and dashes
    btree *tmp=root; // using temporary binary tree since we don't want to loose original one
    FILE *f; // initialize f file
    f=fopen("decode.txt","a"); // open f file
    if (s!=NULL)
    {
        while (s[i]!='\0') // moving through string when it is not equal to terminator
        {
            if (s[i]=='.' || s[i]=='-') //checking if its morse code
            {
                while ( s[i]=='-' || s[i]=='.') // going through binary tree
                {
                    if (s[i]=='.') // left node of tree
                        tmp=tmp->left;
                    else if (s[i]=='-')
                        tmp=tmp->right; // right node of tree
                    ++i;
                }
                printf("%c",tmp->letter); //printing a letter on the node
                fprintf(f,"%c",tmp->letter);
            }
            if (s[i]==' ' && s[i+1]==' ') // double space
            {
                printf(" "); // double space=white space
                fprintf(f," ");
            }
            if (i==strlen(s))
                printf("\n"); // printing new line for better look
            ++i;
            tmp=root; // initializing to the original root pointer
        }
        fprintf(f,"\n");
    }
    fclose(f); //closing file
}

/* this function combines reading function [read_long_line], printing text into
file function [printtxt], inserting function [Allinsert] and printing decode [search] */
void decode (Trnslt *s)
{
    btree *root=NULL;
    char *m=read_long_line(); //reading a long string from user
    root=Allinsert(root,s); //initializing root to allinsert function
    search(root,m);  //calling search function
    free(m);  //free dynamic array
    free(root);

}

// function will give statistics of letters
void statisticDecd (Trnslt *s)
{
    FILE *f;
    char c;
    f=fopen("decode.txt","r");
    if (f!=NULL)
    {
        while (fscanf(f,"%c",&c)!=EOF) // doing following steps until end of file
        {
            for (int i=0; i!=50; ++i) // going through all array of structures
                if (s[i].letter==c)
                    s[i].l+=1;
        }
        for (int j=0; j!=50; ++j)
            if (s[j].l!=0)// only will print statistics of used letters
            {
                printf("\t'%c' letter is used %d times\n",s[j].letter,s[j].l);
            }
    }
    fclose(f); // close f file to prevent memory leak
    remove("decode.txt"); // deletes “decode.txt” text file to start again from new file next time
}

// function will give statistics of dots and dashes. This function is similar to the above one
void statisticEncd()
{
    FILE *f;
    char c;
    int dot=0,dash=0;
    f=fopen("encode.txt","r");
    if (f!=NULL)
    {
        while (fscanf(f,"%c",&c)!=EOF)
        {
            if (c=='.')
                ++dot;
            else if (c=='-')
                ++dash;
        }
        if (dot!=0)//only will print statistics of dashes and dots, if they are used
        {
            printf("\tDot is used %d times\n",dot);
        }
        if (dash!=0)
        {
            printf("\tDash is used %d times\n",dash);
        }
    }
    fclose(f);
    remove("encode.txt");
}

/*In this function the 1st input of the user is encoded and then the 2 input of the user is compared to the encoded
string, if it is true then appropriate message is printed*/
void playGame (Trnslt *s)
{
    fflush(stdin); //clearing input buffer
    char newl;
    char encodedl[7];
    char *userEncodedL;
    int answer;
    printf("Please enter your letter:\n"); //asking user to enter the letter
    scanf("%c", &newl); //reading input
    for (int i=0; i!=50; ++i) //going through array of structures
    {
        if (s[i].letter==newl)
        {
            strcpy(encodedl, s[i].morse); //looking for needed character in translate table and copying it into string
        }
    }

    fflush(stdin); //clearing input buffer
    printf("Please write your answer: ");//asking user to enter the encoded version of his 1st input
    userEncodedL=read_long_line();//reading user's input
    answer=strcmp(userEncodedL,encodedl); /*comparing the string with encoded letter made
                                                         by program with the user's 2nd input*/
    if (answer==0)   //if they are the same printing message below
        printf("Bravo! You are correct!\n");
    else
        printf("Input ERROR!\n");
}
/*This function converts text file into string and returns string. It is similar to read function, used above */
char *read_from_file (char *filename)
{
    FILE *f;
    f=fopen(filename,"r");
    if (f==NULL) //checking if file exist
    {
        printf("ERROR! File does not exist!");
        return;
    }
    else
    {
        int cnt = 0;
        char *line = (char*) malloc(sizeof(char) * 1); // initializing dynamic array
        line[0] = '\0'; //terminating zero
        char newchar;
        while (fscanf(f,"%c", &newchar) == 1 && newchar!=EOF) //reading the file character by character until end of file
        {
            char *newarr = (char*) malloc(sizeof(char) * (cnt+1+1)); //extending the array
            for (int i = 0; i < cnt; ++i)
                newarr[i] = line[i];  //copying an old array into extended one
            newarr[cnt] = newchar;   //intializing new char in new array
            newarr[cnt+1] = '\0';
            free(line);
            line = newarr; //redirecting line pointer to a new extended array
            ++cnt; //counter increments
        }

        return line;  //returning new extended array
    }
    fclose(f);
}
/*This function takes a translate pointer and 2 strings: the one that we want to encode and a string which
contains the name of file we want to save. As a result we get a file with encoded text.
 This functionis similar to morse function */
void morse_file (char *text,Trnslt *s, char* saving_file)
{
    FILE *f;
    f=fopen(saving_file,"a");
    if (text!=NULL)
    {
        int i,l=strlen(text);
        for (i=0; i!=l; ++i)
        {
            for (int j=0; j!=50; ++j)
            {
                if (tolower(text[i])==s[j].letter)
                {
                    fprintf(f,"%s ",s[j].morse);
                }
            }
            if (text[i]==' ')
            {
                fprintf(f," ");
            }
        }
        fprintf(f,"\n");
    }
    else
    {
        printf("Can not create file!");
        return;
    }
    fclose(f);
}
/* combination of 3 functions: the one which reads long line,
reads from file and  morse file func which saves the encoded file
This function is similar to code function*/
void encode_file (Trnslt *s)
{
    fflush(stdin); //clearing input buffer
    printf("Please enter file name to encode: ");
    char *filename=read_long_line(); //reading the input filename
    char *text=read_from_file(filename); //copying the text read from file into string
    printf("\nPlease enter file name to save: ");
    char *save=read_long_line(); //reading the output filename
    morse_file(text,s,save); //encoding the string and saving it in output file
    free(text);              //we free everything because of using the dynamic memory allocation
    free(filename);
    free(save);
}
/*this function searches letters 1 by 1 from binary tree of morse code,
and prints it into file that user want to save it in.
This function is similar to search function*/
void search_file (btree *root,char *s, char *saving_file)
{
    int i=0;
    btree *tmp=root;
    FILE *f; //defines file f
    f=fopen(saving_file,"a"); //opens file f and append into it
    if (s!=NULL) //when file file exists
    {
        while (s[i]!='\0')    //this part of function is similar to the search function that you can find above
        {
            if (s[i]=='.' || s[i]=='-')
            {
                while ( s[i]=='-' || s[i]=='.')
                {
                    if (s[i]=='.')
                        tmp=tmp->left;
                    else if (s[i]=='-')
                        tmp=tmp->right;
                    ++i;
                }
                fprintf(f,"%c",tmp->letter);
            }
            if (s[i]==' ' && s[i+1]==' ')
            {
                fprintf(f," ");
            }
            ++i;
            tmp=root;
        }
        fprintf(f,"\n");
    }
    fclose(f); //closing file for safety
}
/*This function takes a translate pointer and combines several functions.
As a result we get a file with decoded text named by user. This function
is similar to decode function*/
void decode_file (Trnslt *s)
{
    fflush(stdin); //clearing input buffer
    btree *root=NULL;
    printf("Please enter file name to decode: "); //asking user to enter the name of the file which need to be decoded
    char *filename=read_long_line(); //reading the input filename
    char *text=read_from_file(filename); //reading and copying the text from the file to the string
    root=Allinsert(root,s);
    printf("\nPlease enter file name to save: ");//asking user for the name of output file
    char *save=read_long_line(); //reading the output filename
    search_file(root,text,save); //searching for the needed character in binary tree and saving in output file
    free(text); // due to using of dynamic array we need to free it
    free(save);
    free(filename);
    free(root);
}
