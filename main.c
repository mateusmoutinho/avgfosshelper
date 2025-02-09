
#define true 1
#define false 0
#define bool int

#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
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
    sprintf(csp,"%s/%s/c\n",CF, hs);

    //project dir
    char pd[1000] = {0};
    sprintf(pd,"%s/%s",CF,hs);
    //project path
    char pp  [1000] = {0};
    sprintf(pp,"%s/%s/p",CF,hs);
    //last modification path

    char lmp [1000] = {0};
    sprintf(lmp,"%s/%s/l",CF,hs);
    /// its repo cached 
    bool irc = false;
    /// cloning the repo ----------------------------------------------------
    //check if check sum path its a file 

    if(dtw_entity_type(csp) == 1){
        if(dtw_entity_type(lmp) == 1){

            if(dtw_entity_type(pp) == 2){
                irc = true;
                // implemetar um check sum
            }
            else{
                printf("intergrity check failed\n");
                return 1;
            }
        }else{
            printf("intergrity check failed\n");
            return 1;
        }
    }


    if(!irc){
        dtw_create_dir_recursively(pp);
        char cmd[1000] = {0};
        sprintf(cmd,"cd %s && git clone %s",pp,rpc);
        int error =  system(cmd);  
        if(error != 0){
            printf("Error cloning the repo\n");
            //dtw_remove_any(pd);
            return 1;
        }
        ///files of project
        DtwStringArray *all_files = dtw_list_files_recursively(pp,1);
        ///acumulted hash
        uint8_t acumulated_hash[32] = {0};
        //acumulated hash setted
        bool acumulated_hash_setted= false;

        for(int i = 0; i < all_files->size;i++){
            //current file
            char *current_file = all_files->strings[i];
            printf("%s\n",current_file);
            long size;
            bool is_binary;
            unsigned char *content = dtw_load_any_content(current_file,&size,&is_binary);
            if(!acumulated_hash ){
                acumulated_hash_setted = true;
                calc_sha_256(acumulated_hash,content,size);
            }else{
                uint8_t current_hash[32] = {0};
                calc_sha_256(current_hash,content,size);
                char join_hash[65] = {0};
                strcat(join_hash,acumulated_hash);
                strcat(join_hash,current_hash);
                printf("%s\n",join_hash);
                calc_sha_256(acumulated_hash,join_hash,64);
            }

        }
    }
    

    return 0;
    //comand 
    
}