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

int main(){
    FILE *fp = openFile("capi_estatistica.jpg");
    FILE *fp2 = openFile("capi_psicologia.jpg");

    unsigned char *result = getMD5(fp);

    printf("MD5 FILE 1:");
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++){
        printf("%02x", result[i]);
    }
    printf("\n");
    
    result = getMD5(fp);

    printf("MD5 FILE 1:");
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++){
        printf("%02x", result[i]);
    }

    printf("\n");

    result = getMD5(fp2);

    printf("MD5 FILE 2:");
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++){
        printf("%02x", result[i]);
    }


    
    free(result);
    fclose(fp);
    fclose(fp2);

    return 0;
}