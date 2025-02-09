
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
    ///hash of the repo url
    char hs[60] ={0}; 
    for (unsigned int i = 0; i < SIZE_OF_SHA_256_HASH; i++) {
        sprintf(hs + i * 2, "%02x", rsv[i]);
    }
    //check sum path
    char csp[1000] = {0};
    sprintf("%s/%s/c\n",CF,hs);
    //project path
    char pp  [1000] = {0};
    sprintf("%s/%s/p\n",CF,hs);
    //last modification path

    char lmp [1000] = {0};
    sprintf("%s/%s/l\n",CF,hs);

    return 0;
    //comand 
    
}