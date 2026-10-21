#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>

int written(char buffer){
    return write(1, &buffer, 1);
}

size_t length_positive_number_ten(size_t num){
    size_t reste = num;
    size_t digits = 1;
    while (reste > 9){ //Base 10 - 1 so 9
        reste /= 10; //base 10
        digits++;
    }
    return digits;
}

size_t length_positive_number_eight(size_t num){
    size_t reste = num;
    size_t digits = 1;
    while (reste > 7){ //Base 8 - 1 so 7
        reste /= 8; //base 8
        digits++;
    }
    return digits;
}

size_t length_positive_number_sixteen(size_t num){
    size_t reste = num;
    size_t digits = 1;
    while (reste > 15){ //Base 16 - 1 so 15
        reste /= 16; //base 16
        digits++;
    }
    return digits;
}

size_t length_number(double num){
    return num >= 0 ? length_positive_number_ten(num) : length_positive_number_ten(num * -1) + 1;
}

int print_int_d(int number){
    size_t number_length = length_number(number);
    int n = number;
    char *buf = malloc(number_length + 1); //+1 pour \0
    if (buf == NULL){
        return -1;
    }

    buf[number_length] = '\0';
    for (size_t j = 0; j < number_length; j++){
        buf[(number_length - 1) - j] = n < 0 ? '0' - (n % 10) : '0' + (n % 10);
        n /= 10;
    }
    
    if (number < 0 && buf[0] == '0'){
        buf[0] = '-';
    }

    int byte = write(1, buf, number_length);

    free(buf);
    return byte;
}

int print_int_u(unsigned int number){
    size_t number_length = length_positive_number_ten(number);
    unsigned int n = number;
    char *buf = malloc(number_length + 1); //+1 pour \0
    if (buf == NULL){
        return -1;
    }

    buf[number_length] = '\0';

    for (size_t j = 0; j < number_length; j++){
        buf[(number_length - 1) - j] = '0' + (n % 10);
        n /= 10;
    }

    int byte = write(1, buf, number_length);

    free(buf);
    return byte;
}

int print_int_o(unsigned int number){
    size_t number_length = length_positive_number_eight(number);
    unsigned int n = number;
    char *buf = malloc(number_length + 1); //+1 pour \0
    if (buf == NULL){
        return -1;
    }

    buf[number_length] = '\0';

    for (size_t j = 0; j < number_length; j++){
        buf[(number_length - 1) - j] = '0' + (n % 8);
        n /= 8;
    }


    int byte = write(1, buf, number_length);

    free(buf);
    return byte;
}

int print_int_x(unsigned int number){
    size_t number_length = length_positive_number_sixteen(number);
    unsigned int n = (unsigned int)number;
    char *buf = malloc(number_length + 1); //+1 pour \0
    if (buf == NULL){
        return -1;
    }

    buf[number_length] = '\0';
    
    for (size_t j = 0; j < number_length; j++){
        if ((n % 16) > 9){
            buf[(number_length - 1) - j] = '0' + (39 + n % 16);
            n /= 16;

        } else {
            buf[(number_length - 1) - j] = '0' + (n % 16);
            n /= 16;
        }
    }

    int byte = write(1, buf, number_length);

    free(buf);
    return byte;
}

int print_int_p(void *pointer){
    if (pointer == NULL){
        int byte = write(1, "(nil)\0", 6);
        return byte;
    }
    uintptr_t n = (uintptr_t)pointer;
    size_t number_length = length_positive_number_sixteen(n);
    char *buf = malloc(number_length + 3); //+1 pour \0
    if (buf == NULL){
        return -1;
    }
    
    buf[number_length + 2] = '\0';
    buf[0] = '0';
    buf[1] = 'x';

    for (size_t j = 0; j < number_length; j++){
        int bu = (number_length - 1) - j + 2;


        if ((n % 16) > 9){
            buf[bu] = '0' + (39 + n % 16);
            n /= 16;

        } else {
            buf[bu] = '0' + (n % 16);
            n /= 16;
        }
    }

    int byte = write(1, buf, number_length + 2);

    free(buf);
    return byte;
}

int print_unsigned_int (unsigned int number){
    size_t number_length = length_number(number);
    int n = number;
    char *buf = malloc(number_length + 1); //+1 pour \0
    if (buf == NULL){
        return -1;
    }

    buf[number_length] = '\0';
    
    for (size_t j = 0; j < number_length; j++){
        buf[(number_length - 1) - j] = n < 0 ? '0' - (n % 10) : '0' + (n % 10);
        n /= 10;
    }
    
    if (number <= 0 && buf[0] == '0'){
        buf[0] = '-';
    }

    int byte = write(1, buf, number_length);

    free(buf);
    return byte;
}

int write_pourcent_c(va_list parametersInfos){
    char c = va_arg(parametersInfos, int);
    int byte_c = written(c);

    if (byte_c != 1){
        va_end(parametersInfos);
        return -1;  
    }
    return byte_c;
}
int write_pourcent_s(va_list parametersInfos){
    char *s = va_arg(parametersInfos, char *);
    int byte_s = 0;
    if (s != NULL){
        for (int j = 0; s[j] != '\0'; j++){
            int byte_s = written(s[j]);
    
            if (byte_s != 1){
                va_end(parametersInfos);
                return -1; 
            }
        }
    }
    else{
        va_end(parametersInfos);
        return -1;
    }
    return byte_s;
}

int my_printf(char * restrict format, ...){
    int counter = 0;
    va_list parametersInfos;
    va_start(parametersInfos, format);
    size_t i = 0;
    while(format[i] != '\0'){
        if (format[i] != '%'){ //%
            int wrote = written(format[i]);

            if (wrote != 1){
                va_end(parametersInfos);
                return -1;
            }

            else{
                counter++;
            }
        }

        else if(format[i + 1] == '%'){ //%%
            int byte_pourcent = written(format[i]);
            i++;
            if (byte_pourcent != 1){
                va_end(parametersInfos);
                return -1;
            }
            else{
                counter++;
                }
        }
        else if (format[i + 1] == 'c'){ //%c
            write_pourcent_c(parametersInfos);
            i++;
            counter++;
        }
        else if (format[i + 1] == 's'){ //%s
            write_pourcent_s(parametersInfos);
            i++;
            counter++;
        
        }
        else if((format[i + 1] == 'd') || (format[i + 1] == 'i')){ //%d et %i
            int d = va_arg(parametersInfos, int);
            print_int_d(d);
            i++;
            counter++;
        }
        else if(format[i + 1] == 'u'){ //%u
            unsigned int u = va_arg(parametersInfos, unsigned int);
            print_int_u(u);
            i++;
            counter++;
        }
        
        else if (format[i + 1] == 'o'){ //%o
            unsigned int o = va_arg(parametersInfos, unsigned int);
            print_int_o(o);
            i++;
            counter++;              
        }

        else if (format[i + 1] == 'x'){ //%x
            unsigned int x = va_arg(parametersInfos, unsigned int);
            print_int_x(x);
            i++;
            counter++;

        }

        else if (format[i + 1] == 'p'){ //%p
            void *p = va_arg(parametersInfos, void *);
            print_int_p(p);
            i++;
            counter++;   
        }
    i++;
    }
    va_end(parametersInfos);
    return counter;
           
}

int main(){
    char * a = malloc(2);
    my_printf("%d\n", -42);
    printf("%d\n", -42);
    my_printf("%d\n", 0);
    printf("%d\n", 0);
    my_printf("%u\n", -1);
    printf("%u\n", -1);
    my_printf("%x\n", -1);
    printf("%x\n", -1);
    my_printf("%o\n", 0);
    printf("%o\n", 0);
    my_printf("%c\n", 65);
    printf("%c\n", 65);
    my_printf("%c\n", 0);
    printf("%c\n", 0);
    my_printf("%s\n", "world");
    printf("%s\n", "world");
    my_printf("%p\n", NULL);
    printf("%p\n", NULL);
    my_printf("%%\n");
    printf("%%\n");
    my_printf("Hello %s, you are %d!\n", "Bob", 30);
    printf("Hello %s, you are %d!\n", "Bob", 30);
    my_printf("no format here\n");
    printf("no format here\n");
    my_printf("%d\n", INT_MIN);
    printf("%d\n", INT_MIN);
    free(a);
    return 0;
}