
#define true 1
#define false 0
#define bool int


#include <stdio.h>
#include "dtw.h"
#include "funcs.h"
#define  CACHE_FOLDER ".cache"
int main(int argc, char *argv[]){
    

    //means the user did not provide the repo to clone
    if (argc < 2){
        printf("Usage: %s <repo_to_clone>\n",argv[0]);
        return 1;
    }

    char *repo_to_clone = argv[1];
    //check if the repo name is too long
    if(get_strlen(argv) > 900){
        printf("The repo name is too long\n");
        return 1;
    }

    char command[1000];
    sprintf(command,"git clone %s %s");
    return 0;
}