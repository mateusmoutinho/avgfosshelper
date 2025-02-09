

typedef struct DtwStringArray {
    int size;
    char **strings;
}DtwStringArray;


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


char * calc_sha_256_returning_string(const void *input, size_t len);

void calc_sha_256_from_string(uint8_t hash[SIZE_OF_SHA_256_HASH], const char *input);

int calc_sha_256_from_file(uint8_t hash[SIZE_OF_SHA_256_HASH], const char *filename);
