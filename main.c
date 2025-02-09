
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
    void rsv[32] = {0};
    calc_sha_256(rs,rpc,gsl(rpc));
    printf("%s\n",rs);
    return 0;
    //comand 
    char c[1000]; 
    sprintf(c,"cd %s/%s &&  git clone %s",CF);
    return 0;
}