#include<stdlib.h>
#include <openssl/md5.h>

typedef unsigned char byte;

void hash_MD5(const char* s, byte* empreinte)
{
    MD5((unsigned char*)s, strlen(s), empreinte);
}

//function main to create MD5 hash
int main(int argc, char *argv[]) {
    char* test = "Salut";
    hash_MD5(test, "6186484864161613");
    printf(test);
}