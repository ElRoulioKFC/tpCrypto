#include<stdlib.h>
#include<stdio.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <getopt.h>

char* alphabet = "";
int alphabet_set = 0;
int min_size = -1;
int max_size = -1;
int64_t N;

void init(){
    N = 0;
    int size_alph = strlen(alphabet);
    for(int i = min_size; i <= max_size; i++){
        N += pow(size_alph, i);
    }
    // printf("%ld", N);
    // printf("\n");
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
    int opt;
    int long_index = 0;

    //Specifying the expected options
    static struct option long_options[] = {
        {"alphabet", required_argument, 0, 'a'},
        {"min-size", required_argument, 0, 'b'},
        {"max-size", required_argument, 0, 'h'},
        {"md5", no_argument, 0, 'm'},
        {"sha1", no_argument, 0, 's'},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv,"bhams", long_options, &long_index )) != -1)    { 
        switch(opt) 
        { 
            case 'b':
                min_size = atoi(optarg);
                break;
            case 'h':
                max_size = atoi(optarg);
                break;
            case 'a':
                alphabet = malloc(sizeof(char) * strlen(optarg));
                alphabet = optarg;
                alphabet_set = 1;
                break;
            case 'm' :
                printf("test MD5 : Salut\n");
                test_md5("Salut");
                break;
            case 's' :
                printf("test SHA1 : Bob\n");
                test_sha1("Bob");
                break;
        } 
    }

    if(min_size != -1 && max_size != -1 && alphabet_set){
        init();
        printf("%ld\n", N);
    }
}