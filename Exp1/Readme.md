[< Back Home](/Readme.md) 
# Exp 1: Lexcial Analysis
Create a program that reads a real C program and categorize each tokens to appropriate classes

## Sample Input:
> src.c
```
#include <stdio.h>
#include <stdlib.h>

void main() {
    int a, b;

    printf("Enter Number 1: " );
    scanf("%d", &a);

    printf("Enter Number 2: ");
    scanf("%d", &b);

    int sum = (a ^ b) % 6;
    printf("The sum is %d\n", sum);
}
```

## Sample Output
> dest.txt
```
void		->	Keywords
main		->	Identifiers
(		->	Punctuators
)		->	Punctuators
{		->	Punctuators
int		->	Keywords
a		->	Identifiers
,		->	Punctuators
b		->	Numeric Constants
;		->	Punctuators
printf		->	Identifiers
(		->	Punctuators
"Enter Number 1: "	->	String Constants
)		->	Punctuators
;		->	Punctuators
scanf		->	Identifiers
(		->	Punctuators
"%d"	->	String Constants
,		->	Punctuators
&		->	Operators
a		->	Identifiers
)		->	Punctuators
;		->	Punctuators
printf		->	Identifiers
(		->	Punctuators
"Enter Number 2: "	->	String Constants
)		->	Punctuators
;		->	Punctuators
scanf		->	Identifiers
(		->	Punctuators
"%d"	->	String Constants
,		->	Punctuators
&		->	Operators
b		->	Numeric Constants
)		->	Punctuators
;		->	Punctuators
int		->	Keywords
sum		->	Identifiers
=		->	Operators
(		->	Punctuators
a		->	Identifiers
^		->	Operators
b		->	Numeric Constants
)		->	Punctuators
%		->	Operators
6		->	Numeric Constants
;		->	Punctuators
printf		->	Identifiers
(		->	Punctuators
"The sum is %d\n"	->	String Constants
,		->	Punctuators
sum		->	Identifiers
)		->	Punctuators
;		->	Punctuators
}		->	Punctuators
```


## Code: 
[lexical_mini.c](lexical_mini.c)

### Explaination
Let keywords.kw be the file which contains the keywords
> keywords.kw
```
auto
break
case
char
const
continue
default
do
double
else
enum
extern
float
for
goto
if
int
long
register
return
short
signed
sizeof
static
struct
switch
typedef
union
unsigned
void
volatile
while
```

Let operators.opr be the file containing valid operators
> operators.opr
```
+
-
*
/
%
=
+=
-=
*=
/=
%=
.
->
==
!=
>
<
>=
<=
&&
||
!
&
|
~
^
```

> main.c

Include required headers
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
```

Define some constants
```
#define LOADABLE_KEYWORDS_COUNT 32
#define LOADABLE_OPERATORS_COUNT 26
```

Create array of string to hold keywords and operators
```
char keywords[LOADABLE_KEYWORDS_COUNT][16];
char operators[LOADABLE_OPERATORS_COUNT][16];
```

Let load_data be a function to load the required data
```
void load_data(char * filename, int arr_size, char arr[arr_size][16]) {
    FILE *file = fopen(filename, "r");

    for (int i = 0; i < arr_size; i++) 
        fscanf(file, "%s", arr[i]);

    printf("Loaded %d items from %s\n", arr_size, filename);
    fclose(file);
}
```

Create a parse function. This is a core function in this program.
```
bool parse(FILE *src_file, char* regex, char* extracted) { ... }
```
We will go line by line. This function try to parse a scanf regex form to token. If sucess returns true else false

First we will reset the `extracted` string variable and check for the file is readable or not
```
extracted[0] = '\0'; // Reset output string
if (src_file == NULL) return false; // Error if file isn't opened;
```
Mark current file reading head
```
int pointer = ftell(src_file); // Mark current pointer
```
Perform a regex parse, if sucess, we can return immediately else we need to reset head back to previous position if the head got moved to any position during the parse
```
if (fscanf(src_file, regex, extracted) == 1) return true;

fseek(src_file, pointer, SEEK_SET); // Reset pointer back to previous
return false;
```

That's it create a literal extraction function. This will read and copy the characters to the output variable until terminal character is encountered. Escape characters are considered.
```
void extract_literals(FILE *src_file, char terminal, char* extracted) {
    extracted[0] = '\0'; // Reset output string
    if (src_file == NULL) return; // Error if file isn't opened;

    int i = 0;
    while (true) {
        char c = fgetc(src_file);

        if (c == '\\') { 
            // Ignore next character from checking as terminal or not
            extracted[i++] = '\\';
            c = fgetc(src_file);
        } else if (c == terminal) break;

        extracted[i++] = c;
        extracted[i] = '\0';
    }
}
```
Create some helper function find the index of a string from the array of string
```
int index_of(char* value, int arr_size, char arr[arr_size][16]) {
    for (int i = 0; i < arr_size; i++)
        if (strcmp(arr[i], value) == 0) return i;
    
    return -1;
}
```

Create `extract_leximes` function, also next core function. We will go line by line
```
void extract_leximes() {
    FILE *src_file = fopen("src.c", "r");
    FILE *dest_file = fopen("dest.txt", "w");

    char token[100];
    while(!feof(src_file)) { ... }

    fclose(src_file);
    fclose(dest_file);
}
```

This while is crazy. Here is where parsing comes. First we check for numerical categorizations. Regex for that will be `%[bx0-9.fd]`
```
if (parse(src_file, " %[bx0-9.fd]", token)) fprintf(dest_file, "%s\t\t->\tNumeric Constants\n", token);
```
Similarly for identifiers or keywords it will be `%[a-zA-Z0-9]`
```
if (parse(src_file, " %[a-zA-Z0-9]", token)) 
    if (index_of(token, LOADABLE_KEYWORDS_COUNT, keywords) == -1) fprintf(dest_file, "%s\t\t->\tIdentifiers\n", token);
    else fprintf(dest_file, "%s\t\t->\tKeywords\n", token);
```
For Operators: `%[-+*/%.<>=!&|~^]`
```
if (parse(src_file, " %[-+*/%.<>=!&|~^]", token)) 
    if (index_of(token, LOADABLE_OPERATORS_COUNT, operators) != -1) fprintf(dest_file, "%s\t\t->\tOperators\n", token);
    else for (int i = 0; i < strlen(token); i++) fprintf(dest_file, "%c\t\t->\tPunctuators\n", token[i]);
```
If all these failed to parse we will go for character wise check for other compinations
```
char c = getc(src_file);
if (c == EOF) break;

if (c == '#') fscanf(src_file, " %*[^\n]"); // Skip line
else if (c == '\'') {
    extract_literals(src_file, '\'', token);
    fprintf(dest_file, "\'%s\'\t\t->\tCharacter Constants\n", token);
} else if (c == '\"') {
    extract_literals(src_file, '\"', token);
    fprintf(dest_file, "\"%s\"\t->\tString Constants\n", token);
} else if (c == ' ' || c == '\t' || c == '\n') continue; // Ignore these characters
else fprintf(dest_file, "%c\t\t->\tPunctuators\n", c);
```
That's it

Finally the main function
```
int main() {
    load_data("keywords.kw", LOADABLE_KEYWORDS_COUNT, keywords);
    load_data("operators.opr", LOADABLE_OPERATORS_COUNT, operators);

    extract_leximes();

    return 0;
}
```