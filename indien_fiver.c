#include <stdio.h>      // Entrées/sorties standard
#include <stdlib.h>     // Fonctions générales
#include <string.h>     // Fonctions de traitement de chaînes de caractères
#include <ctype.h>      // Fonctions de traitement de caractères
#include <math.h>       // Fonctions mathématiques
#include <stdint.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <getopt.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#define TAILLE_MAX 1000

#define ALPHABET "abcdefghijklmnopqrstuvwxyz"
#define ALPHABET_LEN strlen(ALPHABET)
#define TAILLE_MIN 4
#define TAILLE_MAX 5
#define NB_CLEAR_TEXTS 11881376
#define TABLE_WIDTH 200
#define TABLE_HEIGHT 100

// Structure représentant la table
typedef struct {
    int width;
    int height;
    int *data; // Tableau contenant les entiers de la table
} Table;

uint8_t EMPREINTE[MD5_DIGEST_LENGTH]; // variable globale pour stocker l'empreinte MD5
char TEXTE[TAILLE_MAX+1]; // variable globale pour stocker le texte "clair"

char *alphabet;
int taille_min, taille_max;
long long int N;

void hash_SHA1(const char* s, byte* empreinte)
{
    SHA1((unsigned char*)s, strlen(s), empreinte);
}

void hash_MD5(const char* s, byte* empreinte)
{
    MD5((unsigned char*)s, strlen(s), empreinte);
}

void init(char *a, int t_min, int t_max) {
    // Copie de l'alphabet donné
    int len = strlen(a);
    alphabet = (char *) malloc(len + 1);
    strcpy(alphabet, a);
    
    // Initialisation des variables globales
    taille_min = t_min;
    taille_max = t_max;
    N = 0;
    
    // Calcul de N
    int n = strlen(alphabet);
    for (int i = taille_min; i <= taille_max; i++) {
        N += pow(n, i);
    }
    
    // Vérification de la valeur de N
    if (N <= 0) {
        printf("Erreur : valeur de N incorrecte !\n");
        exit(1);
    }
}

void parse_args(int argc, char *argv[], Config *config) {
    int opt;

    while ((opt = getopt(argc, argv, "hA:m:M:cs")) != -1) {
        switch (opt) {
            case 'h':
                help = true;
                break;
            case 'A':
                alphabet = optarg;
                break;
            case 'm':
                min_size = atoi(optarg);
                break;
            case 'M':
                max_size = atoi(optarg);
                break;
            case 'c':
                command = CREATE;
                break;
            case 'r':
                command = CRACK;
                break;
            case 's':
                command = TEST;
                break;
            case '?':
                // option invalide
                fprintf(stderr, "Usage: %s [-h] [-A alphabet] [-m min_size] [-M max_size] [-c | -r | -s]\n", argv[0]);
                exit(EXIT_FAILURE);
                break;
        }
    }

    // Vérification des arguments obligatoires
    if (command == UNDEFINED) {
        fprintf(stderr, "Usage: %s [-h] [-A alphabet] [-m min_size] [-M max_size] [-c | -r | -s]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Vérification des arguments
    if (max_size < min_size) {
        fprintf(stderr, "La taille maximale doit être supérieure ou égale à la taille minimale.\n");
        exit(EXIT_FAILURE);
    }

    if (alphabet == NULL) {
        fprintf(stderr, "L'alphabet doit être spécifié.\n");
        exit(EXIT_FAILURE);
    }

    if (min_size < 1 || max_size < 1) {
        fprintf(stderr, "Les tailles minimale et maximale doivent être supérieures ou égales à 1.\n");
        exit(EXIT_FAILURE);
    }
}

char* i2c(int n) {
    int len = 0, m = n;
    while (m > 0) {  // calcul de la longueur de la chaîne à allouer
        m /= 10;
        len++;
    }
    if (n == 0) len = 1;
    char* s = malloc(len + 1);  // allouer de la mémoire pour la chaîne
    if (s == NULL) return NULL;  // vérifier l'allocation
    int i = len - 1;
    while (n > 0) {  // construire la chaîne à partir de la fin
        s[i] = n % 10 + '0';
        n /= 10;
        i--;
    }
    s[len] = '\0';  // terminer la chaîne
    return s;
}

uint64_t h2i(unsigned char* hash, int N) {
    // On cast le tableau d'octets en uint64_t
    uint64_t* ptr = (uint64_t*) hash;

    // On divise le uint64_t en 2 uint32_t pour éviter les problèmes d'overflow
    uint32_t a = (uint32_t) (*ptr & 0xFFFFFFFF);
    uint32_t b = (uint32_t) (*ptr >> 32);

    // On effectue le modulo N
    uint64_t res = ((uint64_t) a + (uint64_t) b) % N;

    return res;
}

void i2i(uint64_t idx1, uint64_t idx2) {
    char texte[20];
    int taille_texte;
    memset(texte, 0, 20);

    sprintf(texte, "%" PRIu64 "%" PRIu64, idx1, idx2);
    taille_texte = strlen(texte);

    MD5((unsigned char*)texte, taille_texte, EMPREINTE);
}

char* nouvelle_chaine(uint64_t idx1, int largeur) {
    int i;
    i2i(idx1, 1);
    memcpy(TEXTE, EMPREINTE, MD5_DIGEST_LENGTH);
    for(i = 2; i <= largeur; i++) {
        i2i(idx1, i);
        memcpy(TEXTE, EMPREINTE, MD5_DIGEST_LENGTH);
    }
    TEXTE[MD5_DIGEST_LENGTH] = '\0';
    return TEXTE;
}

// Fonction qui calcule l'indice aléatoire pour une chaîne de longueur donnée
uint64_t index_aleatoire(int taille_chaine) {
    // Calcule l'empreinte MD5 du texte clair
    MD5_CTX context;
    MD5_Init(&context);
    MD5_Update(&context, TEXTE, taille_chaine);
    MD5_Final(EMPREINTE, &context);

    // Convertit les 8 premiers octets de l'empreinte en un entier non signé de 64 bits
    uint64_t i = *((uint64_t *) EMPREINTE);

    // Effectue le modulo par rapport à N
    i = i % 387515648;

    // Retourne l'indice aléatoire
    return i;
}

// Fonction qui crée une table de hachage triée
void creer_table(int largeur, int hauteur) {
    // Alloue de la mémoire pour la table de hachage
    uint64_t **table = (uint64_t **) malloc(hauteur * sizeof(uint64_t *));
    for (int i = 0; i < hauteur; i++) {
        table[i] = (uint64_t *) malloc(TABLE_WIDTH * sizeof(uint64_t));
    }

    // Initialise la première colonne de la table avec des indices croissants
    for (int i = 0; i < hauteur; i++) {
        table[i][0] = i + 1;
    }

    // Remplit le reste de la table avec des indices aléatoires
    for (int j = 1; j < TABLE_WIDTH; j++) {
        for (int i = 0; i < TABLE_HEIGHT; i++) {
            strncpy(TEXTE, nouvelle_chaine(table[i][j-1], largeur), TAILLE_MAX);
            uint64_t index = index_aleatoire(strlen(TEXTE));
            table[i][j] = index;
        }
    }

    // Trie la table par ordre croissant de la dernière colonne
    qsort(table, hauteur, sizeof(uint64_t *), compar);

    // Affiche la table
    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < TABLE_WIDTH; j++) {
            printf("%011lu: %03lu --> %011lu\n", i, j, table[i][j]);
        }
    }

    // Libère la mémoire allouée pour la table de hachage
    for (int i = 0; i < hauteur; i++) {
        free(table[i]);
    }
    free(table);
}

// Fonction qui sauvegarde une table dans un fichier au format ASCII
void sauve_table_ascii(Table *table, char *filename, char *hash_func, char *alphabet, int min_size, int max_size) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Erreur : impossible d'ouvrir le fichier %s en écriture\n", filename);
        return;
    }
    
    // Entête : informations sur la table
    fprintf(f, "hash function: %s\n", hash_func);
    fprintf(f, "alphabet: %s\n", alphabet);
    fprintf(f, "min size: %d\n", min_size);
    fprintf(f, "max size: %d\n", max_size);
    fprintf(f, "width: %d\n", table->width);
    fprintf(f, "height: %d\n", table->height);
    
    // Données : début/fin de chaque chaîne au format ASCII
    for (int i = 0; i < table->height; i++) {
        int start = table->data[i * table->width];
        int end = table->data[(i+1) * table->width - 1];
        fprintf(f, "%08d --> %08d\n", start, end);
    }
    
    fclose(f);
}

// Fonction qui ouvre une table à partir d'un fichier au format ASCII
Table *ouvre_table_ascii(char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Erreur : impossible d'ouvrir le fichier %s en lecture\n", filename);
        return NULL;
    }
    
    // Entête : informations sur la table
    char hash_func[256];
    char alphabet[256];
    int min_size, max_size, width, height;
    fscanf(f, "hash function: %255s\n", hash_func);
    fscanf(f, "alphabet: %255s\n", alphabet);
    fscanf(f, "min size: %d\n", &min_size);
    fscanf(f, "max size: %d\n", &max_size);
    fscanf(f, "width: %d\n", &width);
    fscanf(f, "height: %d\n", &height);
    Table *table = malloc(sizeof(Table));
    table->width = width;
    table->height = height;
    table->data = malloc(width * height * sizeof(int));
    
    // Données : début/fin de chaque chaîne au format ASCII
    for (int i = 0; i < height; i++) {
        int start, end;
        fscanf(f, "%08d --> %08d\n", &start, &end);
        table->data[i * width] = start;
        table->data[(i+1) * width - 1] = end;
    }
    
    fclose(f);
    return table;
}

void affiche_table(const char *nom_fichier, int debut, int fin) {
    FILE *fichier = fopen(nom_fichier, "rb");
    if (fichier == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", nom_fichier);
        exit(1);
    }

    int largeur, hauteur;
    fread(&largeur, sizeof(largeur), 1, fichier);
    fread(&hauteur, sizeof(hauteur), 1, fichier);
    printf("Table %dx%d\n", largeur, hauteur);

    int nb_lignes = fin - debut + 1;
    int debut_offset = debut * sizeof(int) * 2;
    int fin_offset = fin * sizeof(int) * 2;

    // On se positionne au début de la première ligne à afficher
    fseek(fichier, debut_offset + sizeof(largeur) + sizeof(hauteur), SEEK_SET);

    for (int i = debut; i <= fin; i++) {
        int debut_chaine, fin_chaine;
        fread(&debut_chaine, sizeof(debut_chaine), 1, fichier);
        fread(&fin_chaine, sizeof(fin_chaine), 1, fichier);

        printf("%08x: %d --> %d\n", i, debut_chaine, fin_chaine);

        // On passe à la ligne suivante
        fseek(fichier, sizeof(int) * 2 * (largeur - 1), SEEK_CUR);
    }

    fclose(fichier);
}

int compare_entree_table(const void *a, const void *b) {
    return memcmp(((const EntreeTable *)a)->fin,
                  ((const EntreeTable *)b)->fin,
                  MAX_CHAINE);
}

void recherche_dichotomique(const EntreeTable *table, int n, const uint8_t *valeur,
                            int *debut, int *fin) {
    // Recherche dichotomique sur la table triée par ordre croissant de la dernière colonne
    int gauche = 0;
    int droite = n - 1;
    int milieu = (gauche + droite) / 2;
    while (gauche <= droite) {
        int cmp = memcmp(valeur, table[milieu].fin, MAX_CHAINE);
        if (cmp == 0) {
            // Valeur trouvée, cherchons l'intervalle complet
            *debut = milieu;
            while (*debut > 0 && memcmp(valeur, table[*debut - 1].fin, MAX_CHAINE) == 0) {
                (*debut)--;
            }
            *fin = milieu;
            while (*fin < n - 1 && memcmp(valeur, table[*fin + 1].fin, MAX_CHAINE) == 0) {
                (*fin)++;
            }
            return;
        } else if (cmp < 0) {
            droite = milieu - 1;
        } else {
            gauche = milieu + 1;
        }
        milieu = (gauche + droite) / 2;
    }
    // Valeur non trouvée
    *debut = -1;
    *fin = -1;
}

void inverse_chaine(uint8_t *chaine) {
    int longueur = strlen((const char *)chaine);
    for (int i = 0; i < longueur / 2; i++) {
        uint8_t temp = chaine[i];
        chaine[i] = chaine[longueur - i - 1];
        chaine[longueur - i - 1] = temp;
    }
}

void print_stats(int N, int largeur, int hauteur) {
    int taille = largeur * hauteur * 16; // Chaque point est codé sur 16 octets
    printf("Estimation de la taille de la table : %d octets\n", taille);
    
    int i, m = hauteur;
    double v = 1.0;
    for (i = 0; i < largeur; i++) {
        v = v * (1 - (double)m / N);
        m = N * (1 - exp(- (double)m / N));
    }
    double couverture = 100 * (1 - v);
    printf("Estimation de la couverture de la table : %.2f%%\n", couverture);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command> [arguments]\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "create") == 0) {
        if (argc != 5) {
            printf("Usage: %s create <hauteur> <largeur> <fichier>\n", argv[0]);
            return 1;
        }
        int hauteur = atoi(argv[2]);
        int largeur = atoi(argv[3]);
        char *fichier = argv[4];
        int result = cree_table(hauteur, largeur, fichier);
        if (result == 0) {
            printf("Table créée avec succès dans le fichier %s.\n", fichier);
        } else {
            printf("Erreur lors de la création de la table.\n");
        }
    } else if (strcmp(argv[1], "info") == 0) {
        if (argc != 3) {
            printf("Usage: %s info <fichier>\n", argv[0]);
            return 1;
        }
        char *fichier = argv[2];
        int result = affiche_table(fichier, 0, 10);
        if (result == 0) {
            printf("Affichage des premières et dernières valeurs de la table dans le fichier %s.\n", fichier);
        } else {
            printf("Erreur lors de l'affichage de la table.\n");
        }
    } else if (strcmp(argv[1], "crack") == 0) {
        if (argc != 3) {
            printf("Usage: %s crack <hash>\n", argv[0]);
            return 1;
        }
        char *hash = argv[2];
        int result = crack(hash);
        if (result == 0) {
            printf("Hash cracké avec succès !\n");
        } else {
            printf("Erreur lors du cracking du hash.\n");
        }
    } else if (strcmp(argv[1], "stats") == 0) {
        if (argc != 4) {
            printf("Usage: %s stats <hauteur> <largeur>\n", argv[0]);
            return 1;
        }
        int hauteur = atoi(argv[2]);
        int largeur = atoi(argv[3]);
        int taille = estime_taille(hauteur, largeur);
        float couverture = estime_couverture(hauteur, largeur);
        printf("Estimation de la taille de la table : %d octets.\n", taille);
        printf("Estimation de la couverture de la table : %.2f%%.\n", couverture);
    } else {
        printf("Commande inconnue : %s\n", argv[1]);
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    int cmd_index = 1;
    if (argc <= 1) {
        fprintf(stderr, "Usage: ./rbt <CMD> [OPTIONS] [ARGS]\n");
        return 1;
    }

    if (strcmp(argv[1], "create") == 0) {
        create_rainbow_table(argc - 2, argv + 2);
    } else if (strcmp(argv[1], "info") == 0) {
        info_rainbow_table(argc - 2, argv + 2);
    } else if (strcmp(argv[1], "stats") == 0) {
        stats_rainbow_table(argc - 2, argv + 2);
    } else if (strcmp(argv[1], "crack") == 0) {
        crack_hash(argc - 2, argv + 2);
    } else if (strcmp(argv[1], "bruteforce") == 0) {
        bruteforce_hash(argc - 2, argv + 2);
    } else if (strcmp(argv[1], "test") == 0) {
        cmd_index = test_rainbow_tables(argc - 2, argv + 2);
    } else {
        fprintf(stderr, "Invalid command\n");
        return 1;
    }

    return 0;
}