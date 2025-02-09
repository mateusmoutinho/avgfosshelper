#include "dtw.h"

int dtw_entity_type(const char *path){
    //returns 1 for file, 2 for directory, -1 for not found
    struct stat path_stat;

    if(stat(path,&path_stat) == 0){
        if(S_ISREG(path_stat.st_mode)){
            return DTW_FILE_TYPE;
        }else if(S_ISDIR(path_stat.st_mode)){
            return DTW_FOLDER_TYPE;
        }
    }
    return DTW_NOT_FOUND;

}

void dtw_create_dir_recursively(const char *path){

    int entity =dtw_entity_type(path);

    if(entity == DTW_FOLDER_TYPE){
        return;
    }

    if(entity == DTW_FILE_TYPE){
        remove(path);
    }


    long size_path = strlen(path);
    for(int i=0;i <  size_path;i++){
        if((path[i] == '\\'  || path[i] == '/' )  &&( i != size_path - 1)){

            char * current_path = (char*)malloc(i + 1);
            current_path[i] = '\0';
            strncpy(current_path,path,i);

            if(dtw_entity_type(current_path) == DTW_FILE_TYPE){
                remove(current_path);
            }

            dtw_create_dir(current_path);

            free(current_path);
        }
    }

    dtw_create_dir(path);
}