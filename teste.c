#include "DoTheWorld/src/one.c"

int main(){
    char * sha1 = dtw_generate_sha_from_string("eai thales blz fff");
    char *conteudo_do_arquivo = "aaaaa1";
    char *conteudo2 = malloc(1000);
    memcpy(conteudo2,conteudo_do_arquivo,sizeof(conteudo_do_arquivo));
    memcpy(conteudo2+strlen(conteudo_do_arquivo),sha1,strlen(sha1));
    char *sha2 = dtw_generate_sha_from_string(conteudo2);
    printf("%s\n",conteudo2);
}