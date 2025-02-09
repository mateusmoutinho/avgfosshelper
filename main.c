
#define true 1
#define false 0
#define bool int
#include <time.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include "dtw.h"
#include "funcs.h"
#include "sha-256.h"
//cache folder
#define  CF ".cache"
//timeout
#define TM 1000 
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
    //project internal dir 
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
        //check if the last modification path is a file
        if(dtw_entity_type(lmp) == 1){
            //check if the project its a folder
            if(dtw_entity_type(pp) == 2){
                irc = true;
                // implemetar um check sum
                ///==========================Generating sha of project=======================================
                ///files of project
                DtwStringArray *afs = dtw_list_files_recursively(pp,1);
                ///acumulted hash
                uint8_t ahs[32] = {0};
                //acumulated hash setted
                bool ass= false;

                for(int i = 0; i < afs->size;i++){
                    //current file
                    char *current_file = afs->strings[i];
                    //size
                    long s;
                    //is binary
                    bool ib;
                    //content
                    unsigned char *c = dtw_load_any_content(current_file,&s,&ib);
                    if(!ahs ){
                        ass = true;
                        calc_sha_256(ahs,c,s);
                    }else{

                        ///current hash
                        uint8_t ch[32] = {0};
                        calc_sha_256(ch,c,s);
                        ///join hash
                        uint8_t jh[65] = {0};
                        memcpy(jh,ahs,32);
                        memcpy(jh+32,ch,32);
                        calc_sha_256(ahs,jh,64);
                    
                    }

                }
                //actumullated hash syt
                char ast[65] = {0};
                for (unsigned int i = 0; i < SIZE_OF_SHA_256_HASH; i++) {
                    sprintf(ast + i * 2, "%02x", ahs[i]);
                }
                //old hash
                char *olh = dtw_load_string_file_content(csp);
                if(strcmp(ast,olh) != 0){                  
                    printf("intergrity check failed dont modifiy the cache dir\n");
                    return 1;
                }

s

                //=====================================================================================
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
        
        ///==========================Generating sha of project=======================================
        ///files of project
        DtwStringArray *afs = dtw_list_files_recursively(pp,1);
        ///acumulted hash
        uint8_t ahs[32] = {0};
        //acumulated hash setted
        bool ass= false;

        for(int i = 0; i < afs->size;i++){
            //current file
            char *current_file = afs->strings[i];
            //size
            long s;
            //is binary
            bool ib;
            //content
            unsigned char *c = dtw_load_any_content(current_file,&s,&ib);
            if(!ahs ){
                ass = true;
                calc_sha_256(ahs,c,s);
            }else{

                ///current hash
                uint8_t ch[32] = {0};
                calc_sha_256(ch,c,s);
                ///join hash
                uint8_t jh[65] = {0};
                memcpy(jh,ahs,32);
                memcpy(jh+32,ch,32);
                calc_sha_256(ahs,jh,64);
            
            }

        }
        //actumullated hash syt
        char ast[65] = {0};
        for (unsigned int i = 0; i < SIZE_OF_SHA_256_HASH; i++) {
            sprintf(ast + i * 2, "%02x", ahs[i]);
        }
        //=====================================================================================
        dtw_write_string_file_content(csp,ast);
        //time of last update
        char ts[20] = {0};
        sprintf(ts,"%ld",time(NULL));
        dtw_write_string_file_content(lmp,ts);
    }
    

    return 0;
    //comand 
    
}