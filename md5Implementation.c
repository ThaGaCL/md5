#include <stdio.h>
#include <stdlib.h>
#include <openssl/md5.h>

FILE* openFile(char *filename){
    FILE *fileClient;
    fileClient = fopen(filename, "rb");
    if(fileClient == NULL){
        printf("Error while opening file %s\n", filename);
        exit(1);
    }
    return fileClient;
}

int getFileSize(FILE* f){
    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    fseek(f, 0, SEEK_SET);
    return size;
}

/// FUNÇÕES DO SERVIDOR ///

int fileExists(char* filename, char* dirname){
    FILE *fileClient;
    fileClient = fopen(filename, "rb");
    if(fileClient == NULL){
        return 0;
    }
    fclose(fileClient);
    return 1;
}


//// FUNÇÕES DE MD5 (COMPARTILHADAS) ///

// Gera o MD5 de um arquivo
unsigned char* getMD5(FILE* fileClient) {
    int size = getFileSize(fileClient);
    char* buffer = (char*)malloc(size);
    fread(buffer, size, 1, fileClient);

    const unsigned char* data = (const unsigned char*)buffer;

    MD5_CTX mdContext;
    MD5_Init(&mdContext);
    MD5_Update(&mdContext, data, size);

    unsigned char* result = (unsigned char*)malloc(MD5_DIGEST_LENGTH);
    MD5_Final(result, &mdContext);

    return result;
}

// Compara dois MD5
int compareMD5(unsigned char* md5_1, unsigned char* md5_2){
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++){
        if(md5_1[i] != md5_2[i]){
            return 0;
        }
    }
    return 1;
}


int main(){
    FILE *fileClient = openFile("capi_filosofa.png"); // Arquivo do cliente
    FILE *fileServer;   // Arquivo do servidor


    // o diretorio deve ser o diretorio de backup do servidor
    // o nome do arquivo deve ser enviado em uma mensagem do cliente
    if(fileExists("capi_filosofa.png", "/server")){
        // mensagem de "arquivo existe"
        fileServer = openFile("./server/capi_filosofa.png");
        
        unsigned char *result = getMD5(fileClient); // Gera o MD5 Do arquivo do cliente
        unsigned char *result2 = getMD5(fileServer); // Gera o MD5 Do arquivo do servidor
        
        if(compareMD5(result, result2)){
            printf("Arquivos iguais\n");
        }else{
            printf("Arquivos diferentes\n");
        }
        
        free(result);
        free(result2);
        fclose(fileServer);
    }else{
        // mensagem de "arquivo nao existe"
        printf("Arquivo nao existe\n");
    }


    fclose(fileClient);

    return 0;
}