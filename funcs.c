
#include "funcs.h"
//get thhe string length 
int gsl(const char *str){
    int i = 0;
    while (str[i] != '\0'){
        i++;
    }
    return i;
}