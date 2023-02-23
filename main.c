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
int alphabet_size;
int min_size = -1;
int max_size = -1;
int64_t N;
int i2c_called = 0;

char* i2c_naif(int64_t i, int size) {
    char* result = (char*)malloc(sizeof(char) * 1000);
    int idx;
    int pos = 0;
    if (i == 0) {
        result[pos] = alphabet[0];
    }
    while (i > 0 || pos < size) {
        if(i < 0){
            result[pos++] = alphabet[0];
        }
        idx = i % alphabet_size;
        result[pos++] = alphabet[idx];
        i = (i - 1) / alphabet_size;
    }
    result[pos] = '\0';
    int len = strlen(result);
    // Inverse la chaîne de caractères
    for (int j = 0; j < len / 2; j++) {
        char temp = result[j];
        result[j] = result[len - j - 1];
        result[len - j - 1] = temp;
    }
    return result;
}

//integer to string from alphabet
char* i2c(int64_t value){
    char* result;
    int size;

    while ((value % pow(alphabet_size, i)))
    {
        /* code */
    }

    return result;
}

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
        {"md5", required_argument, 0, 'm'},
        {"sha1", required_argument, 0, 's'},
        {"i2c", required_argument, 0, 'x'},
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
                alphabet_size = strlen(alphabet);
                break;
            case 'm' :
                printf("test MD5 : %s\n", optarg);
                test_md5(optarg);
                break;
            case 's' :
                printf("test SHA1 : %s\n", optarg);
                test_sha1(optarg);
                break;
            case 'x' :
                i2c_called = 1;
                i2c_num = atoi(optarg);
                break;
        } 
    }

    if(min_size != -1 && max_size != -1 && alphabet_size){
        init();
        printf("N --> %ld\n", N);
        if(i2c_called){
            printf("\nI2C --> %s\n", i2c_naif(i2c_num, max_size));
        }
    }
}