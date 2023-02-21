#include<stdlib.h>
#include<stdio.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <string.h>
#include <math.h>

char* alphabet;
int min_size;
int max_size;
int N;

void init(char *argv[]){
    alphabet = argv[1];
    min_size = atoi(argv[2]);
    max_size = atoi(argv[3]);
    N = 0;
    int size_alph = strlen(alphabet);
    for(int i = min_size; i <= max_size; i++){
        N = N + pow(size_alph, i);
    }
}

typedef unsigned char byte;
unsigned char *MD5(const unsigned char *d, 
                   unsigned long n,
                   unsigned char *md);

void hash_MD5(char* s, byte* empreinte)
{
    MD5((unsigned char*)s, strlen(s), empreinte);
}

void hash_SHA1(const char* s, byte* empreinte)
{
    SHA1((unsigned char*)s, strlen(s), empreinte);
}

void test_md5(char* testString){
    byte empreinte[MD5_DIGEST_LENGTH];
    hash_MD5(testString, empreinte);
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++ ){
        printf("%0x", empreinte[i]);
    }
    printf("\n");
}


void test_sha1(char* testString){
    byte empreinte[SHA_DIGEST_LENGTH];
    hash_SHA1(testString, empreinte);
    for(int i = 0; i < SHA_DIGEST_LENGTH ; i++ ){
        printf("%0x", empreinte[i]);
    }
    printf("\n");
}


//function main to create MD5 hash
int main(int argc, char *argv[]) {
    init(argv);
    printf("test MD5 : Salut\n");
    test_md5("Salut");
    
    printf("test SHA1 : Bob\n");
    test_sha1("Bob");
}