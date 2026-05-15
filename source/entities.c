//
// Utkan Başurgan
//
//--------------------------------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//--------------------------------------------------------------------------------------------------------------------------------

typedef struct
{
    char *genes[9][N];
    int fitness;
}
Entity;

//--------------------------------------------------------------------------------------------------------------------------------

/*
@brief Sözlükte rastgele dolu bir yuva seçer ve o yuvadaki kelimeyi döndürür.

@param dictionary Kelime yuvalarını içeren sözlük.

@return Seçilen kelimenin işaretçisi (sözlük belleğinde kalır, serbest bırakılmamalıdır).
*/
static const char *getRandomWordFromDictionary(Dictionary *dictionary)
{
    int random;

    do
    {
        random = rand() % HASH_SIZE;
    }
    while (!dictionary->dictionarySlot[random].occupied);

    return dictionary->dictionarySlot[random].word;
}

//--------------------------------------------------------------------------------------------------------------------------------

/*
@brief Popülasyondaki her bireyin gen matrisini sözlükten rastgele kelimelerle doldurur ve fitness değerini sıfırlar.

@param population Başlatılacak birey dizisi.
@param dictionary Kelime kaynağı.

@return Yok.
*/
void setupEntities(Entity population[], Dictionary *dictionary)
{
    for (int entity = 0; entity < populationSize; entity++)
    {
        for (int author = 0; author < 9; author++)
        {
            for (int word = 0; word < N; word++)
            {
                population[entity].genes[author][word] = strdup(getRandomWordFromDictionary(dictionary));
            }
        }

        population[entity].fitness = 0;
    }
}

//--------------------------------------------------------------------------------------------------------------------------------