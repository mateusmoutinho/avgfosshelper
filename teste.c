#include "DoTheWorld/src/one.c"

int main(){
    char minh_str[20] = "aaaa";
    memcpy(minh_str+4,"mateus",6);
    printf("%s\n",minh_str);
}