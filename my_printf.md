Submit directory: . Submit file: my_printf.c

NAME
int my_printf(char * restrict format, ...);
Prolog
The very first thing we do when learning a programming language is to write “Hello World!” on the screen. Well, guess what? This custom comes from a notebook called... Programming in C: A Tutorial!

Unfortunately for you, this project will bring you back to the 1970s, before the existence of this book, and you will have to write your own function to greet the world.

Fortunately for you, you will strengthen your programming skills and learn new programming concepts: variadic functions and variable argument lists. Recoding printf is also a good exercise to learn how to have a well-structured code organization. Modularity is the key here, since printf has a lot of different options that you will have to recode yourself. Adding a new option/feature should be easy and smooth if the code is well designed!

Description
The my_printf() function products output according to a format, as described below. The my_printf() function writes output to stdout, the standard output stream.

This function writes the output under the control of a format string that specifies how subsequent arguments (or arguments accessed via the variable-length argument facilities of stdarg(3)) are converted for output.

The format string is composed of zero or more directives: ordinary characters (not %), which are copied unchanged to the output stream; and conversion specifications, each of which results in fetching zero or more subsequent arguments.

Each conversion specification is introduced by the % character. The arguments must correspond properly (after type promotion) with the conversion specify. After the %, the following may appear:

Specify	Description
d/ i	The int (or appropriate variant) argument is converted to signed decimal.
o	Unsigned octal.
u	Unsigned decimal.
x	Unsigned hexadecimal.
c	The int argument is converted to an unsigned char, and the resulting character is written.
s	The char * argument is expected to be a pointer to an array of character type (pointer to a string). Characters from the array are written up to (but not including) a terminating NUL character.
p	The void * pointer argument is printed in hexadecimal.
Requirements
Your code must be compiled with the flags -Wall -Wextra -Werror.
Hint(s)
man printf
man 3 stdarg
Watch out for memory leaks!
You can test your code against memory errors by compiling with the debugging flags -g3 -fsanitize=address
Global variables are strictly FORBIDDEN
Static variables are strictly FORBIDDEN
Authorized function(s)
write(2)
malloc
free
va_start, va_arg, va_copy, va_end (see man 3 stdarg)
(Obvious) Unauthorized functions
printf and co. (fprintf, sprintf, snprintf, asprintf, dprintf, vprintf, vfprintf, vsprintf, vsnprintf, vasprintf, vdprintf)
Multiline macros are forbidden
Including another .c is forbidden
Macros with logic (while/if/variables/...) are forbidden
Epilogue
Don't forget you can ask questions on the chat!

Good luck!

🧪 Test ideas to consider — My Printf
Input "%d", -42
Expected Output -42
Input "%d", 0
Expected Output 0
Input "%u", -1
Expected Output 4294967295
💡 Unsigned representation of a negative int (32 bits)
Input "%x", -1
Expected Output ffffffff
💡 Two's complement
Input "%o", 0
Expected Output 0
Input "%c", 65
Expected Output A
Input "%c", 0
Expected Output (a null byte is still written)
💡 Trap: write() must write the \0 byte, not stop at it like a normal C string would
Input "%s", "world"
Expected Output world
Input "%p", NULL
Expected Output (nil) or 0x0 depending on your implementation
Input "%%"
Expected Output %
💡 Must not be interpreted as an incomplete format
Input "Hello %s, you are %d!", "Bob", 30
Expected Output Hello Bob, you are 30!
Input "no format here"
Expected Output no format here
💡 my_printf should behave like write() with no specify
Input "%d", INT_MIN
Expected Output -2147483648
💡 Edge case: the absolute value of INT_MIN doesn't fit in an int, watch out if you use an internal abs()
Input Check my_printf's return value
Expected Output Must equal the actual number of characters written (like the real printf)
💡 Common trap: forcing to count some characters, or counting the \0 by mistake