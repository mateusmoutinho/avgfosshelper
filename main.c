
#define true 1
#define false 0
#define bool int
#include <dirent.h>

#include <stdio.h>
#include "dtw.h"
#include "funcs.h"
#include "sha-256.h"
//cache folder
#define  CF ".cache" 
int main(int argc, char *argv[]){
    

    //means the user did not provide the repo to clone
    if (argc < 2){
        printf("Usage: %s <repo_to_clone>\n",argv[0]);
        return 1;
    }

    //repo to clone
    char *rpc = argv[1];
    //check if the repo name is too long
    if(gsl(rpc) > 900){
        printf("The repo name is too long\n");
        return 1;
    }
    //repo sha
    uint8_t rsv[32] = {0};
    calc_sha_256(rsv,rpc,gsl(rpc));
    char hash_string[60] ={0}; 
    for (unsigned int i = 0; i < SIZE_OF_SHA_256_HASH; i++) {
        sprintf(hash_string + i * 2, "%02x", rsv[i]);
    }

    char check_sum[1000] = {0};
    printf("%s\n",hash_string);
    return 0;
    //comand 
    
}