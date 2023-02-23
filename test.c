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
char* empreinte = "";
char* texte = "";

void  append(char*s, char c) {
     int len = strlen(s);
     s[len] = c;
     s[len+1] = '\0';
}

char* i2c_naif(int64_t i, int size) {
    // init
    char* result = malloc(sizeof(char) * size);
    int64_t idx = i;
    
    for(int f = 0; f < size; f++){
        result[f] = alphabet[idx % alphabet_size];
        idx = idx / alphabet_size;
    }

    // Inverse la chaîne de caractères
    for (int j = 0; j < size / 2; j++) {
        char temp = result[j];
        result[j] = result[size - j - 1];
        result[size - j - 1] = temp;
    }
    return result;
}

//integer to string from alphabet
char* i2c(int64_t value){
    int size = min_size;
    int64_t tmp_val = value;

    while ((tmp_val >= pow(alphabet_size, size)) && (size < max_size))
    {
        tmp_val -= pow(alphabet_size, size);
        size++;
    }

    printf("Detected size : %d\n", size);
    return i2c_naif(tmp_val, size);
}

/*
uint64_t h2i(unsigned char *hash) {
    uint64_t num = 0;
    memcpy(&num, hash, 8);
    return num % N;
}*/

uint64_t h2i(unsigned char* hash, int column) {
    printf("HASHHHHHH --> %s\n", hash);
    uint64_t* val = (uint64_t*) hash;
    return *val;
}

// uint64_t h2i(char* t, int column) {
//     uint64_t i = *((uint64_t*) t);
//     printf("n = %d", N);
//     return (i + column) % N;
// }

/*
char* i2i(uint64_t idx, int t){
    char* clair = i2c(idx);
    hash_MD5(clair, idx);
    uint64_t indice = h2i(empreinte, t)
    
    return indice;
}*/

/*
char* nouvelle_chaine(uint64_t idx1, uint64_t largeur){
    return "test";
}*/

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

char* test_md5(char* testString){
    byte empreinte[MD5_DIGEST_LENGTH];
    hash_MD5(testString, empreinte);
    char* res = malloc(sizeof(char) * 10000);
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++ ){
        sprintf("%0x", empreinte[i]);
        res[i] = empreinte[i];
    }
    printf("\n");
    return res;
}


char* test_sha1(char* testString){
    byte empreinte[SHA_DIGEST_LENGTH];
    hash_SHA1(testString, empreinte);
    char* res = "";
    for(int i = 0; i < SHA_DIGEST_LENGTH ; i++ ){
        printf("%0x", empreinte[i]);
        res += sprintf("%0x", empreinte[i]);
    }
    printf("\n");
    return res;
}


//function main to create MD5 hash
int main(int argc, char *argv[]) {
    int opt;
    int long_index = 0;
    int i2c_num = 0;

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
            char* i2cResult = i2c(i2c_num);
            printf("\nI2C --> %s\n", i2cResult);
        }
    }
    N = 12338352;
    printf("=== %s ===\n", test_md5("oups"));
    uint64_t h = h2i(test_md5("oups"), 1);
    printf("val --> %ld\n", h);
}