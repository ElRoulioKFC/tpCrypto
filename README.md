# tpCrypto

## Question 1
Nous avons utilisé les librairy libssl et lcrypto pour le programme. 

Afin de testé l'implémentation de MD5, utiliser la commande : 
./exec --md5=Salut
Le résultat est bien "af4fef1bc0861ca2824db7315f844327"

Afin de testé l'implémentation de SHA, utiliser la commande :
./exec --sha1=Bob
Le résultat est bien "da6645f6e22bf5f75974dc7eed5fcd6160d6b51e"

## Question 2
Nous avons implémenté l'initialisation de l'alphabet, des tailles min et max et de N avec des variables globales. 

Afin de testé l'implémentation, utiliser les commandes : 
./exec --alphabet=abcdefghijklmnopqrstuvwxyz --min-size=4 --max-size=4
Résultat : N --> 456976

./exec --alphabet=abcdefghijklmnopqrstuvwxyz --min-size=2 --max-size=4
Résultat : N --> 475228

./exec --alphabet=ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz --min-size=4 --max-size=7
Résultat : N --> 1048229827840

## Question 3
Nous avons programmé la fonction "i2c" en deux parties, une partie naive qui ne calcule que pour une seule taille de chaîne, et une fonction qui utilise cette dernière pour chaque taille possible. 

Afin de testé l'implémentation : 
./exec --alphabet=ABCDEFGHIJKLMNOPQRSTUVWXYZ --min-size=4 --max-size=4 --i2c=1234
Résultat : N --> 456976
           Detected size --> 4
           I2C --> ABVM

./exec --alphabet=ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz --min-size=1 --max-size=6 --i2c=150106454
Résultat : N --> 20158268676
           Detected size --> 5
           I2C --> Table

./exec --alphabet=ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz --min-size=1 --max-size=6 --i2c=75324
Résultat : N --> 20158268676
           Detected size --> 3
           I2C --> arc

./exec --alphabet=ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz --min-size=1 --max-size=6 --i2c=1651
Résultat : N --> 20158268676
           Detected size --> 2
           I2C --> en

./exec --alphabet=ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz --min-size=1 --max-size=6 --i2c=4173921
Résultat : N --> 20158268676
           Detected size --> 4
           I2C --> ciel

## Question 4

## Question 5
Pour testé la fonction h2i : 
./exec --alphabet=abcdefghijklmnopqrstuvwxyz --min-size=4 --max-size=5 --md5=oups --h2i
Résultat : N --> 12338352
           MD5 de "oups" --> 72eb471fb3bd65c03d29f2fcbb9984d6
           H2I de 72eb471fb3bd65c03d29f2fcbb9984d6 avec un t = 1 --> 10419507

## Question 6

## Question 7

La variable t permet d'augmenter la couverture de la table en utilisant des indices aléatoires.