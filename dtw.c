#include "dtw.h"


bool dtw_write_any_content(const char *path,unsigned  char *content,long size){
    //Iterate through the path and create directories if they don't exist
    int entity_type =dtw_entity_type(path);
    if(entity_type == DTW_FOLDER_TYPE){
        dtw_remove_any(path);
    }


    if(entity_type == DTW_NOT_FOUND){
        long path_size = (long)strlen(path);
        for(long i = path_size-1;i > 0;i--){
            //runs in negative mode til / or \ is found
            if(path[i] == '\\' || path[i] == '/'){
                char *dir_path =(char*)malloc(i +2);
                dir_path[i] = '\0';
                strncpy(dir_path,path,i);

                dtw_create_dir_recursively(dir_path);
                free(dir_path);

                break;
            }
        }
    }

    FILE *file = fopen(path,"wb");
    if(file == NULL){

        return false;
    }

    fwrite(content, sizeof(char),size, file);

    fclose(file);
    return true;
}

char *dtw_load_string_file_content(const char * path){
    long size;
    bool is_binary;
    unsigned char *element = dtw_load_any_content(path,&size,&is_binary);
    if(element == NULL){
        return NULL;
    }

    if(is_binary){
        free(element);
        return NULL;
    }
    return (char*)element;
}

bool dtw_write_string_file_content(const char *path,const char *content){
    long size;
    if(content == NULL){
        size = 0;
    }
    else{
        size = (long)strlen(content);
    }
    return dtw_write_any_content(path,(unsigned char*)content,size);
}

unsigned char *dtw_load_any_content(const char * path,long *size,bool *is_binary){

    *is_binary = false;
    *size = 0;

    int entity = dtw_entity_type(path);
    if(entity != DTW_FILE_TYPE){
        return NULL;
    }
    FILE  *file = fopen(path,"rb");

    if(file ==NULL){
        return NULL;
    }


    if(fseek(file,0,SEEK_END) == -1){
        fclose(file);
        return NULL;
    }


    *size = ftell(file);

    if(*size == -1){
        fclose(file);
        return NULL;
    }

    if(*size == 0){
        fclose(file);
        return (unsigned char*)strdup("");
    }


    if(fseek(file,0,SEEK_SET) == -1){
        fclose(file);
        return NULL;
    }

    unsigned char *content = (unsigned char*)malloc(*size +1);
    int bytes_read = fread(content,1,*size,file);
    if(bytes_read <=0 ){
        free(content);
        fclose(file);
        return NULL;
    }


    *is_binary = false;
    for(int i = 0;i < *size;i++){
        if(content[i] == 0){
            *is_binary = true;
            break;
        }
    }
    content[*size] = '\0';

    fclose(file);
    return content;
}



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
char *private_dtw_replace_string_once(const char *target, const char *old_element, const char *new_element) {

    const char *pos = strstr(target, old_element);

    int size_of_old_element = (int)strlen(old_element);
    int size_of_new_element = (int)strlen(new_element);
    // Allocate memory for the new string
    char *result = (char *)malloc(strlen(target) + size_of_new_element - size_of_old_element + 1);

    // Copy the part of the original string before the old substring
    strncpy(result, target, pos - target);

    // Copy the new substring to the result string
    strcpy(result + (pos - target), new_element);

    // Copy the rest of the original string after the old substring
    strcpy(result + (pos - target) + size_of_new_element, pos + size_of_old_element);

    return result;



}

char* dtw_replace_string(const char *target, const char *old_element, const char *new_element) {
    char *result = (char *)malloc(strlen(target) + 1);
    strcpy(result, target);
    char *temp = NULL;
    while (strstr(result, old_element) != NULL) {
        temp = private_dtw_replace_string_once(result, old_element, new_element);
        free(result);
        result = temp;
    }
    return result;
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

bool dtw_remove_any(const char* path) {
    //means is an file
    if(remove(path) == 0){
        return true;
    }

    struct DtwStringArray *files = dtw_list_files_recursively(path,DTW_CONCAT_PATH);
    int files_size = files->size;
    for(int i = 0; i < files_size; i++){
        remove(files->strings[i]);
    }
    DtwStringArray_free(files);


    struct DtwStringArray *dirs = dtw_list_dirs_recursively(path,DTW_CONCAT_PATH);
    int dirs_size = dirs->size;
    for(int i = dirs->size -1; i >=0; i--){
        rmdir(dirs->strings[i]);
    }
    DtwStringArray_free(dirs);
    //remove / to the path
    if(files_size ||dirs_size){
        return true;
    }
    return false;


}



struct DtwStringArray * newDtwStringArray(){
    struct DtwStringArray *self = ( DtwStringArray*)malloc(sizeof(struct DtwStringArray));
    self->size = 0;

    self->strings = (char**)malloc(1);

    return self;
}

int DtwStringArray_find_position( DtwStringArray *self, const char *string){
    for(int i = 0; i < self->size; i++){
        if(strcmp(self->strings[i], string) == 0){
            return i;
        }
    }
    return -1;
}


void DtwStringArray_set_value( DtwStringArray *self, int index, const char *value){
    if(index < self->size && index >= 0){
        int size = strlen(value);
        self->strings[index] = (char*)realloc(self->strings[index], size + 1);
        self->strings[index][size] = '\0';
        strcpy(self->strings[index], value);
    }
}
void DtwStringArray_append_getting_ownership( DtwStringArray *self, char *string){
    self->strings =  (char**)realloc(self->strings, (self->size+ 1) * sizeof(char*));
    self->strings[self->size] = string;
    self->size+=1;
}

// Function prototypes
void DtwStringArray_append( DtwStringArray *self, const  char *string){

    self->strings =  (char**)realloc(self->strings, (self->size+ 1) * sizeof(char*));
    self->strings[self->size] = strdup(string);
    self->size+=1;
}

void DtwStringArray_pop( DtwStringArray *self, int position){
    free(self->strings[position]);
    for(int i = position; i < self->size -1; i++){
        self->strings[i] = self->strings[i+1];
    }
    self->size-=1;
}

void DtwStringArray_merge( DtwStringArray *self,  DtwStringArray *other){
    for(int i = 0; i < other->size; i++){
        DtwStringArray_append(self, other->strings[i]);
    }
}


void DtwStringArray_represent( DtwStringArray *self){
    for(int i = 0; i < self->size; i++){
        printf("%s\n", self->strings[i]);
    }
}

int private_dtw_string_cmp(const void *a, const void *b){

    bool a_its_number;

    double a_num_value = private_dtw_convert_string_to_number(*(const char **)a, &a_its_number);

    if(a_its_number){
        bool b_its_number;

        double b_num_value = private_dtw_convert_string_to_number(*(const char **)b, &b_its_number);

        if(b_its_number){


            return a_num_value - b_num_value;
        }
    }

    const char *str_a = *(const char **)a;
    const char *str_b = *(const char **)b;
    return strcmp(str_a, str_b);
}

void DtwStringArray_sort(struct DtwStringArray *self) {

    qsort(self->strings, self->size, sizeof(char*), private_dtw_string_cmp);
}

 DtwStringArray * DtwStringArray_clone(DtwStringArray *self){
    DtwStringArray  *clone = newDtwStringArray();
    for(int i = 0; i< self->size; i++){
        DtwStringArray_append(clone,self->strings[i]);
    }
    return clone;
}

char * privateDtwStringArray_append_if_not_included(DtwStringArray *self,char *value){
    long position=DtwStringArray_find_position(self,value);
    if(position != -1){
        free(value);
        return self->strings[position];
    }
    DtwStringArray_append_getting_ownership(self,value);
    return value;
}

void DtwStringArray_free(struct DtwStringArray *self){
    for(int i = 0; i < self->size; i++){
            free(self->strings[i]);
    }

    free(self->strings);
    free(self);
}

double private_dtw_convert_string_to_number(const char *num, bool *its_a_number){

    long size_num = strlen(num);

    if(size_num == 0){
        *its_a_number = false;
        return -1;
    }

    bool dot_found = false;

    for(int i = 0; i < size_num; i++){
        char current = num[i];

        if(current == '.'){
            if(i == 0 || dot_found || i == size_num - 1){
                *its_a_number = false;
                return -1;
            }

            dot_found = true;
            continue;
        }

        if(current == '0'){
            continue;
        }

        if(current == '1'){
            continue;
        }

        if(current == '2'){
            continue;
        }

        if(current == '3'){
            continue;
        }

        if(current == '4'){
            continue;
        }

        if(current == '5'){
            continue;
        }

        if(current == '6'){
            continue;
        }

        if(current == '7'){
            continue;
        }

        if(current == '8'){
            continue;
        }

        if(current == '9'){
            continue;
        }

        *its_a_number = false;

        return -1;
    }

    *its_a_number = true;

    return atof(num);


}

char *private_dtw_format_path(const char *path){
    int path_size = (int)strlen(path);
    char *buffer = (char*)malloc((path_size+2) * sizeof(char));
    int buffer_size = 0;
    bool already_concatened = false;
    for(long i = 0; i < path_size;i++){
        char current_char = path[i];

        bool is_a_divisor =current_char == '\\' || current_char == '/';

        if(is_a_divisor && already_concatened){
            continue;
        }
        if(is_a_divisor){
#ifdef __linux
            buffer[buffer_size] = '/';
#else
            buffer[buffer_size] = '\\';
#endif
            buffer_size+=1;
            already_concatened = true;
            continue;
        }

        already_concatened = false;
        buffer[buffer_size] = current_char;
        buffer_size+=1;

    }
    buffer[buffer_size] = '\0';
    return buffer;
}

void private_dtw_remove_double_bars_from_string_array(struct DtwStringArray*path){
    for(int i =0;i< path->size;i++){
        char *buffer = private_dtw_format_path(path->strings[i]);
        free(path->strings[i]);
        path->strings[i] = buffer;
    }
}

bool dtw_ends_with(const char *string, const char *suffix){
    if(strlen(string) < strlen(suffix)){
        return false;
    }
    if(strcmp(string + strlen(string) - strlen(suffix), suffix) == 0){
        return true;
    }
    return false;
}

void private_dtw_add_end_bar_to_dirs_string_array(struct DtwStringArray * dirs){
    for(int i = 0; i < dirs->size; i++){

        if(!dtw_ends_with(dirs->strings[i], "/") || !dtw_ends_with(dirs->strings[i],"\\")){
            char *formated_dir =  (char*)malloc(strlen(dirs->strings[i]) + 3);
            sprintf(formated_dir,"%s/",dirs->strings[i]);
            DtwStringArray_set_value(dirs, i, formated_dir);
            free(formated_dir);
        }


    }
}


struct DtwStringArray* private_dtw_remove_start_path(struct DtwStringArray *paths,const char *rm_path){

    char *path_to_remove = dtw_replace_string(rm_path,"//","/");

    int size_to_remove = strlen(path_to_remove) +1;

    if(dtw_ends_with(path_to_remove,"/")){
        size_to_remove-=1;
    }


    struct DtwStringArray *new_array = newDtwStringArray();

    for(int i =0; i < paths->size; i++){

        char *current_path_string = paths->strings[i];
        int current_path_string_size = strlen(current_path_string);

        char *new_string = (char*)malloc(current_path_string_size +2);
        new_string[current_path_string_size] =0;

        strcpy(new_string,current_path_string);
        memmove(
                new_string,
                current_path_string+size_to_remove,
                strlen(current_path_string) - size_to_remove +1
        );
        if(strcmp(new_string,"/") == 0  || strcmp(new_string,"") == 0){
            free(new_string);
            continue;
        }

        DtwStringArray_append(new_array, new_string);
        free(new_string);

    }
    free(path_to_remove);
    return new_array;
}




#ifdef __linux__

bool private_dtw_verify_if_add(const int expected_type, int d_type){
    if (expected_type == DTW_FILE_TYPE  && d_type == DT_REG) {
        return true;
    }

    if (expected_type == DTW_FOLDER_TYPE && d_type == DT_DIR) {
        return true;
    }

    if (expected_type == DTW_ALL_TYPE) {

        return true;
    }
    return false;
}
bool private_dtw_verify_if_skip(struct dirent *entry){
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            return true;
        }
        return false;
}

struct DtwStringArray * dtw_list_basic(const char *path,int expected_type,bool concat_path){

    DIR *dir;
    struct dirent *entry;

    //array of directories
    struct DtwStringArray *dirs = newDtwStringArray();
    int i = 0;

    //means that the directory is not found
    if ((dir = opendir(path)) == NULL) {
        return dirs;
    }

    //reads the directory and adds the directories to the array
    while ((entry = readdir(dir)) != NULL) {
        //means is not a directory
        if (private_dtw_verify_if_skip(entry)){
            continue;
        }

        if (private_dtw_verify_if_add(expected_type,entry->d_type)) {


            if(concat_path){
                //allocates memory for the directory
                char *generated_dir = (char*)malloc(strlen(path) + strlen(entry->d_name) + 2);
                if(path[strlen(path) - 1] == '/'){
                    sprintf(generated_dir, "%s%s", path, entry->d_name);
                }
                else{
                    sprintf(generated_dir, "%s/%s", path, entry->d_name);
                }

                DtwStringArray_append(dirs, generated_dir);
                free(generated_dir);
            }
            else{
                DtwStringArray_append(dirs, entry->d_name);

            }

            i++;
        }
    }

    if(expected_type == DTW_FOLDER_TYPE){
        private_dtw_add_end_bar_to_dirs_string_array(dirs);
    }
    closedir(dir);

    return dirs;
}

#endif


#ifdef _WIN32


bool private_dtw_verify_if_add(const int expected_type, WIN32_FIND_DATAA entry){

    if (expected_type == DTW_FILE_TYPE && entry.dwFileAttributes == WIN32_FILETYPE) {
        return true;
    }

    if (expected_type == DTW_FOLDER_TYPE && entry.dwFileAttributes != WIN32_FILETYPE){
        return true;
    }

    if (expected_type == DTW_ALL_TYPE) {
        return true;
    }

    return false;
}

bool private_dtw_verify_if_skip(WIN32_FIND_DATAA *entry){
    if (strcmp(entry->cFileName, ".") == 0 || strcmp(entry->cFileName, "..") == 0) {
        return true;
    }
    return false;
}

struct DtwStringArray *  dtw_list_basic(const char *path,int expected_type,bool concat_path){

    WIN32_FIND_DATAA file_data;
    HANDLE file_handle;
    char search_path[MAX_PATH];

    //array of directories
    struct DtwStringArray *dirs = newDtwStringArray();
    int i = 0;

    // build the search path string
    snprintf(search_path, MAX_PATH, "%s\\*", path);

    // try to open the directory
    if ((file_handle = FindFirstFileA(search_path, &file_data)) == INVALID_HANDLE_VALUE) {
        return dirs;
    }

    do {
        // skip "." and ".." directories
        if (private_dtw_verify_if_skip(&file_data)){
            continue;
        }

        // verify if it's a file or directory
        if (private_dtw_verify_if_add(expected_type, file_data)) {

            if(concat_path){
                // allocate memory for the directory
                if(path[strlen(path) - 1] == '\\' || path[strlen(path) - 1] == '/'){
                    char *generated_dir = (char*)malloc(strlen(path) + strlen(file_data.cFileName) + 1);
                    sprintf(generated_dir, "%s%s", path, file_data.cFileName);
                    DtwStringArray_append(dirs, generated_dir);
                    free(generated_dir);
                }
                else{
                    char *generated_dir = (char*)malloc(strlen(path) + strlen(file_data.cFileName) + 2);


                    sprintf(generated_dir, "%s/%s", path, file_data.cFileName);

                    DtwStringArray_append(dirs, generated_dir);
                    free(generated_dir);
                }


            }
            else{
                DtwStringArray_append(dirs, file_data.cFileName);

            }

            i++;
        }
    } while (FindNextFileA(file_handle, &file_data) != 0);

        if(expected_type == DTW_FOLDER_TYPE){
            private_dtw_add_end_bar_to_dirs_string_array(dirs);
        }

    FindClose(file_handle);

    return dirs;
}
#endif



 DtwStringArray * dtw_list_dirs_recursively(const char *path,bool concat_path){

        struct  DtwStringArray *dirs  = newDtwStringArray();
        //verify if the path is a directory

        int entity_type = dtw_entity_type(path);
        if(entity_type != DTW_FOLDER_TYPE){
                return dirs;
        }


        DtwStringArray_append(dirs, (char*)path);

        private_dtw_add_end_bar_to_dirs_string_array(dirs);
        int i = 0;
        //The size of dirs will increase til it reaches the end of the array
        while(i < dirs->size){
                struct DtwStringArray *sub_dirs = dtw_list_basic(
                    dirs->strings[i],
                    DTW_FOLDER_TYPE,
                    true
                    );
                //merge the two dirs
            DtwStringArray_merge(dirs, sub_dirs);
                DtwStringArray_free(sub_dirs);
                i++;

        }
        //unsifth path in dirs
     private_dtw_remove_double_bars_from_string_array(dirs);

        if(!concat_path){

            struct DtwStringArray *removed =  private_dtw_remove_start_path(dirs,path);
            DtwStringArray_free(dirs);
            return removed;
        }
        return dirs;
}



 DtwStringArray *  dtw_list_files_recursively(const char *path,bool concat_path){

    struct DtwStringArray *dirs = dtw_list_dirs_recursively(path,DTW_CONCAT_PATH);

    struct  DtwStringArray *files = newDtwStringArray();
    for(int i = 0; i < dirs->size; i++){
        struct DtwStringArray *sub_files = dtw_list_basic(dirs->strings[i],DTW_FILE_TYPE,DTW_CONCAT_PATH);
        DtwStringArray_merge(files, sub_files);
        DtwStringArray_free(sub_files);
    }

    DtwStringArray_free(dirs);

    if(!concat_path){

        struct DtwStringArray *removed =  private_dtw_remove_start_path(files,path);
        DtwStringArray_free(files);
        return removed;
    }

    return files;
}




 DtwStringArray * dtw_list_all_recursively(const char *path,bool concat_path){

    struct DtwStringArray *dirs = dtw_list_dirs_recursively(path,DTW_CONCAT_PATH);

    struct DtwStringArray *all = newDtwStringArray();

    for(int i = 0; i < dirs->size; i++){

        if(!dtw_ends_with(dirs->strings[i], "/") || !dtw_ends_with(dirs->strings[i], "\\") ){

            char *formated_dir =  (char*)malloc(strlen(dirs->strings[i]) + 2);
            sprintf(formated_dir,"%s/",dirs->strings[i]);
            DtwStringArray_append(all, formated_dir);
            free(formated_dir);
        }

        else{
            DtwStringArray_append(all, dirs->strings[i]);
        }

        struct DtwStringArray *sub_files = dtw_list_basic(dirs->strings[i],DTW_FILE_TYPE,true);
        DtwStringArray_merge(all,sub_files);
        DtwStringArray_free(sub_files);
    }
    DtwStringArray_free(dirs);
     private_dtw_remove_double_bars_from_string_array(all);
    if(!concat_path){

        struct DtwStringArray *removed =  private_dtw_remove_start_path(all,path);
        DtwStringArray_free(all);
        return removed;
    }
    return all;
}
