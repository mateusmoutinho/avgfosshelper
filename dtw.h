#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#define DTW_FILE_TYPE 1
#define DTW_FOLDER_TYPE 2
#define DTW_ALL_TYPE 3
#define DTW_NOT_FOUND -1
#define DTW_CONCAT_PATH true
#define DTW_NOT_CONCAT_PATH false
#define WIN32_FILETYPE 32

typedef struct DtwStringArray {
    int size;
    char **strings;
}DtwStringArray;

#ifdef __linux__
#define dtw_create_dir(path) mkdir(path,0777)
#elif _WIN32
#define dtw_create_dir(path) _mkdir(path)

#endif
unsigned char *dtw_load_any_content(const char * path,long *size,bool *is_binary);

bool dtw_write_any_content(const char *path,unsigned  char *content,long size);

bool dtw_write_string_file_content(const char *path,const char *content);

char *dtw_load_string_file_content(const char * path);

// End the structure with a semicolon
int  DtwStringArray_find_position( DtwStringArray *self, const char *string);


void DtwStringArray_append_getting_ownership( DtwStringArray *self, char *string);

void DtwStringArray_append( DtwStringArray *self, const char *string);

void DtwStringArray_pop( DtwStringArray *self, int position);

void DtwStringArray_merge( DtwStringArray *self,  DtwStringArray *other);
void DtwStringArray_represent( DtwStringArray *self);
void DtwStringArray_free( DtwStringArray *self);

int private_dtw_string_cmp(const void *a, const void *b);
void DtwStringArray_sort( DtwStringArray *self);

void DtwStringArray_set_value( DtwStringArray *self, int index, const char *value);

 DtwStringArray * newDtwStringArray();

char * privateDtwStringArray_append_if_not_included(DtwStringArray *self,char *value);

DtwStringArray * DtwStringArray_clone(DtwStringArray *self);


DtwStringArray * dtw_list_files(const char *path, bool concat_path);
DtwStringArray * dtw_list_dirs(const char *path, bool concat_path);

DtwStringArray *  dtw_list_all(const char *path,  bool concat_path);

#ifdef __linux__

bool private_dtw_verify_if_add(const int expected_type, int d_type);
bool private_dtw_verify_if_skip(struct dirent *entry);

#endif


#ifdef _WIN32

bool private_dtw_verify_if_add(const int expected_type, WIN32_FIND_DATAA entry);
bool private_dtw_verify_if_skip(WIN32_FIND_DATAA *entry);
#endif



struct DtwStringArray * dtw_list_basic(const char *path,int expected_type,bool concat_path);


 DtwStringArray * dtw_list_dirs_recursively(const char *path,bool concat_path);


 DtwStringArray *  dtw_list_files_recursively(const char *path,bool concat_path);


 DtwStringArray * dtw_list_all_recursively(const char *path,bool concat_path);

void dtw_create_dir_recursively(const char *path);


int dtw_entity_type(const char *path);

bool dtw_remove_any(const char* path);

double private_dtw_convert_string_to_number(const char *num, bool *its_a_number);

long int dtw_get_entity_last_motification_in_unix(const char *path);
