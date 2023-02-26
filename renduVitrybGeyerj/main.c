#include<stdlib.h>
#include<stdio.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <getopt.h>

typedef unsigned char byte;

char* alphabet = "";
int alphabet_set = 0;
int alphabet_size;
int min_size = -1;
int max_size = -1;
int64_t N;
int i2c_called = 0;
int i2c_num;
char* texte_clair_i2i = "";
unsigned char* md5_res = "";
byte md5_em[MD5_DIGEST_LENGTH];
unsigned char* sha1_res = "";
byte sha1_em[SHA_DIGEST_LENGTH];
int h2i_called = 0;
int i2i_called = 0;
int new_chain_length = 0;

void hash_MD5(char* s, byte* empreinte)
{
    MD5((unsigned char*)s, strlen(s), empreinte);
}

void hash_SHA1(const char* s, byte* empreinte)
{
    SHA1((unsigned char*)s, strlen(s), empreinte);
}

void hash_MD5_e(char* s)
{
    byte empreinte[MD5_DIGEST_LENGTH];
    MD5((unsigned char*)s, strlen(s), empreinte);
}

void hash_SHA1_e(char* s)
{
    byte empreinte[SHA_DIGEST_LENGTH];
    SHA1((unsigned char*)s, strlen(s), empreinte);
}

void bin_to_char(unsigned char* tmp, byte* em, int length){
    char* p = tmp;
    for(int i = 0; i < length ; i++ ){
        sprintf(p, "%0x", em[i]);
        p += 2;
    }
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

uint64_t h2i(unsigned char* y, int t) {
    unsigned char y_8[8];
    sscanf(y, "%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx", &y_8[0], &y_8[1], &y_8[2], &y_8[3], &y_8[4], &y_8[5], &y_8[6], &y_8[7]);
    uint64_t y_64 = *((uint64_t*) y_8);
    return (y_64 + t) % N;
}

uint64_t i2i(uint64_t idx, int t){
    // calcul message clair : indice -> clair (i2c)
    texte_clair_i2i = i2c(idx);
    printf("--i2c--> %s \n", texte_clair_i2i);
    
    // calcul empreinte : clair -> empreinte (hash)
    byte empreinte[MD5_DIGEST_LENGTH];
    hash_MD5(texte_clair_i2i, empreinte);
    printf("--h--> ");
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++ ){
        printf("%0x ", empreinte[i]);
    }
    printf("\n");
    bin_to_char(texte_clair_i2i, empreinte, MD5_DIGEST_LENGTH);
    free(texte_clair_i2i);

    // calcul indice : empreinte -> indice (h2i)
    printf("t -> %ld\n", t);
    uint64_t indice = h2i(empreinte, t);
    printf("--h2i-->%ld\n", indice);
    
    return indice;
}

uint64_t nouvelle_chaine(uint64_t idx1, int largeur){
    /*Calcul une chaîne de h et d'indices de longueur largeur, renvoie un tableau contenant tous les indices*/
    uint64_t res = idx1;

    for (int i = 1; i < largeur; i++)
    {
        res = i2i(res, i);
    }

    return res;
}


void init(){
    N = 0;
    int size_alph = strlen(alphabet);
    for(int i = min_size; i <= max_size; i++){
        N += pow(size_alph, i);
    }
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
        {"h2i", no_argument, 0, 'y'},
        {"nouv-chaine", required_argument, 0, 'z'},
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
                md5_res = optarg;
                printf("MD5 : %s\n", md5_res);
                test_md5(md5_res);
                break;
            case 's' :
                sha1_res = optarg;
                printf("SHA1 : %s\n", sha1_res);
                test_sha1(sha1_res);
                break;
            case 'x' :
                i2c_called = 1;
                i2c_num = atoi(optarg);
                break;
            case 'y' :
                h2i_called = 1;
                break;
            case 'z' :
                new_chain_length = atoi(optarg);
                i2i_called = 1;
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

    if(strlen(md5_res) != 0 && h2i_called){
        unsigned char tmp[MD5_DIGEST_LENGTH * 2];
        hash_MD5(md5_res, md5_em);
        bin_to_char(tmp, md5_em, MD5_DIGEST_LENGTH);
        printf("H2I '%s' --> %ld\n", tmp, h2i(tmp, 1));
    }
    else if(strlen(sha1_res) != 0 && h2i_called){
        unsigned char tmp[SHA_DIGEST_LENGTH * 2];
        hash_MD5(sha1_res, sha1_em);
        bin_to_char(tmp, sha1_em, SHA_DIGEST_LENGTH);
        printf("H2I '%s' --> %ld\n", tmp, h2i(tmp, 1));
    }

    if(i2i_called){
        printf("Chaine de taille %ld --> %ld\n", new_chain_length, nouvelle_chaine(1, new_chain_length));
    }
}