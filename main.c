#include<stdlib.h>
#include <openssl/md5.h>
#include <stdio.h>
#include <string.h>

typedef unsigned char byte;

void hash_MD5(const char* s, byte* empreinte)
{
    MD5((unsigned char*)s, strlen(s), empreinte);
}

void hash_SHA1(const char* s, byte* empreinte)
{
    SHA1((unsigned char*)s, strlen(s), empreinte);
}

//function main to create MD5 hash
int main(int argc, char *argv[]) {
    char* test = "Salut";
    hash_MD5(test, char name[MD5_DIGEST_LENGTH]);");
    printf(test);
    return 0;
}