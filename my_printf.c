#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

int written(char buffer){
    return write(1, &buffer, 1);
}

size_t length_positive_number(size_t num){
    int reste = num;
    size_t digits = 0;
    while (reste > 0){
        reste /= 10;
        digits++;
    }
    // printf("%lu\n", digits);
    return digits;
}

size_t length_number(double num){
    return num > 0 ? length_positive_number(num) : length_positive_number(num * -1) + 1;
}

int print_number(int number){
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
                if (format[i + 1] == '\0'){
                    va_end(parametersInfos);
                    return counter;
                }

            }
        }

        else if(format[i] && format[i + 1] == '%'){ //%%
            int byte_pourcent = written(format[i]);
            i = i + 1;
            if (byte_pourcent != 1){
                va_end(parametersInfos);
                return -1;
            }
            else{
                counter++;
                }
        }
        else if (format[i] && format[i + 1] == 'c'){ //%c

            char c = va_arg(parametersInfos, int);
            int byte_c = written(c);
            i++;

            if (byte_c != 1){
                va_end(parametersInfos);
                return -1;  
            }
            else{
                counter++;
            }
        }
        else if (format[i] && format[i + 1] == 's'){ //%s

            char *s = va_arg(parametersInfos, char *);
            i++;
            if (s != NULL){
                for (int j = 0; s[j] != '\0'; j++){
                    int byte_s = written(s[j]);
            
                    if (byte_s != 1){
                        va_end(parametersInfos);
                        return -1; 
                    }
                    else{
                        counter++;
                    }
                }
            }
            else{
                va_end(parametersInfos);
                return -1;
            }
        
        }
        else if((format[i] && format[i + 1] == 'd') || (format[i] && format[i + 1] == 'i')){ //%d et %i
            int d = va_arg(parametersInfos, int);
            i++;
            print_number(d);
        }
        else if(format[i] && format[i + 1] == 'u'){ //%u
            unsigned int n = va_arg(parametersInfos, unsigned int);
            print_number(n);
            i++;
            counter++;
        }
        
        else if (format[i] && format[i + 1] == 'o'){ //%o
            int o = va_arg(parametersInfos, int);
            print_number(o);
            i++;
            counter++;              
        }
        i++;
    }
    va_end(parametersInfos);
    return counter;
           
}

int main(){
    int r = my_printf("%d\n", -513);
    return r;
}