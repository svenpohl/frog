//
//
// Frog - A simplified programming language.
// 
// Input:  A ".frog"-file, variables as values in JSON.
// Output: variables in JSON.
// 
// V1.0 - 05.nov.2017 - sven.pohl@zen-systems.de
//
//

#include <list>
#include <vector>
using namespace std;
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "frog.h"


//
//
// Contructor of frog.
//
//
frog::frog()
{
code_ptr = 0;
strcpy(statement,"");
} // Constructor





//
//
// assign_braces()
//
//
int frog::assign_braces()
{
int READY = 0;
 

/*
Pseudocode:

1) Search a brace without twin
   found     -> ready.
   not found -> goto 2)
2) Look for the next closing brace without twin-brace.
   found     -> assign it, increment element-pointer++;
   not found -> search another open brace   
3) Goto 1)      
*/

while (READY == 0)
      {      
      int size = code_vector.size();      
      int FOUND = 0;
      
      for (int i=0; i<size; i++)
          {
          int size2 = code_vector[i].split_elements.size();
                    
          if (size2 >= 1)
             {
             if ( strcmp( code_vector[i].split_elements[0].value, "{") == 0 &&
                  code_vector[i].split_elements[0].twin_brace_index == -1)
                  {
                  FOUND = 1;
          
                  //
                  // Search other brace                  
                  //
                  int found_error = 0;

                  for (int i2=(i+1); i2<size; i2++)
                      {
                      if ( strcmp( code_vector[i2].split_elements[0].value, "}") == 0 &&
                           code_vector[i2].split_elements[0].twin_brace_index == -1)
                         {
                         // assign it                         
                         code_vector[i ].split_elements[0].twin_brace_index = i2;
                         code_vector[i2].split_elements[0].twin_brace_index = i;
                         i = 0; // reset
                         break;
                         } //
                      else
                      if ( strcmp( code_vector[i2].split_elements[0].value, "{") == 0  &&
                           code_vector[i2].split_elements[0].twin_brace_index == -1)
                         {                         
                         found_error = 1;
                         i = i2-1;                         
                         break;
                         } //
                         
                      } // for i2...
                  
                  }  // if...
             } // if (size2 >= 1)
          } // for i...
                              
      READY = 1;
      } // while READY == 0


return(1); 
} // assign_braces



//
//
// trim - Simple char-array 'trim' function. Removes leading und final spaces.
//
//
void frog::trim( char *s )
{
int l = strlen(s);
char buffer[255];

int i,i2;
int first = 0;
int second = 0;

if ( strlen(s) == 0) return;
   
while ( isspace( s[first] ) && first < l) first++;

second = l-1;
while ( isspace( s[second] ) && second > 0) second--;

i2 = 0;
for (i=first; i<=second; i++) 
    {
    buffer[i2] = s[i];
    i2++;
    }
buffer[i2] = '\0';

strcpy(s,buffer);
} //// trim



//
//
// strpos - Returns index of a search_string 'needle'.
// Returns: If found : the position index, if not: -1.
//
//
int frog::strpos(char *haystack, char *needle)
{
char *p = strstr(haystack, needle);
if (p) return p - haystack;
return -1;   // Not found = -1.
} // strpos



//
//
// parse_next_statement.
//
// 
int frog::parse_next_statement( char *code )
{
int ret = 1;
int i = 0;

while (
      code_ptr < code_length &&
      code[code_ptr] != '\r' &&
      code[code_ptr] != '\n' &&     
      code[code_ptr] != ';' 
      )
      {
      statement[i] = code[code_ptr];
      code_ptr++;
      i++;
      } 
           
statement[i] = '\0';      


while ( 
      (code_ptr < code_length) &&
      	(
      	code[code_ptr] == '\n' ||
      	code[code_ptr] == '\r' ||
      	code[code_ptr] == ' '  ||
      	code[code_ptr] == ';' 
      	)
      )
      {      
      code_ptr++;    
      }

//
// Remove comments.
//
int pos = strpos(statement,(char *)"//");
if (pos != -1) statement[pos] = '\0';

if (code_ptr >= code_length) ret = -1;

return(ret);
} // parse_next_statement
 




//
//
// jump_next_token
//
//
int frog::jump_next_token(char* buffer, char* token, char delimiter, int &i, int l)
{
int fertig = 0;

//
// Skip Spaces
//
while (buffer[i] == delimiter && i < l) i++;
            
//      
// Grab token      
//
int i2=0;      
while (buffer[i] != delimiter && i < l)
      { 
      token[i2] = buffer[i];
      i++;
      i2++;      
      }
token[i2] = '\0';
            
if (i >= l) fertig = 1;
return(fertig);      
} // jump_next_token




//
//
// is_number()
//
//
int frog::is_number( char ch )
{
int ret = 0;
if ( ch >= '0' && ch <= '9' ) ret = 1;
return(ret);
} // is_number


//
//
// is_number().
// returns 1, if numbers, returs 2 if even a '.' signals 'float'
//
//
int frog::is_number( char *buffer )
{
int ret = 1;
int l = strlen(buffer);

for (int i=0; i<l; i++)
    {
  
    if ( buffer[i] == '.' ) {
                            ret = 2;
                            }
    if ( !is_number(buffer[i]) && buffer[i] != '.')
       {
       ret = 0;
       break;
       }
    }    

return(ret);
} // is_number



//
//
// is_int().
// returns 1, if numbers, returns 2 if even a '.' signals a 'float'
//
//
int frog::is_int( char *buffer )
{
int ret = 1;
int l = strlen(buffer);

for (int i=0; i<l; i++)
    {     
    if ( !is_number(buffer[i]) )
       {
       ret = 0;
       break;
       }
    }    

return(ret);
} // is_number
 
 
 

//
//
// is_letter()
//
//
int frog::is_letter( char ch )
{
int ret = 0;
if ( 
   (ch >= 65 && ch <= 90 ) ||
   (ch >= 97 && ch <= 122) ||
   (ch == '_')             ||
   (ch == '-')             
   ) ret = 1;
return(ret);
} // is_letter 
 
 
 
 
 
//
//
// split_statement()
//
//
int frog::split_statement( char *statement, vector<element> &elements )
{
int SHOW = 0;
element tmp_element;
int ret = 1;
int l = strlen(statement);
int i;
int i2;
elements.clear();

i=0;
while (i < l)
{


if ( statement[i] == ' ' && i < l)
   {   
   if (SHOW) printf("[SPACE]\n");
   i++;   
   } // '['
else



if ( is_letter(statement[i]) )
   {
   strcpy(tmp_element.type,"string");
   
   i2 = 0;
   while ( 
         is_letter(statement[i]) ||
         is_number(statement[i]) ||
         statement[i] == '.'     ||
         statement[i] == ':'     
         )
         {
         tmp_element.value[i2] = statement[i];
         i2++;
         i++;
         }
   tmp_element.value[i2] = '\0';      
   
   if (SHOW) printf("Type: [%s] value:[%s] \n",tmp_element.type,tmp_element.value);

   tmp_element.while_index = -1; 
   elements.push_back( tmp_element );
   } // string
else


if ( statement[i] == '[' )
   {   
   strcpy(tmp_element.type,"brace_open");
   strcpy(tmp_element.value,"[");
   
   if (SHOW) printf("Type: [%s] value:[%s] \n",tmp_element.type,tmp_element.value);
   i++;   
   tmp_element.while_index = -1; 
   elements.push_back( tmp_element );
   } // '['
else


if ( statement[i] == ']' )
   {   
   strcpy(tmp_element.type,"brace_close");
   strcpy(tmp_element.value,"]");
   
   if (SHOW) printf("Type: [%s] value:[%s] \n",tmp_element.type,tmp_element.value);
   i++;   
   tmp_element.while_index = -1; 
   elements.push_back( tmp_element );
   } // '['
else


if ( statement[i] == '{' )
   {   
   strcpy(tmp_element.type,"open_square_bracket");
   strcpy(tmp_element.value,"{");
   tmp_element.twin_brace_index = -1;
   
      
   if (SHOW) printf("Type: [%s] value:[%s] \n",tmp_element.type,tmp_element.value);
   i++;   
   tmp_element.while_index = -1; 
   elements.push_back( tmp_element );
   } // '{'
else


if ( statement[i] == '}' )
   {   
   strcpy(tmp_element.type,"open_square_bracket");
   strcpy(tmp_element.value,"}");
   tmp_element.twin_brace_index = -1;
   
   if (SHOW) printf("Type: [%s] value:[%s] \n",tmp_element.type,tmp_element.value);
   i++;   
   tmp_element.while_index = -1; 
   elements.push_back( tmp_element );
   } // '}'
else



if ( statement[i] == '(' )
   {   
   strcpy(tmp_element.type,"open_bracket");
   strcpy(tmp_element.value,"(");
   
   if (SHOW) printf("Type: [%s] value:[%s] \n",tmp_element.type,tmp_element.value);
   i++;   
   tmp_element.while_index = -1; 
   elements.push_back( tmp_element );
   } // '('
else


if ( statement[i] == ')' )
   {   
   strcpy(tmp_element.type,"close_bracket");
   strcpy(tmp_element.value,")");
   
   if (SHOW) printf("Type: [%s] value:[%s] \n",tmp_element.type,tmp_element.value);
   i++;   
   tmp_element.while_index = -1; 
   elements.push_back( tmp_element );
   } // ')'
else


if ( 
   statement[i] == '+' || 
   statement[i] == '-' ||   
   statement[i] == '*' ||   
   statement[i] == '/'   
   )
   {
   if (SHOW) strcpy(tmp_element.type,"ARITHMETIC");
   
   i2 = 0;
   while (          
         statement[i] == '+' || 
         statement[i] == '-' ||   
         statement[i] == '*' ||   
         statement[i] == '/'   
         )
         {
         tmp_element.value[i2] = statement[i];
         i2++;
         i++;
         }
   tmp_element.value[i2] = '\0';      
   
   if (SHOW) printf("Type: [%s] value:[%s] \n",tmp_element.type,tmp_element.value);

   tmp_element.while_index = -1; 
   elements.push_back( tmp_element );
   } // ARITHMETIC
else



if ( 
   statement[i] == '=' || 
   statement[i] == '!' ||   
   statement[i] == '>' ||   
   statement[i] == '<'   
   )
   {
   if (SHOW) strcpy(tmp_element.type,"EQUAL");
   
   i2 = 0;
   while (          
         statement[i] == '=' || 
         statement[i] == '!' || 
         statement[i] == '>' ||   
         statement[i] == '<' 
         )
         {
         tmp_element.value[i2] = statement[i];
         i2++;
         i++;
         }
   tmp_element.value[i2] = '\0';      
   
   if (SHOW) printf("Type: [%s] value:[%s] \n",tmp_element.type,tmp_element.value);

   tmp_element.while_index = -1; 
   elements.push_back( tmp_element );
   } // EQUAL
else



if ( is_number(statement[i]) )
   {
   strcpy(tmp_element.type,"number");
   
   i2 = 0;
   while ( 
         is_number(statement[i]) ||
         statement[i] == '.'
         )
         {
         tmp_element.value[i2] = statement[i];
         i2++;
         i++;
         }
   tmp_element.value[i2] = '\0';      
   
   if (SHOW) printf("Type: [%s] value:[%s] \n",tmp_element.type,tmp_element.value);

   tmp_element.while_index = -1; 
   elements.push_back( tmp_element );
   } // number
else



if ( 
   statement[i] == '\"' 
   )
   {
   strcpy(tmp_element.type,"STRING_CONST");
   
   i2 = 0;
   i++;
   while (          
         statement[i] != '\"'  
         )
         {
         tmp_element.value[i2] = statement[i];
         i2++;
         i++;
         }
   tmp_element.value[i2] = '\0';      
   
   if (SHOW) printf("Type: [%s] value:[%s] \n",tmp_element.type,tmp_element.value);
   i++;
   tmp_element.while_index = -1; 
   elements.push_back( tmp_element );
   } // STRING_CONST



} // while (i < l)


if (SHOW) printf("elements.size: %ld \n", elements.size() );
if ( elements.size() == 0 ) ret = 0;

return(ret);
} // split_statement
 
 
 
 
//
//
// show_codeblock - Displays a codeblock in debug mode.
//
// 
int frog::show_codeblock( long code_index )
{
int offset_minus = 3;
int offset_plus  = 3;
printf("\n");

int max_code_index = code_vector.size();   


for (int i=code_index-offset_minus; i<=code_index+offset_plus; i++)
    {
    if (i > 0 && i < max_code_index)
       {
       printf("%04d ",i);
              
       if (i == code_index) printf("> "); else printf("| "); 
       
       int size2 = code_vector[i].split_elements.size();
       for (int i2=0; i2<size2; i2++)
           {
           printf("%s ", code_vector[i].split_elements[i2].value);
           }
           
       printf("\n");
       
       } // if (i > 0 && i < max_code_index)
    } // for

  
printf("\n");   
return(0);
} // show_codeblock
 
 
//
//
// get_label_index()
//
// 
int frog::get_label_index( char *label )
{
int ret = -1;

int size = labels.size();
for (int i=0; i<size; i++)
    {
    if ( strcmp( labels[i].name, label) == 0 )
       {
       ret = labels[i].index;
       break;
       }
    } // size

return(ret);
} // get_label_index
 
 
//
//
// precompiled - dump and echo variabe.
// Do whatever you need for special functions: define it here! 
//
// 
int frog::precompiled( char *command, char *variable )
{
int  type;
int  length;
int  index;
char buffer[255];



#define TYPE_CHAR  0
#define TYPE_INT   1
#define TYPE_FLOAT 2

if (strcmp(command,"dump") == 0)
   {
   get_var_info( variable, type, length, index);
   
   if (type == TYPE_CHAR ) strcpy(buffer,"TYPE_CHAR");
   if (type == TYPE_INT  ) strcpy(buffer,"TYPE_INT");
   if (type == TYPE_FLOAT) strcpy(buffer,"TYPE_FLOAT");
   printf("-[DUMP]------------------------------------------------------\n");
   
   printf("DUMP: [%s] type:%s length:%d %d\n", (char *)variable,buffer,length,index);
   
   printf("    { ");
   for (int i=0; i<length; i++)
       {
       if (type == TYPE_CHAR)  printf( "%c|%d", variables[index].char_value [i], variables[index].char_value[i]) ;
       if (type == TYPE_INT  ) printf( "%d", variables[index].int_value  [i] );
       if (type == TYPE_FLOAT) printf( "%f" , variables[index].float_value[i] );

       if (i < (length-1)) printf(",");
       } // for i..length
   printf(" }\n");
   printf("-------------------------------------------------------------\n");

   } // dump


if (strcmp(command,"echo") == 0)
   {   
   int size = variables.size();
   for (int i=0; i<size; i++)
       {
       if ( strcmp(variables[i].name, variable ) == 0 )
          {
          
          if ( variables[i].type == TYPE_CHAR )
             {             
             printf("$> ");
             for (int i2=0; (i2<variables[i].length) && (variables[i].char_value[i2] != '\0'); i2++)
                 {                  
                 printf("%c",variables[i].char_value[i2] );
                 }
             printf("\n");             
             } // TYPE_CHAR       

          if ( variables[i].type == TYPE_INT )
             {
             printf("> ");
             for (int i2=0; i2<variables[i].length; i2++)
                 {
                 printf("%d ",variables[i].int_value[i2] );
                 }
             printf("\n");
             } // TYPE_INT

          if ( variables[i].type == TYPE_FLOAT )
             {
             printf("> ");
             for (int i2=0; i2<variables[i].length; i2++)
                 {
                 printf("%f ",variables[i].float_value[i2] );
                 }
             printf("\n");
             } // TYPE_FLOAT
             
          break;
          }
       } // for i...
   
   } // echo

return(1);
} // precompiled
 
 
 
//
//
// variabled_define_export
//
// 
int frog::variabled_define_export(int code_index)
{
int SHOW = 0;
if (SHOW) printf("EXPORT variabled_define_export \n");

int size = variables.size();
if (SHOW) printf("size: %d \n",size);

if ( 
   strcmp( code_vector[code_index].split_elements[0].value, "EXPORT" ) == 0 
   )
   { 
   
   if ( strcmp( code_vector[code_index].split_elements[1].value, "CLEAR" ) == 0 )
      {
      if (SHOW) printf("Export CLEAR...\n");
      for (int i=0; i<size; i++)
           {
           variables[i].export_var = 0;
           variables[i].export_order_index = 0;           
           } // for i...
      } // CLEAR
   else
   if ( strcmp( code_vector[code_index].split_elements[1].value, "ALL" ) == 0 )
      {
      if (SHOW) printf("Export ALL...\n");
      for (int i=0; i<size; i++)
           {
           variables[i].export_var = 1;
           } // for i...
      } // CLEAR
   else
      {
      if (SHOW) printf("Export %s...\n",code_vector[code_index].split_elements[1].value);
      
      //
      // get max index+1
      //
      int export_order_index = 0;
      for (int i=0; i<size; i++)
          {
          if (variables[i].export_order_index > export_order_index)
             {
             export_order_index = variables[i].export_order_index;
             }
          }
      export_order_index++;
      if (SHOW) printf(" *++*+*-> %d \n",export_order_index);
      
      
      for (int i=0; i<size; i++)
           {
           if ( strcmp(code_vector[code_index].split_elements[1].value, variables[i].name ) == 0 )
              {
              if (SHOW) printf("index: %d \n",i);
              variables[i].export_var = 1;
              variables[i].export_order_index = export_order_index;
              break;
              }
           } // for i...
      } // CLEAR
      
   } // EXPORT CLEAR





return(0);
} // variabled_define_export
 
 
//
//
// variables_out
//
//
int frog::variables_out( char *json_output )
{
int  SHOW = 0;
int  ret = 0;
char buffer[255];

int size = variables.size();


strcpy(json_output,"{ \n");

int min_index = 99999999;
int max_index = 0;

      for (int i=0; i<size; i++)
          {
          if (variables[i].export_order_index > max_index  )
             {    
             max_index = variables[i].export_order_index;
             }

          if (variables[i].export_order_index < min_index)
             {
             min_index = variables[i].export_order_index;
             }

          } // for i
     
      if (SHOW) printf(" -> min_index:%d , max_index:%d \n",min_index,max_index);



int cnt_to_export = 0;
int cnt_export = 0;

for (int i=0; i<size; i++)
    {
    if ( variables[i].export_var == 1 ) cnt_to_export++;
    }


for (int k=0; k<=max_index; k++)
{
for (int i=0; i<size; i++)
    {
    if (variables[i].export_var == 1 && variables[i].export_order_index == k )
       {
       sprintf(buffer,"\"%s\":",variables[i].name);
       strcat(json_output,buffer);

       if ( variables[i].type == TYPE_CHAR )
          {
          sprintf(buffer,"\"");
          strcat(json_output,buffer);
          
          for (int i2=0; i2<variables[i].length; i2++)
              {
              if (variables[i].char_value[i2] != '\0')
                 {
                 sprintf(buffer,"%c",variables[i].char_value[i2]);
                 strcat(json_output,buffer);                 
                 } else break;
                   
              } // i2..          
         
          sprintf(buffer,"\"");
          strcat(json_output,buffer);                     
          } // TYPE_CHAR
       
       
       if ( variables[i].type == TYPE_INT )
          {
          sprintf(buffer,"\"");
          strcat(json_output,buffer);                     
          for (int i2=0; i2<variables[i].length; i2++)
              {
              sprintf(buffer,"%d",variables[i].int_value[i2]);
              strcat(json_output,buffer);                     
              
              if (i2 < (variables[i].length-1)) 
                 {
                 strcat(json_output," ");                     
                 }
              }
          sprintf(buffer,"\"");
          strcat(json_output,buffer);                     
          } // TYPE_INT


       if ( variables[i].type == TYPE_FLOAT )
          {
          sprintf(buffer,"\"");
          strcat(json_output,buffer);                     
          for (int i2=0; i2<variables[i].length; i2++)
              {
              sprintf(buffer,"%f",variables[i].float_value[i2]);
              strcat(json_output,buffer);                     
  
              if (i2 < (variables[i].length-1)) 
                 {
                 strcat(json_output," ");                     
                 }
              } // for..
            sprintf(buffer,"\"");
            strcat(json_output,buffer);                     
            } // TYPE_FLOAT
          
       cnt_export++;   
          
       if (cnt_export < (cnt_to_export))
          {
          sprintf(buffer,",\n");
          strcat(json_output,buffer);                     
          } else
            {
            sprintf(buffer,"\n");
            strcat(json_output,buffer);                     
            }
                            
       
       } // if

    } // for i...

} // for k...

strcat(json_output,"}\n");


return(ret);
} // variables_out()




//
//
// variables_in
//
//
int frog::variables_in( char *json_input )
{
int SHOW = 0;
int ret = 0;
if (SHOW) printf("IN variables_in()\n");
 
long length = 0;
int  PHASE = 0;
int  i;
default_value *default_value_tmp;

if (SHOW) printf("load_input...\n");
if (SHOW) printf("json_input [%s]\n", json_input);

//
// Clear default_values
//
long size = default_values.size();
for (long i=0; i<size; i++)
    {
    free(default_values[i].value);
    } // for i..size
default_values.clear();
    
 



length = strlen(json_input);

if (SHOW) printf("l: %ld \n",length);

long cur_pos = 0;

while (cur_pos < length)
      {
      
      if (PHASE == 0)
         {
         if (json_input[cur_pos] == '\"') 
            {
            PHASE = 1;
            i = 0; 
            
            default_value_tmp = new default_value();
            
            //
            // Get the name
            //
            cur_pos++;
            while ( json_input[cur_pos] != '\"' )
                  {
                           
                     {
                     if (SHOW) printf("%c-%d|", json_input[cur_pos],PHASE);
                     default_value_tmp->name[i] = json_input[cur_pos];                     
                     } // if
                  
                  i++;
                  cur_pos++;   
                  }  // if
            cur_pos++;   
            
            
            default_value_tmp->name[i] = '\0';
     
            // 
            // Jump to next '"'
            // Then cur_pos points to the first Char of the value.
            //
            
            while ( json_input[cur_pos] != '\"' )
                  {
                  cur_pos++;
                  if (SHOW) printf("cp %ld -%c\n", cur_pos,json_input[cur_pos]);
                  } 
            

            cur_pos++;
            if (SHOW) printf("cp2 %ld\n", cur_pos);
            if (SHOW) printf("after2 (%c)- PHASE%d \n", json_input[cur_pos],PHASE);



         
            //
            // Get bufferlength
            //
            long tmp_pos = cur_pos;
            default_value_tmp->length = 0;
            while ( json_input[cur_pos] != '\"' )
                  {
                  default_value_tmp->length++;
                  cur_pos++;
                  } 

            //
            // Allocate Memory.
            //
            if (SHOW) printf("---> L: %ld\n",default_value_tmp->length);            
            default_value_tmp->value = (char *)malloc( default_value_tmp->length+2 );

            cur_pos = tmp_pos;
            long i=0;
            while ( json_input[cur_pos] != '\"' )
                  {
                  default_value_tmp->value[i] = json_input[cur_pos];
                  cur_pos++;
                  i++;
                  } 
            
            default_value_tmp->value[cur_pos] = '\n';
            
            
            if (SHOW) printf("Value geladen: [%s]\n",default_value_tmp->value);  
            
            default_values.push_back( *default_value_tmp );    
            
            if (SHOW) printf("after3 (%c)-%d \n", json_input[cur_pos],PHASE);
            cur_pos++;
            PHASE = 0;       
            } // if first '"'
                
         } // if (PHASE == 0)
        
      
      cur_pos++;
      } // while (cur_pos < length) 

size = default_values.size();
if (SHOW) printf("size vektor: %ld \n",size);

 



 

//
// Step: Preset variables with the json_input 
//

size = variables.size();
if (SHOW) printf("size-variables: %ld \n",size);

if (SHOW) printf("\n");

for (int i=0; i<size; i++)   
    {
    if (SHOW) printf("i:%d - %s \n ",i,variables[i].name);
    
    int size2 = default_values.size();
    for (int i2=0; i2<size2; i2++)
        {        
        if (SHOW) printf("T: %d  n:[%s]\n",i2, default_values[i2].name );
        if (
           strcmp( variables[i].name, default_values[i2].name ) == 0
           )
           {
           if (SHOW) printf("--> FOUND %s! type:%d  \n", variables[i].name, variables[i].type);
           
           if (variables[i].type == TYPE_CHAR)
              {
              if (SHOW) printf("TYPE_CHAR\n");
              
              int min_length = variables[i].length;
              if ( default_values[i2].length < min_length )
                 {
                 min_length = default_values[i2].length;
                 } 
                 
              if (SHOW) printf("min_length: %d defaultL:%ld variL:%ld \n",min_length,default_values[i2].length,variables[i].length );   

              int i3 = 0;
              for ( i3=0; i3<min_length; i3++ )
                  {
                  variables[i].char_value[i3] = default_values[i2].value[i3];
                  } // for i3...

              variables[i].char_value[i3] = '\0';                                                              
              } // TYPE_CHAR


           if (variables[i].type == TYPE_INT)
              {
              if (SHOW) printf("TYPE_INT\n");
   
               if (SHOW) printf("--- INT---\n");
     
               char token[255];
               char delimiter = ' ';
               int fertig = 0;
               int l = strlen(default_values[i2].value);
               if (SHOW) printf(">>> l: %d \n",l);

               int k = 0;
               int index = 0;
               while (fertig == 0)
                     { 
                     fertig = jump_next_token(default_values[i2].value,token,delimiter,k,l);                   
                  
                     long value = -1;
                     sscanf( token,"%ld",&value);
                     variables[i].int_value[index] = value;
                     if (SHOW) printf(">>> INT VALUE: %d \n",variables[i].int_value[index]);
                     index++;
                     } // while (fertig == 0)
       
              } // TYPE_INT


           if (variables[i].type == TYPE_FLOAT)
              {
              if (SHOW) printf("TYPE_FLOAT\n");
              
              if (SHOW) printf("--- FLOAT ---\n");
     
               char token[255];
               char delimiter = ' ';
               int fertig = 0;
               int l = strlen(default_values[i2].value);
               if (SHOW) printf(">>> l: %d \n",l);

               int k = 0;
               int index = 0;
               while (fertig == 0)
                     { 
                     fertig = jump_next_token(default_values[i2].value,token,delimiter,k,l);
                  
                     double value = -1;

                     value = atof(token);                     
                     variables[i].float_value[index] = (double)value;

                     index++;
                     } // while (fertig == 0)
                     
              } // TYPE_FLOAT

           
           } // if strcmp(name,name) == 0...
           
        } // for i2...

    
    } // for i..
 


//
// Clear default_values
//
size = default_values.size();
for (int i=0; i<size; i++)
    {
    free(default_values[i].value);
    } // for i..size
default_values.clear();


return(ret);
} // variables_in()



//
//
// get_var_info
//
//
int frog::get_var_info( char *name, int &type, int &length, int &index)
{
int size = variables.size();

type   = -1;
length = 0;
index  = -1;

for (int i=0; i<size; i++)
    {
    if ( strcmp( variables[i].name, name ) == 0 )
       {
       type   = variables[i].type;
       length = variables[i].length;
       index  = i;
       break;
       }
    } // for i..size


if (length > 0) return( 1 ); else 
                return( 0 );
} // get_var_info
 
 

//
//
// get_var_info
//
//
int frog::get_var_value( int code_index, int element_index, int &element_index_next, int &variable_index, int &variable_array_index, int &type, int &value_int, unsigned char &value_char, double &value_float, char *value_string )
{
int SHOW = 0;
int ret = 0;
int local_type;
int length;
int index;

type = -1;
value_int  = 0;
value_char = '\0';
value_float = 0;
strcpy(value_string,"");
variable_index = -1;
variable_array_index = -1;

if (SHOW) printf("type_split_element: %s \n",code_vector[code_index].split_elements[element_index].type);
get_var_info( code_vector[code_index].split_elements[element_index].value, local_type, length, index);    
if (SHOW) printf("A [%s] local_type:%d length:%d index:%d \n",code_vector[code_index].split_elements[element_index].value,local_type,length,index);

// ...A String is a string, even if only numbers included.
if (strcmp(code_vector[code_index].split_elements[element_index].type,"STRING_CONST") == 0)
   {
   type = TYPE_STRING;
   strcpy(value_string,code_vector[code_index].split_elements[element_index].value);
   return 1;
   }

if (local_type < 0)
   {
   
   if (SHOW) printf("[%s] ist keine Variable\n", code_vector[code_index].split_elements[element_index].value);
   int number = is_number(code_vector[code_index].split_elements[element_index].value); 

   int _is_int = is_int(code_vector[code_index].split_elements[element_index].value); 

   if ( _is_int )
      {
      type = TYPE_INT;
      value_int = atof(code_vector[code_index].split_elements[element_index].value);
      element_index_next = element_index + 1;
      
      if (SHOW) printf("is_INT!%d - %d\n",number,value_int);      
      } else 


   if ( number )
      {
      type = TYPE_FLOAT;
      value_float = atof(code_vector[code_index].split_elements[element_index].value);
      element_index_next = element_index + 1;
      
      if (SHOW) printf("is_Number!%d - %g\n",number,value_float);      
      } else 
        {
        if (SHOW) printf("is NO number [%s]\n", code_vector[code_index].split_elements[element_index].type);
        type = TYPE_STRING;
        element_index_next = element_index + 1;
        strcpy(value_string, code_vector[code_index].split_elements[element_index].value);     
        }
        
   } // _is_int
   else
       {
       //
       // Keine Konstante sondern eine Variable
       //
        
       if (SHOW) printf("%s IST eine Variable! anzahl %ld\n",code_vector[code_index].split_elements[element_index].value, code_vector[code_index].split_elements.size() );
       get_var_info( code_vector[code_index].split_elements[element_index].value, local_type, length, index);    
       if (SHOW) printf("B local_type:%d length:%d index:%d \n",local_type,length,index);

       variable_index = index;

       //
       // Is an Array?
       //
       int array_index = 0;
       int local_type2 = -1;
       int index2 = 0;
       
       if (code_vector[code_index].split_elements.size() > 3)
          {
          if (SHOW) printf("Hole Array index...falls vorhanden [%s]\n",code_vector[code_index].split_elements[element_index+1].value);
          // ...is Array or operator following
          // if array - is the index const or a integer var?
          if ( strcmp( code_vector[code_index].split_elements[element_index+1].value,"[") == 0 )
             {             
             get_var_info( code_vector[code_index].split_elements[element_index+2].value, local_type2, length, index2);
             if (SHOW) printf("--- arrayindex- local_type2: %d index2: %d\n", local_type2, index2);
  
             if ( local_type2 == -1)
                {
                // const - directly get int value und assign zu index.
                array_index = (int)atof(code_vector[code_index].split_elements[element_index+2].value);
                if (SHOW) printf("arrayindex is CONSTANT(%s), value: %d \n", code_vector[code_index].split_elements[element_index+2].value, array_index );
                } else
                  {
                 
                  if (SHOW) printf("--- Array-VARIABLE named: %s \n",code_vector[code_index].split_elements[element_index+2].value);
                  array_index = variables[index2].int_value[0];
                  if (SHOW) printf("--- arrayindex ist VARIABLE(%s), wert(muss 1 sein!): %d \n", code_vector[code_index].split_elements[element_index+2].value, array_index );
                  }      

             element_index_next = element_index + 4;
             
             } // if strcmp..."["
             else
                {
                // Operator?
                if (SHOW) printf("Operator ??? %s\n",code_vector[code_index].split_elements[element_index+1].value);
                element_index_next = element_index + 1; 
                }
            
            
          } else
            {
            if (SHOW) printf("...is no array, variable can assigned directly\n");
            array_index = 0;
            element_index_next = element_index + 1;   
            } // if NO array

        if (SHOW) printf("+ + + local_type: %d variable index=%d - current array_index=%d \n",local_type, index, array_index);
        if ( local_type == TYPE_INT) 
           {
           value_int = variables[index].int_value[array_index];
           
           if (SHOW) printf("Is single integer - value_int:%d\n",value_int);
           }         

        if ( local_type == TYPE_CHAR) 
           {
           value_char = variables[index].char_value[array_index];
           
           if (SHOW) printf("* Is single CHAR value_char:%c\n",value_char);
           }         

        if ( local_type == TYPE_FLOAT) 
           {
           value_float = variables[index].float_value[array_index];
            
           if (SHOW) printf("Is single FLOAT value_float:%f\n",value_float);
           }   
       variable_array_index = array_index;
       type                 = local_type;       
       } // is variable

 
 
return(ret);
} // get_var_value
 
 
 
 
 
//
//
// assignment
//
//
int frog::assignment( long code_index )
{
int SHOW = 0;
if (SHOW) printf("ASSIGN: %ld ....\n",code_index);

int    value1_type  = -1;
int    value1_int   = 0;
unsigned char   value1_char  = 0;
double value1_float = 0;
char   value1_string[255];
int    value1_variable_index       = 0;
int    value1_variable_array_index = 0;



int    value2_type  = -1;
int    value2_int   = 0;
unsigned char   value2_char  = 0;
double value2_float = 0;
char   value2_string[255];
int    value2_variable_index       = 0;
int    value2_variable_array_index = 0;

int    value3_type    = -1;
int    value3_int     = 0;
unsigned char   value3_char    = 0;
double value3_float   = 0;
char   value3_string[255];
int    value3_variable_index       = 0;
int    value3_variable_array_index = 0;

int    op             = 0;
int    pos_2_index    = 0;
int    pos_index_next = 0;



int split_elements_size = code_vector[code_index].split_elements.size();
if (SHOW) printf("+ split_elements_size: %d \n",split_elements_size);

//
// 1) Wert 2 bestimmen
//
pos_2_index = -1;
for (int i=0; i<split_elements_size; i++)
    {
    if ( strcmp(code_vector[code_index].split_elements[i].value,"=") == 0 ) 
       {
       pos_2_index = i+1;
       }
    
    } // for i...
if (SHOW) printf("--- pos_2_index position: %d \n ",pos_2_index);


if (split_elements_size <= 3)
   {
   if ( strcmp(code_vector[code_index].split_elements[1].value,"=") == 0 ) 
      {
      pos_2_index = 2;
      }
   } // if (split_elements_size <= 3)
   else
   if (split_elements_size > 3)
      {
      if ( strcmp(code_vector[code_index].split_elements[4].value,"=") == 0 ) 
         {
         pos_2_index = 5;
         }
      } 

if (SHOW) printf("pos_2_index: %d - [%s] \n",pos_2_index,code_vector[code_index].split_elements[pos_2_index].value);  


get_var_value( code_index, pos_2_index, pos_index_next, value2_variable_index, value2_variable_array_index, value2_type, value2_int, value2_char, value2_float, (char*)value2_string );

if (SHOW) 
   {
   printf("get_var_value returns: type:%d \n", value2_type);
   printf("              returns: value2_int:%d \n", value2_int);
   printf("              returns: value2_char:%c \n", value2_char);
   printf("              returns: value2_float:%g \n", value2_float);
   printf("              returns: value2_string:%s \n", value2_string);
   printf("get_var_value returns: pos_index_next:%d \n", pos_index_next);
   printf("get_var_value returns: value2_variable_index:%d \n", value2_variable_index);
   printf("get_var_value returns: value2_variable_array_index:%d \n", value2_variable_array_index);
   }
   
//   
// END - get param-2 value
//


//
// 2) Zeichen nach 2. Ausdruck
//
if (SHOW) printf("--- 2---- Zeichen nach 2. Ausdruck \n");
op = -1;
if ( pos_index_next >= split_elements_size )
   {
   if (SHOW) printf("Ausdruck zuende\n");
   } else
     {    
     if ( code_vector[code_index].split_elements[pos_index_next].value[0] == '+' ) op = OPERATOR_ADD;
     if ( code_vector[code_index].split_elements[pos_index_next].value[0] == '-' ) op = OPERATOR_SUB;
     if ( code_vector[code_index].split_elements[pos_index_next].value[0] == '*' ) op = OPERATOR_MUL;
     if ( code_vector[code_index].split_elements[pos_index_next].value[0] == '/' ) op = OPERATOR_DIV;
     if (SHOW) printf("following: [%s] op:%d \n",code_vector[code_index].split_elements[pos_index_next].value,op);
     pos_index_next++;
     }

//
// 3) ... do something with value3
//
if (op > -1)
   {
   if (SHOW) printf("--- 3---- Informationen ueber dritte Variable 3 holen \n");
   
 
   get_var_value( code_index, pos_index_next, pos_index_next, value3_variable_index, value3_variable_array_index, value3_type, value3_int, value3_char, value3_float, (char*)value3_string );

   if (SHOW) 
   {
   printf("get_var_value returns: type:%d \n", value3_type);
   printf("              returns: value3_int:%d \n", value3_int);
   printf("              returns: value3_char:%c \n", value3_char);
   printf("              returns: value3_float:%g \n", value3_float);
   printf("              returns: value3_string:%s \n", value3_string);
   printf("get_var_value returns: pos_index_next:%d \n", pos_index_next);
   printf("get_var_value returns: value3_variable_index:%d \n", value3_variable_index);
   printf("get_var_value returns: value3_variable_array_index:%d \n", value3_variable_array_index);
   }

 
   } // if (op > -1)

//
// 4) Informationen ueber linke zuzuweisende Variable 1 holen
//
if (SHOW) printf("--- 4---- Informationen ueber linke zuzuweisende Variable 1 holen \n");


get_var_value( code_index, 0, pos_index_next, value1_variable_index, value1_variable_array_index, value1_type, value1_int, value1_char, value1_float, (char*)value1_string );

if (SHOW) 
   {
   printf("FIRST ELEMENT: [%s]\n", code_vector[code_index].split_elements[0].value);
   printf("get_var_value 1 returns: value1_type:%d \n", value1_type);
   printf("              1 returns: value1_int:%d \n", value1_int);
   printf("              1 returns: value1_char:%c \n", value1_char);
   printf("              1 returns: value1_float:%g \n", value1_float);
   printf("              1 returns: value1_string:%s \n", value1_string);
   printf("get_var_value 1 returns: pos_index_next:%d \n", pos_index_next);
   printf("get_var_value 1 returns: value1_variable_index:%d \n", value1_variable_index);
   printf("get_var_value 1 returns: value1_variable_array_index:%d \n", value1_variable_array_index);
   }

 

//
// BEGIN assignment
//
if (SHOW) printf("--- 5 --- Assignment \n");



if (SHOW) printf("Operator: [%d]\n",op);


if ( value1_type == TYPE_CHAR )
   {
   unsigned char tmp_char;

   
   // Is it a buffer?
   int l = strlen(value2_string);
   if (SHOW) printf("Length buffer: %d\n",l);

   if (SHOW) printf("type CHAR (%d) (%d)\n",value2_variable_index,value2_variable_array_index);
   if ( value2_type == TYPE_CHAR   ) tmp_char = value2_char;
   if ( value2_type == TYPE_STRING ) tmp_char = value2_string[0];
   if ( value2_type == TYPE_INT    ) tmp_char = value2_int;
   if ( value2_type == TYPE_FLOAT  ) tmp_char = (char)value2_float;
      
   
   if ( l > 1)
      {
      int max_len = l;
      if (SHOW) printf("Long buffer (%s)\n",value2_string);
      int l2 = strlen( (char*)value2_string );
      if (l2 < max_len) max_len = l2;
      
      int i=0;
      for (i=0; i<max_len; i++)
          {          
          variables[value1_variable_index].char_value[i] = value2_string[i];
          }

      variables[value1_variable_index].char_value[i] = '\0';

      if (SHOW) printf(">>> STRING value new: %s \n", variables[value1_variable_index].char_value );
      } // if ( l > 1)
      else
          {
          //
          // Op
          //
          if (op != -1)
             {
             double   tmp_char3;
             if ( value3_type == TYPE_CHAR   ) tmp_char3 = value3_char;
             if ( value3_type == TYPE_STRING ) tmp_char3 = (double)value3_string[0];
             if ( value3_type == TYPE_INT    ) tmp_char3 = (double)value3_int;
             if ( value3_type == TYPE_FLOAT  ) tmp_char3 = (double)value3_float;
             if (SHOW) printf("- tmp_char3: %f\n",tmp_char3);
             if (op == OPERATOR_ADD) tmp_char = tmp_char + tmp_char3;  
             if (op == OPERATOR_SUB) tmp_char = tmp_char - tmp_char3;  
             if (op == OPERATOR_DIV) tmp_char = tmp_char / tmp_char3;  
             if (op == OPERATOR_MUL) tmp_char = tmp_char * tmp_char3;  
             
             if (SHOW) printf("- tmp_char: %d - %d\n",tmp_char,tmp_char);
             } // if (op != -1)

          
          variables[value1_variable_index].char_value[value1_variable_array_index] = (unsigned char)tmp_char;
          }
   

   if (SHOW) printf(">>> CHAR value new: %c \n", variables[value1_variable_index].char_value[value1_variable_array_index]);
   } // TYPE_CHAR

if ( value1_type == TYPE_INT )
   {
   double tmp_float;
   if (SHOW) printf("type INT (%d) (%d)\n",value2_variable_index,value2_variable_array_index);
   if ( value2_type == TYPE_CHAR  ) tmp_float = value2_char;
   if ( value2_type == TYPE_STRING ) tmp_float = (double)value2_string[0];
   if ( value2_type == TYPE_INT   ) tmp_float = value2_int;
   if ( value2_type == TYPE_FLOAT ) tmp_float = value2_float;
   
   if (SHOW) printf("tmp_float: %g \n", tmp_float);
   
   
   //
   // Op
   //
   if (op != -1)
      {
      double tmp_float3;
      if ( value3_type == TYPE_CHAR   ) tmp_float3 = (double)value3_char;
      if ( value3_type == TYPE_STRING ) tmp_float3 = (double)value3_string[0];
      if ( value3_type == TYPE_INT    ) tmp_float3 = (double)value3_int;
      if ( value3_type == TYPE_FLOAT  ) tmp_float3 =         value3_float;
             
      if (op == OPERATOR_ADD) tmp_float = tmp_float + tmp_float3;  
      if (op == OPERATOR_SUB) tmp_float = tmp_float - tmp_float3;  
      if (op == OPERATOR_DIV) tmp_float = tmp_float / tmp_float3;  
      if (op == OPERATOR_MUL) tmp_float = tmp_float * tmp_float3;  
      } // if (op != -1)
                
 
   variables[value1_variable_index].int_value[value1_variable_array_index] = (int)tmp_float;
   if (SHOW) printf(">>> INT value new: %d \n", variables[value1_variable_index].int_value[value1_variable_array_index]);
   } // TYPE_INT

if ( value1_type == TYPE_FLOAT )
   {
   double tmp_float;
   if (SHOW) printf("type FLOAT (%d) (%d)\n",value2_variable_index,value2_variable_array_index);
   if ( value2_type == TYPE_CHAR  ) tmp_float = value2_char;
   if ( value2_type == TYPE_STRING ) tmp_float =  (double)value2_string[0];   
   if ( value2_type == TYPE_INT   ) tmp_float = value2_int;
   if ( value2_type == TYPE_FLOAT ) tmp_float = value2_float;
   
   if (SHOW) printf("tmp_float: %g \n", tmp_float);
   
   
   //
   // Op
   //
   if (op != -1)
      {
      double tmp_float3;
      if ( value3_type == TYPE_CHAR   ) tmp_float3 = (double)value3_char;
      if ( value3_type == TYPE_STRING ) tmp_float3 = (double)value3_string[0];
      if ( value3_type == TYPE_INT    ) tmp_float3 = (double)value3_int;
      if ( value3_type == TYPE_FLOAT  ) tmp_float3 =         value3_float;
             
      if (op == OPERATOR_ADD) tmp_float = tmp_float + tmp_float3;  
      if (op == OPERATOR_SUB) tmp_float = tmp_float - tmp_float3;  
      if (op == OPERATOR_DIV) tmp_float = tmp_float / tmp_float3;  
      if (op == OPERATOR_MUL) tmp_float = tmp_float * tmp_float3;  
      } // if (op != -1)
                
                   
   variables[value1_variable_index].float_value[value1_variable_array_index] = tmp_float;
   if (SHOW) printf(">>> FLOAT value new: %f \n", variables[value1_variable_index].float_value[value1_variable_array_index]);
   } // TYPE_CHAR


return 0;
} // assignment
 
 
 
 
 
//
//
//
// check_condition - Checks condition at current code_index, returns true if
//                   the condition matches.
//
//
int frog::check_condition( long code_index, int &new_code_index  )
{
int SHOW = 0;
int ret = -1;
int pos_1_index                 = -1;
int pos_index_next              = -1;
new_code_index = -1;

int value1_variable_index       = -1;
int value1_variable_array_index = -1;
int value1_type                 = -1;
int value1_int                  = -1;
unsigned char value1_char       = -1;
double value1_float             = -1;
char value1_string[255]; 

int value2_variable_index       = -1;
int value2_variable_array_index = -1;
int value2_type                 = -1;
int value2_int                  = -1;
unsigned char value2_char       = -1;
double value2_float             = -1;
char value2_string[255]; 

char statement[10];
char compare[10];
strcpy(statement,code_vector[code_index].split_elements[0].value);

if (SHOW) printf("-------------- CHECK CONDITION..(%s).\n",statement);


//
// bei While... Indexe eintragen
//
if ( strcmp( statement, "WHILE" ) == 0 )
   {
   if (SHOW) printf("WHILE.(current CI:%ld).. after:[%s] %s \n",code_index, code_vector[code_index+1].split_elements[0].value ,  code_vector[code_index+1].split_elements[1].value); 
   
   if ( strcmp( code_vector[code_index+1].split_elements[0].value,"{" ) == 0 )
      {
      int twin_brace_index = code_vector[code_index+1].split_elements[0].twin_brace_index;
      if (SHOW)  printf("Brace follows! (now: %ld, twin brace: %d)\n",code_index+1,twin_brace_index);

      code_vector[twin_brace_index].split_elements[0].while_index = code_index;
     
      //
      // The WHILE has to know, where it hast to jump if FALSE
      //      
      code_vector[code_index].split_elements[0].while_index_to_jump_if_false = twin_brace_index+1;

      } // if '{'
      else
         {
         if (SHOW) printf("No following brace!\n");
         code_vector[code_index+1].split_elements[0].while_index = code_index;

         //
         // The WHILE has to know, where it hast to jump if FALSE
         //
         code_vector[code_index].split_elements[0].while_index_to_jump_if_false = code_index+2;
         }
         
     if (SHOW) printf("while_index_to_jump_if_false: %ld\n",code_vector[code_index].split_elements[0].while_index_to_jump_if_false);    
         
   } // WHILE




//
// 1. Get variable info
//
pos_1_index = 1;
get_var_value( code_index, pos_1_index, pos_index_next, value1_variable_index, value1_variable_array_index, value1_type, value1_int, value1_char, value1_float, (char*)value1_string );







if (SHOW) 
{
printf("get_var_value returns: type:%d \n", value1_type);
printf("              returns: value1_int:%d \n", value1_int);
printf("              returns: value1_char:%c \n", value1_char);
printf("              returns: value1_float:%g \n", value1_float);
printf("              returns: value1_string:%s \n", value1_string);
printf("get_var_value returns: pos_index_next:%d \n", pos_index_next);
printf("get_var_value returns: value1_variable_index:%d \n", value1_variable_index);
printf("get_var_value returns: value1_variable_array_index:%d \n", value1_variable_array_index);
}
//...
if (SHOW) printf("NEXT-1: {%s} \n",code_vector[code_index].split_elements[pos_index_next].value);
strcpy(compare,code_vector[code_index].split_elements[pos_index_next].value);
pos_index_next++;
if (SHOW) printf("NEXT-2: {%s} \n",code_vector[code_index].split_elements[pos_index_next].value);


get_var_value( code_index, pos_index_next, pos_index_next, value2_variable_index, value2_variable_array_index, value2_type, value2_int, value2_char, value2_float, (char*)value2_string );

if (SHOW) 
{
printf("get_var_value returns: type:%d \n", value2_type);
printf("              returns: value2_int:%d \n", value2_int);
printf("              returns: value2_char:%c \n", value2_char);
printf("              returns: value2_float:%g \n", value2_float);
printf("              returns: value2_string:%s \n", value2_string);
printf("get_var_value returns: pos_index_next:%d \n", pos_index_next);
printf("get_var_value returns: value2_variable_index:%d \n", value2_variable_index);
printf("get_var_value returns: value2_variable_array_index:%d \n", value2_variable_array_index);
}

double compare1 = -1;
double compare2 = -2;


if ( value1_type == TYPE_CHAR   ) compare1 = (double) value1_char;
if ( value1_type == TYPE_STRING ) compare1 = (double) value1_string[0];
if ( value1_type == TYPE_INT    ) compare1 = (double) value1_int;
if ( value1_type == TYPE_FLOAT  ) compare1 = (double) value1_float;

if ( value2_type == TYPE_CHAR   ) compare2 = (double) value2_char;
if ( value2_type == TYPE_STRING ) compare2 = (double) value2_string[0];
if ( value2_type == TYPE_INT    ) compare2 = (double) value2_int;
if ( value2_type == TYPE_FLOAT  ) compare2 = (double) value2_float;

/*
==
!= 
>
<
>=
<=
*/
  
if (SHOW) printf("Compare %f %f \n",compare1,compare2);           
ret = 0;           
if ( strcmp( compare, "==" ) == 0 ) if (compare1 == compare2) ret = 1;          
if ( strcmp( compare, "!=" ) == 0 ) if (compare1 != compare2) ret = 1;          
if ( strcmp( compare, ">"  ) == 0 ) if (compare1 >  compare2) ret = 1;          
if ( strcmp( compare, "<"  ) == 0 ) if (compare1 <  compare2) ret = 1;          
if ( strcmp( compare, ">=" ) == 0 ) if (compare1 <= compare2) ret = 1;          
if ( strcmp( compare, "<=" ) == 0 ) if (compare1 >= compare2) ret = 1;          


if (SHOW) printf("RET compare: %d \n",ret);



//
// ret is now known, can make suggestion for new code_index 
//



if ( ret == 0 && strcmp( statement, "IF" ) == 0 )
   {
   if (SHOW) printf("IF-Condition is FALSE!\n");
   
   if ( strcmp( code_vector[code_index+1].split_elements[0].value,"{" ) == 0 )
      {
     
      int twin_brace_index = code_vector[code_index+1].split_elements[0].twin_brace_index;
      if (SHOW) printf("IF - Brace following! (now: %ld, twin_brace: %d)\n",code_index+1,twin_brace_index);
      
      
      new_code_index = twin_brace_index;
      } // if '{'
      else
         {
         if (SHOW) printf("IF no folloing brace!\n");
         new_code_index = code_index + 1;
         }
   
   } // IF
   else
      {
      if (SHOW) printf("Bedingung ist TRUE!\n");
      }


return(ret);
} // check_condition 
 
 
 
 
//
//
// jump_if_while
//
//
int frog::jump_if_while( int code_index )
{
int ret = -1;


        if ( code_vector[code_index].split_elements[0].while_index != -1 )
           {
           ret = code_vector[code_index].split_elements[0].while_index-1;
           }

return(ret);
} // jump_if_while()



 
//
//
// run
//
//
int frog::run( char* code, char* json_input, char* json_output, int debug )
{
int SHOW = 0;
vector<element> split_elements;

code_length = strlen(code);




PHASE = 0;


int ret = 1;
while ( ret >= 1)
{
ret = parse_next_statement( code );


if (SHOW) printf("ret:%d | - statement: %ld [%s]\n",ret,code_ptr,statement);

int ret2 = split_statement( statement, split_elements );
if (SHOW) printf("--- Size: %ld - ret2:%d\n", split_elements.size() , ret2);

if (ret2)
   {
   code_elements code_element;   
   code_element.split_elements = split_elements;   
   code_vector.push_back( code_element );
   }


} // while

int size = code_vector.size();
if (SHOW) printf("Code Vector size: %d \n",size);


assign_braces();


for (int i=0; i<size; i++)
    {
    int size2 = code_vector[i].split_elements.size();
    if (SHOW) printf("i: %d - size2: %d \n",i,size2);
            
        if (
           strcmp(code_vector[i].split_elements[0].value, "{" ) == 0 ||
           strcmp(code_vector[i].split_elements[0].value, "}" ) == 0 
           ) 
           {
           if (SHOW) printf("| [%s] index:%ld", code_vector[i].split_elements[0].value,  code_vector[i].split_elements[0].twin_brace_index );
           }
        
    if (SHOW) printf("\n");
    
    } // for i..size

if (SHOW) printf("-End - braces output -----<br>\n");

//
// Step: Select variables and allocate them to 'variables'
//


size = code_vector.size();
if (SHOW) printf("Code Vector size: %d \n",size);



for (int i=0; i<size; i++)
    {
    int size2 = code_vector[i].split_elements.size();
    if (SHOW) printf("\n\n-- i: %d - size2: %d \n",i,size2);


    //
    // char
    //
    if ( strcmp(code_vector[i].split_elements[0].value, "char" ) == 0 )
       {       
       variable *tmpvar;
       tmpvar = new variable();
       
       strcpy( tmpvar->name, code_vector[i].split_elements[1].value );
       tmpvar->type = TYPE_CHAR;
       
       //
       // Single Variable
       //
       if (size2 == 2)
          {          
          tmpvar->length = 1;
          } // if (size2 == 2)

       //
       // Array Variable
       //
       if (size2 == 5 && code_vector[i].split_elements[4].value[0] == ']' )
          {         
          int count;
          sscanf(code_vector[i].split_elements[3].value,"%d",&count);
          tmpvar->length = count;
          } // if (size2 == 2)
                           
       tmpvar->char_value = (unsigned char *)malloc( (tmpvar->length*sizeof(char)) +2 );
       
       variables.push_back( *tmpvar );      
       free( tmpvar );
       } // if 'char'
       
    //
    // int
    //
    if ( strcmp(code_vector[i].split_elements[0].value, "int" ) == 0 )
       {       
       variable *tmpvar;
       tmpvar = new variable();
       
       strcpy( tmpvar->name, code_vector[i].split_elements[1].value );
       tmpvar->type = TYPE_INT;
       
      
       
       //
       // Single Variable
       //
       if (size2 == 2)
          {          
          tmpvar->length = 1;
          } // if (size2 == 2)

       //
       // Array Variable
       //
       if (size2 == 5 && code_vector[i].split_elements[4].value[0] == ']' )
          {         
          int count;
          sscanf(code_vector[i].split_elements[3].value,"%d",&count);
          tmpvar->length = count;
          } // if (size2 == 2)
                           
       tmpvar->int_value = (int *)malloc( (tmpvar->length*sizeof(int)) +2 );
       
       variables.push_back( *tmpvar );      
       free( tmpvar );
       } // if 'int'
       
       
    //
    // float
    //
    if ( strcmp(code_vector[i].split_elements[0].value, "float" ) == 0 )
       {       
       variable *tmpvar;
       tmpvar = new variable();
       
       strcpy( tmpvar->name, code_vector[i].split_elements[1].value );
       tmpvar->type = TYPE_FLOAT;
       
       //
       // Single Variable
       //
       if (size2 == 2)
          {          
          tmpvar->length = 1;
          } // if (size2 == 2)

       //
       // Array Variable
       //
       if (size2 == 5 && code_vector[i].split_elements[4].value[0] == ']' )
          {         
          int count;
          sscanf(code_vector[i].split_elements[3].value,"%d",&count);
          tmpvar->length = count;
          } // if (size2 == 2)
                           
       tmpvar->float_value = (double *)malloc( (tmpvar->length*sizeof(double)) +2 );
       
       variables.push_back( *tmpvar );      
       free( tmpvar );
       } // if 'char'
       
    
    
    } // for i..size


//
// Zero all variables
//
size = variables.size();
for (int i=0; i<size; i++)
    {
    for (int i2=0; i2<variables[i].length; i2++)
        {
        if ( variables[i].type == TYPE_CHAR  ) variables[i].char_value  [i2] = ' ';
        if ( variables[i].type == TYPE_INT   ) variables[i].int_value   [i2] = 0;
        if ( variables[i].type == TYPE_FLOAT ) variables[i].float_value [i2] = 0;
        } // for i2
    } // for i...
 
 
// 
// Get variables preset values from input JSON
//
variables_in ( json_input );




//
// Step: get position of all labes in code_vector an save 
//       in labels-Vektor 
//
size = code_vector.size();
if (SHOW) printf("Code Vector size: %d \n",size);

for (int i=0; i<size; i++)
    {
    int size2 = code_vector[i].split_elements.size();
    
    if ( size2 == 1 )
       {
       int l = strlen(code_vector[i].split_elements[0].value);
       char last_ch =  code_vector[i].split_elements[0].value[l-1];
    
       if (last_ch == ':')
          {
          label tmp_label;
          strcpy(tmp_label.name, code_vector[i].split_elements[0].value);
          
          //
          // Remove ':'
          //        
          tmp_label.name[l-1] = 0;
          tmp_label.index = i;
          labels.push_back(tmp_label);

          } // if
          
       } // if ( size2 == 1 )

    
    } // for i..size


    
    
//
// Goto Main: 
// 




//
// Step 1: Start program execution at label 'MAIN:' 
//

int EXIT = 0;
int code_index = -1;
int save_index = -1;

size = labels.size();
if (SHOW) printf("size labels: %d \n",size);

for (int i=0; i<size; i++)
    {
    
    if (strcmp(labels[i].name,"MAIN") == 0)  
       {
       code_index = labels[i].index+1;
       break;
       } // if
       
    } // for i...


int max_code_index = code_vector.size();

if (debug)
   {
   show_codeblock( code_index );
   getchar();
   }
      
while (EXIT == 0)
      {
      int split_elements_size = code_vector[code_index].split_elements.size();
      
      //
      // Assignment
      //
      if ( 
             (strcmp( code_vector[code_index].split_elements[0].value,"WHILE" ) != 0) &&   
             ( 
              (strcmp( code_vector[code_index].split_elements[1].value,"=" ) == 0 && (split_elements_size > 2) ) ||
              (strcmp( code_vector[code_index].split_elements[4].value,"=" ) == 0 && (split_elements_size > 3) ) 
             ) 
         )
         { 
         if (SHOW) printf("Found Assignment 1:%s 4:%s \n",code_vector[code_index].split_elements[1].value,code_vector[code_index].split_elements[4].value);
         if (SHOW) printf(" %s | ", code_vector[code_index].split_elements[0].value);

         assignment( code_index );

          
         // 
         // Is there a while_index != -1? Jump to it now!
         //           
         int jump_index = jump_if_while( code_index );   
         if (jump_index > -1) code_index = jump_index;
         if (SHOW) printf("Assignement jump_index: %d\n",jump_index);
         } // Assignment
      else
          
      //
      // Precompiled
      //
      if (split_elements_size > 2 &&
          strcmp( code_vector[code_index].split_elements[0].value,"PRECOMPILED" ) == 0 
          )
          {                    
          precompiled( code_vector[code_index].split_elements[1].value, code_vector[code_index].split_elements[2].value );
          
          // 
          // Is there a while_index != -1? Jump to it now!
          //                     
          int jump_index = jump_if_while( code_index );   
          if (jump_index > -1) code_index = jump_index;
          if (SHOW) printf("Assignement jump_index: %d\n",jump_index);
          } // Precompiled          
      else

      //
      // EXPORT
      //
      if ( strcmp( code_vector[code_index].split_elements[0].value,"EXPORT" ) == 0 )
         { 
         if (SHOW) printf("EXPORT call..................%s.....\n",code_vector[code_index].split_elements[1].value);
         variabled_define_export(code_index);
         
         int jump_index = jump_if_while( code_index );   
         if (jump_index > -1) code_index = jump_index;
         if (SHOW) printf("Assignement jump_index: %d\n",jump_index);
         } // EXPORT         
      else

      //
      // OUT
      //
      if ( strcmp( code_vector[code_index].split_elements[0].value,"OUT" ) == 0 )
         {         
         variables_out( json_output);
         
         int jump_index = jump_if_while( code_index );   
         if (jump_index > -1) code_index = jump_index;
         if (SHOW) printf("Assignment jump_index: %d\n",jump_index);
         } // OUT
      else
      
      //
      // IN
      //
      if ( strcmp( code_vector[code_index].split_elements[0].value,"IN" ) == 0 )
         { 
         char json_buffer[2048];  
         variables_in( json_buffer);       
         if (SHOW) printf("json_buffer: [%s]\n", json_buffer);       
         
         int jump_index = jump_if_while( code_index );   
         if (jump_index > -1) code_index = jump_index;
         if (SHOW) printf("Assignment jump_index: %d\n",jump_index);  
         } // IN
      else                  

      //
      // GOTO
      //
      if ( strcmp( code_vector[code_index].split_elements[0].value,"GOTO" ) == 0 )
         { 
         int label_index = get_label_index( code_vector[code_index].split_elements[1].value );
         if (SHOW) printf("GOTO..%s...%d\n",code_vector[code_index].split_elements[1].value,label_index);
         
         save_index = code_index+1;
         code_index = label_index;
         } // GOTO
      else
      
      
      //
      // IF
      //
      if ( strcmp( code_vector[code_index].split_elements[0].value,"IF" ) == 0 )
         { 
         int new_code_index;       
         int ret = check_condition( code_index, new_code_index );
         if (SHOW) printf("COMPARE ret: %d - new_code_index:%d\n", ret, new_code_index);
           
         if (new_code_index != -1) 
            {
            code_index = new_code_index;  
            if (SHOW) printf("IF-jump to: %d\n", code_index);
            }        
         } // if 
      else   


      //
      // WHILE
      //
      if ( strcmp( code_vector[code_index].split_elements[0].value,"WHILE" ) == 0 )
         { 
         int new_code_index;
  
         if (SHOW) printf("WHILE........\n");
       
         ret = check_condition( code_index, new_code_index );
         if (SHOW) printf("COMPARE ret: %d - new_code_index:%d \n", ret, new_code_index );
         
         //
         // WHILE has to know where to jump, if condition not match
         //
         if (ret == 0)
            {
            code_index = code_vector[code_index].split_elements[0].while_index_to_jump_if_false-1;
            }
        
         } // if 
      else   


      //
      // '}'
      //
      if ( strcmp( code_vector[code_index].split_elements[0].value,"}" ) == 0 )
         { 
         // 
         // Is there a while_index != -1? Than jump!
         //            
         int jump_index = jump_if_while( code_index );   
         if (jump_index > -1) code_index = jump_index;
         if (SHOW) printf("} jump_index: %d\n",jump_index);
         } // '}'
      else


      //
      // RETURN
      //
      if ( strcmp( code_vector[code_index].split_elements[0].value,"RETURN" ) == 0 )
         { 
         code_index = save_index-1;
         } // RETURN
      else
          
      //
      // EXIT
      //
      if ( strcmp( code_vector[code_index].split_elements[0].value,"EXIT" ) == 0 )
         { 
         EXIT = 1;
         } // EXIT
      
     
  
      code_index++;
      if (code_index >= max_code_index) EXIT = 1;
            
      if (debug)
         {      
         show_codeblock( code_index );
         getchar();
         }


      } // while (EXIT == 0)
   


return(1);
} // run

