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


#define TYPE_CHAR   0
#define TYPE_INT    1
#define TYPE_FLOAT  2
#define TYPE_STRING 3

#define OPERATOR_ADD  0
#define OPERATOR_SUB  1
#define OPERATOR_DIV  2
#define OPERATOR_MUL  3

class default_value
{
public:
char name[50];
char *value;
long length;
}; // class default_value


class variable
{
public:
char   name[50];
int    export_var;
int    export_order_index;

int    type;
unsigned char   *char_value;
int             *int_value;
double          *float_value;
long   length;
}; // class variable


class element
{
public:
char type[50];
char value[256];
long twin_brace_index; 
long while_index;
long while_index_to_jump_if_false;
}; // class element


class label
{
public:
char name[50];
long index;
}; // class label


class code_elements
{
public:
vector <element> split_elements;
}; // class code_elements


class frog
{
private:
vector <default_value>  default_values;
vector <variable>       variables;

vector <label>          labels;
vector <code_elements>  code_vector;

long code_length;
long code_ptr;
char PHASE;
char statement[255];



public:
frog();

int  assign_braces();
void trim( char *s );
int  strpos(char *haystack, char *needle);
int  parse_next_statement( char *code );
int  jump_next_token(char* buffer, char* token, char delimiter, int &i, int l);

int  is_number( char ch );
int  is_number( char *buffer );
int  is_int   ( char *buffer );
int  is_letter( char ch );

int  split_statement( char *statement, vector<element> &elements );

int  show_codeblock( long code_index );
int  get_label_index( char *label );
int  precompiled( char *command, char *variable );
int  variabled_define_export(int code_index);
int  variables_in( char *json_input );
int  variables_out( char *json_output );
int  get_var_info( char *name, int &type, int &length, int &index);
int  get_var_value( int code_index, int element_index, int &element_index_next, int &variable_index, int &variable_array_index, int &type, int &value_int, unsigned char &value_char, double &value_float, char *value_string );
int  assignment( long code_index );
int  check_condition( long code_index, int &new_code_index );
int  jump_if_while( int code_index ); 
int  run( char* code, char* json_input, char* json_output, int debug );

}; // class frog

