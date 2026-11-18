#include <unistd.h>   // write()
#include <stdarg.h>   // va_list, va_start, va_arg, va_end
#include <stdlib.h>   // malloc(), free()
#include <stdio.h>    // printf() (only used in main to compare with the real one)
#include <stdint.h>   // uintptr_t (integer type big enough to hold a pointer)
#include <limits.h>   // INT_MIN (used in the tests)

int written(char buffer){
    return write(1, &buffer, 1);
}

size_t length_positive_number(size_t num, size_t base){
    size_t reste = num;       // what remains of the number as we divide it
    size_t digits = 1;        // at least one digit, even for 0
    while (reste > base - 1){ // while there is still more than one digit left
        reste /= base;        // drop the last digit
        digits++;             // ...and count it
    }
    return digits;
}

size_t length_number(double num){
    return num >= 0 ? length_positive_number(num, 10) : length_positive_number(num * -1, 10) + 1;
}

int print_int_d(int number){
    size_t number_length = length_number(number); // number of characters (sign included)
    int n = number;                               // working copy that we will divide
    char *buf = malloc(number_length + 1); //+1 pour \0
    if (buf == NULL){
        return -1;
    }

    buf[number_length] = '\0'; // end of string

    // Fill the buffer from right to left, one digit at a time
    for (size_t j = 0; j < number_length; j++){
        // If n is negative, n % 10 is negative or zero, so we use '0' - (n % 10)
        // to get a positive digit. This also works for INT_MIN (no overflow
        buf[(number_length - 1) - j] = n < 0 ? '0' - (n % 10) : '0' + (n % 10);
        n /= 10; // move to the next digit
    }

    // For a negative number, the first slot was not filled with a real digit
    // (n has become 0 by then), so it contains '0': we replace it with '-'.
    if (number < 0 && buf[0] == '0'){
        buf[0] = '-';
    }

    int byte = write(1, buf, number_length); // print the whole buffer at once

    free(buf); // do not forget to free the memory
    return byte;
}

int print_int(unsigned int number, size_t base){
    size_t number_length = length_positive_number(number, base); // number of digits
    unsigned int n = number;                                      // working copy
    char *buf = malloc(number_length + 1); //+1 pour \0
    if (buf == NULL){
        return -1;
    }

    buf[number_length] = '\0';

    // Fill from right to left: the remainder of n / base is the last digit
    for (size_t j = 0; j < number_length; j++){
        buf[(number_length - 1) - j] = '0' + (n % base);
        n /= base;
    }


    int byte = write(1, buf, number_length);

    free(buf);
    return byte;
}

int print_int_x(unsigned int number){
    size_t number_length = length_positive_number(number, 16); // number of hex digits
    unsigned int n = (unsigned int)number;                      // working copy
    char *buf = malloc(number_length + 1); //+1 pour \0
    if (buf == NULL){
        return -1;
    }

    buf[number_length] = '\0';

    for (size_t j = 0; j < number_length; j++){
        if ((n % 16) > 9){
            // '0' is 48, so 48 + 39 + 10 = 97 = 'a'.
            // So 10 -> 'a', 11 -> 'b', ..., 15 -> 'f'.
            buf[(number_length - 1) - j] = '0' + (39 + n % 16);
            n /= 16;

        } else {
            // Digits 0 to 9: simple conversion to ASCII
            buf[(number_length - 1) - j] = '0' + (n % 16);
            n /= 16;
        }
    }

    int byte = write(1, buf, number_length);

    free(buf);
    return byte;
}

int print_int_p(void *pointer){
    // Special case: NULL pointer. We only send 5 bytes: "(nil)" (the \0 is not printed).
    if (pointer == NULL){
        int byte = write(1, "(nil)\0", 5);
        return byte;
    }
    uintptr_t n = (uintptr_t)pointer;                         // address seen as an integer
    size_t number_length = length_positive_number(n, 16);     // number of hex digits
    char *buf = malloc(number_length + 3); //+1 pour \0 (+2 for the "0x" prefix)
    if (buf == NULL){
        return -1;
    }

    buf[number_length + 2] = '\0'; // end of string, after "0x" + digits
    buf[0] = '0';                  // prefix "0x"
    buf[1] = 'x';

    for (size_t j = 0; j < number_length; j++){
        int bu = (number_length - 1) - j + 2; // position in buf (shifted by 2 because of "0x")


        if ((n % 16) > 9){
            buf[bu] = '0' + (39 + n % 16); // 10..15 -> 'a'..'f'
            n /= 16;

        } else {
            buf[bu] = '0' + (n % 16);      // 0..9 -> '0'..'9'
            n /= 16;
        }
    }

    int byte = write(1, buf, number_length + 2); // "0x" + digits

    free(buf);
    return byte;
}

int write_pourcent_c(char c){
    int byte_c = written(c);

    if (byte_c != 1){ // write failed
        return -1;
    }
    return byte_c;
}

int write_pourcent_s(char *s){
    int counter = 0; // number of characters written so far

    if (s == NULL){  // like the real printf: a NULL string prints "(null)"
        s = "(null)";
    }

    for (int j = 0; s[j] != '\0'; j++){
        if (written(s[j]) != 1){ // write failed
            return -1;
        }
        counter++;
    }
    return counter;
}

int my_printf(char * restrict format, ...){
    int counter = 0;               // total number of characters written
    va_list parametersInfos;       // list of the variadic arguments
    va_start(parametersInfos, format); // start reading after the "format" parameter
    size_t i = 0;                  // current position in the format string

    while(format[i] != '\0'){
        if (format[i] != '%'){ //%
            // Normal character: print it as is
            int wrote = written(format[i]);

            if (wrote != 1){
                va_end(parametersInfos);
                return -1;
            }
            counter += wrote;
        }

        else if(format[i + 1] == '%'){ //%%
            // "%%" prints a single '%'
            int byte_pourcent = written(format[i]);

            i++; // skip the second '%'
            if (byte_pourcent != 1){
                va_end(parametersInfos);
                return -1;
            }
            counter += byte_pourcent;
        }

        else if (format[i + 1] == 'c'){ //%c
            char c = (char)va_arg(parametersInfos, int);
            counter += write_pourcent_c(c);
            i++; // skip the conversion letter
        }

        else if (format[i + 1] == 's'){ //%s
            char *s = va_arg(parametersInfos, char *);
            counter += write_pourcent_s(s);
            i++; // skip the conversion letter
        }

        else if((format[i + 1] == 'd') || (format[i + 1] == 'i')){ //%d et %i
            int d = va_arg(parametersInfos, int); // next argument as a signed int
            counter += print_int_d(d);
            i++; // skip the conversion letter
        }

        else if(format[i + 1] == 'u'){ //%u
            unsigned int u = va_arg(parametersInfos, unsigned int); // unsigned decimal
            counter += print_int(u, 10);
            i++; // skip the conversion letter
        }

        else if (format[i + 1] == 'o'){ //%o
            unsigned int o = va_arg(parametersInfos, unsigned int); // unsigned octal
            counter += print_int(o, 8);
            i++; // skip the conversion letter
        }

        else if (format[i + 1] == 'x'){ //%x
            unsigned int x = va_arg(parametersInfos, unsigned int); // unsigned hexadecimal
            counter += print_int_x(x);
            i++; // skip the conversion letter
        }

        else if (format[i + 1] == 'p'){ //%p
            void *p = va_arg(parametersInfos, void *); // pointer address
            counter += print_int_p(p);
            i++; // skip the conversion letter
        }

        i++;
    }
    va_end(parametersInfos); // clean up the variadic argument list
    return counter;

}

int main(){
    char * a = malloc(2); // allocated just to test with free() at the end
    char * volatile  null_str = NULL; // volatile: stops GCC from seeing that it's NULL
    printf("%d\n", my_printf("%d\n", -42));
    printf("%d\n", printf("%d\n", -42));
    printf("%d\n", my_printf("%d\n", 0));
    printf("%d\n", printf("%d\n", 0));
    printf("%d\n", my_printf("%u\n", -1));
    printf("%d\n", printf("%u\n", -1));
    printf("%d\n", my_printf("%x\n", -1));
    printf("%d\n", printf("%x\n", -1));
    printf("%d\n", my_printf("%o\n", 0));
    printf("%d\n", printf("%o\n", 0));
    printf("%d\n", my_printf("%c\n", 65));
    printf("%d\n", printf("%c\n", 65));
    printf("%d\n", my_printf("%c\n", 0));
    printf("%d\n", printf("%c\n", 0));
    printf("%d\n", my_printf("%s\n", "world"));
    printf("%d\n", printf("%s\n", "world"));
    printf("%d\n", my_printf("%p\n", NULL));
    printf("%d\n", printf("%p\n", NULL));
    printf("%d\n", my_printf("%%\n"));
    printf("%d\n", printf("%%\n"));
    printf("%d\n", my_printf("Hello %s, you are %d!\n", "Bob", 30));
    printf("%d\n", printf("Hello %s, you are %d!\n", "Bob", 30));
    printf("%d\n", my_printf("no format here\n"));
    printf("%d\n", printf("no format here\n"));
    printf("%d\n", my_printf("%d\n", INT_MIN));
    printf("%d\n", printf("%d\n", INT_MIN));
    my_printf("%s %s %s!\n", "Hello", "Mr", "Gaetan");
    printf("%c%c%c%c%c!\n", 'H', 'e', 'l', 'l', 'o');
    my_printf("%c%c%c%c%c!\n", 'H', 'e', 'l', 'l', 'o');
    printf("NULL STRING %s!\n", null_str);
    my_printf("NULL STRING %s!\n", null_str);
    free(a);
    return 0;
}
