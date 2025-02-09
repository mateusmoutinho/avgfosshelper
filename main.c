
#define true 1
#define false 0
#define bool int
#include <time.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include "dtw.h"
#include "sha-256.h"
//cache folder
//timeout
int main(int argc, char *argv[]){
    
    for(int i =1; i < argc;i++){
        if(strcmp(argv[i],"--help") == 0){
            printf("Code review tool\n");
            printf("version:29.5.124\n");
            printf("License: AGPL\n");
            printf("Usage: %s <repo_to_clone> [--cache <cache_folder>] [--timeout <timeout_in_seconds>] [--line-chunk <line_chunk>]\n",argv[0]);
            printf("Options:\n");
            printf("  --cache <cache_folder>  Set the cache folder\n");
            printf("  --timeout <timeout_in_seconds>  Set the timeout in seconds\n");
            printf("  --line-chunk <line_chunk>  Set the line chunk\n");
            return 0;
        }
        if(strcmp(argv[i],"--version") == 0){
            printf("29.5.124\n");
            return 0;
        }
    }
    
    //means the user did not provide the repo to clone
    if (argc < 2){
        printf("Usage: %s <repo_to_clone>\n",argv[0]);
        return 1;
    }
    //cache folder
    char cf[1000] = {0};
    //cache folder setted
    bool cfs = false;
    //check if the user provided the cache folder
    for(int i =1; i < argc-1;i++){
        if(strcmp(argv[i],"--cache") == 0){
            strcpy(cf,argv[i+1]);
            cfs = true;
        }
    }
    
    if(!cfs){
        sprintf(cf,"/tmp/.cache");
    }


    //timeout in seconds
    long to  = 300;
    for(int i =1; i < argc-1;i++){
        if(strcmp(argv[i],"--timeout") == 0){
            to = atol(argv[i+1]);
        }
    }
    //line limit
    long ll = 40;
    for(int i =1; i < argc-1;i++){
        if(strcmp(argv[i],"--line-chunk") == 0){
            ll = atol(argv[i+1]);
        }
    }
    //repo to clone
    char *rpc = argv[1];
    //check if the repo name is too long
    if(strlen(rpc) > 900){
        printf("The repo name is too long\n");
        return 1;
    }
    //repo sha
    uint8_t rsv[32] = {0};
    calc_sha_256(rsv,rpc,strlen(rpc));
    ///hash of the repo url
    char hs[60] ={0}; 
    for (unsigned int i = 0; i < SIZE_OF_SHA_256_HASH; i++) {
        sprintf(hs + i * 2, "%02x", rsv[i]);
    }

    //check sum path
    char csp[1000] = {0};
    sprintf(csp,"%s/%s/c\n",cf, hs);

    //project dir
    char pd[1000] = {0};
    sprintf(pd,"%s/%s",cf,hs);
    //project internal dir 
    char pp  [1000] = {0};
    sprintf(pp,"%s/%s/p",cf,hs);

    //last modification path
    char lmp [1000] = {0};
    sprintf(lmp,"%s/%s/l",cf,hs);
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
                   
                    //content
                    uint8_t lmfs[20] = {0};
                    long lmf = dtw_get_entity_last_motification_in_unix(current_file);
                    sprintf(lmfs,"%ld",lmf);
                    if(!ahs ){
                        ass = true;
                        calc_sha_256(ahs,lmfs,strlen(lmfs));
                    }else{

                        ///current hash
                        uint8_t ch[32] = {0};
                        calc_sha_256(ch,lmfs,strlen(lmfs));
                        ///join hash
                        uint8_t jh[65] = {0};
                        memcpy(jh,ahs,32);
                        memcpy(jh+32,ch,32);
                        calc_sha_256(ahs,jh,64);
                    
                    }

                }
                DtwStringArray_free(afs);
                //actumullated hash syt
                char ast[65] = {0};
                for (unsigned int i = 0; i < SIZE_OF_SHA_256_HASH; i++) {
                    sprintf(ast + i * 2, "%02x", ahs[i]);
                }
                //old hash
                char *olh = dtw_load_string_file_content(csp);
                if(strcmp(ast,olh) != 0){                  
                    free(olh);
                    printf("intergrity check failed dont modifiy the cache dir\n");
                    return 1;
                }

                    free(olh);

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
        char cmd[2000] = {0};
        sprintf(cmd,"cd %s && git clone %s --quiet",pp,rpc);
        
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
            char *cf = afs->strings[i];
            
            //content
            uint8_t lmfs[20] = {0};
            long lmf = dtw_get_entity_last_motification_in_unix(cf);
            sprintf(lmfs,"%ld",lmf);
            if(!ahs ){
                ass = true;
                calc_sha_256(ahs,lmfs,strlen(lmfs));
            }else{

                ///current hash
                uint8_t ch[32] = {0};
                calc_sha_256(ch,lmfs,strlen(lmfs));
                ///join hash
                uint8_t jh[65] = {0};
                memcpy(jh,ahs,32);
                memcpy(jh+32,ch,32);
                calc_sha_256(ahs,jh,64);
            
            }

        }
        DtwStringArray_free(afs);

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
    
    //===============================pull check========================================
    //lastt update string
    char *lus = dtw_load_string_file_content(lmp);
    //last update
    long lu = atol(lus);
    free(lus);
    //now
    long n = time(NULL);
    DtwStringArray *ds = dtw_list_dirs(pp,1);
    if(n - lu > to){

        //dirs of pp , the git its the first 
       
    
        //comand line pull
        char cmdp[1000] = {0};
        sprintf(cmdp,"cd %s && git pull  --quiet",ds->strings[0]);

        int error =  system(cmdp);
        if(error != 0){
            printf("Error pulling the repo\n");
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
            char *cf = afs->strings[i];
            
            //content
            uint8_t lmfs[20] = {0};
            long lmf = dtw_get_entity_last_motification_in_unix(cf);
            sprintf(lmfs,"%ld",lmf);
            if(!ahs ){
                ass = true;
                calc_sha_256(ahs,lmfs,strlen(lmfs));
            }else{

                ///current hash
                uint8_t ch[32] = {0};
                calc_sha_256(ch,lmfs,strlen(lmfs));
                ///join hash
                uint8_t jh[65] = {0};
                memcpy(jh,ahs,32);
                memcpy(jh+32,ch,32);
                calc_sha_256(ahs,jh,64);
            
            }

        }
        DtwStringArray_free(afs);

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
    
    DtwStringArray *afs = dtw_list_files_recursively(pp,1);
    ///filter elements that not inside .it
    DtwStringArray *ffs = newDtwStringArray();

    // filter only emenets that does not belong to .git
    char isgit[1000] = {0};
    sprintf(isgit,"%s.git/",ds->strings[0]);
    DtwStringArray_free(ds);
    int isgits = strlen(isgit);
    for(int i = 0; i < afs->size;i++){
        //heck if afs->strings[i] is inside isgit
        for(int j=0;j < isgits;j++){
            if(afs->strings[i][j] != isgit[j]){
                long s;
                bool ib;
                unsigned char *c = dtw_load_any_content(afs->strings[i],&s,&ib);
                if(!ib){
                    DtwStringArray_append(ffs,afs->strings[i]);
                }
                free(c);
                break;
            }
        }
    }
    ////tendencie struct
    DtwStringArray_free(afs);


    //darw a element betwein 0 and afs->size
     srand( time(NULL) +43434299);
    int r = rand() % ffs->size;
    //loaded file
    char *lf = dtw_load_string_file_content(ffs->strings[r]);
    system("clear");
    printf("====================MAKE YOUR HOLLY CODE REVIEW====================\n");
    printf("File: %s\n",ffs->strings[r]+ strlen(pp) + 1);
    printf("====================================================================\n");
    int lfs = strlen(lf);
    int tl = 0;

    int i = 0;
    if(ll < lfs){
        while (true){
            i  = rand() % lfs;
            if (lfs - i > ll){
                break;
            }
        }        
    }
    else{
        i = 0;
    }

    for(; i < lfs;i++){
        printf("%c",lf[i]);
        if(lf[i] == '\n'){
            tl++;
        }
        if(tl > ll){
           break;
        }
    }
    free(lf);
    DtwStringArray_free(ffs);

    return 0;
    //comand 3
    
}