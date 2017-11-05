//
// main.cpp - caller for frog-class
//
#include <list>
#include <vector>
using namespace std;

#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "frog.h"



int main(int argc, char **argv)
{

char frogfilename[512];

strcpy(frogfilename,"hello.frog");
if (argc >= 2)
   {
   if ( strlen(argv[1]) > 0 )
      {
      strcpy(frogfilename,argv[1]);
      }
   }   

//
// Const buffer sizes - increase it, if you want
//
long MAXSIZE = 200000;
char code[MAXSIZE];
char json_input[64000];
char json_output[64000];



strcpy(json_input,"");


// Preset JSON for hello.frog - file
if ( strcmp(frogfilename,"hello.frog") == 0)
{

strcpy(json_input,"\n\
{                                 \n\
\"a\":\"40\",                      \n\
\"buffer\" : \"Hallo Xrog!\"                      \n\
}                                 \n\
");

} // if "hello.frog"


FILE *fp = fopen(frogfilename, "rb");

long len = -1; 
int debug = 0;
len = fread(code, 1,MAXSIZE, fp);
fclose(fp);

printf("JSON-input:\n");
printf("%s\n", json_input);
printf("\n");

//    
// run frog program
//
frog frog_instance;
frog_instance.run( code, json_input, json_output, debug );

printf("JSON-output:\n");
printf("%s\n", json_output);
 

return(1);
} // main