#include <stdio.h>
#include <stdlib.h>
#include <openssl/md5.h>

FILE* openFile(char *filename){
    FILE *fp;
    fp = fopen(filename, "rb");
    if(fp == NULL){
        printf("Error while opening file %s\n", filename);
        exit(1);
    }
    return fp;
}

int getFileSize(FILE* f){
    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    fseek(f, 0, SEEK_SET);
    return size;
}

// Gera o MD5 de um arquivo
unsigned char* getMD5(FILE* fp) {
    int size = getFileSize(fp);
    char* buffer = (char*)malloc(size);
    fread(buffer, size, 1, fp);

    const unsigned char* data = (const unsigned char*)buffer;

    MD5_CTX mdContext;
    MD5_Init(&mdContext);
    MD5_Update(&mdContext, data, size);

    unsigned char* result = (unsigned char*)malloc(MD5_DIGEST_LENGTH);
    MD5_Final(result, &mdContext);

    return result;
}

int compareMD5(unsigned char* md5_1, unsigned char* md5_2){
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++){
        if(md5_1[i] != md5_2[i]){
            return 0;
        }
    }
    return 1;
}


int main(){
    FILE *fp = openFile("capi_estatistica.jpg");
    FILE *fp2 = openFile("capi_psicologia.jpg");

    unsigned char *result = getMD5(fp);
    unsigned char *result2 = getMD5(fp2);

    printf("MD5 FILE 1:");
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++){
        printf("%02x", result[i]);
    }
    printf("\n");
    
    printf("MD5 FILE 2:");
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++){
        printf("%02x", result2[i]);
    }

    if(compareMD5(result, result2)){
        printf("\n\nArquivos iguais\n");
    }else{
        printf("\n\nArquivos diferentes\n");
    }
    
    free(result);
    free(result2);
    fclose(fp);
    fclose(fp2);

    return 0;
}