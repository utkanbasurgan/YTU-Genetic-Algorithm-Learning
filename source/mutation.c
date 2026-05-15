//
// Utkan Başurgan
//
//--------------------------------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//--------------------------------------------------------------------------------------------------------------------------------

/*
@brief İki bireyin her yazar için çift numaralı gen indekslerinde kelime işaretçilerini takas eder.

@param firstEntity Birinci ebeveyn birey.
@param secondEntity İkinci ebeveyn birey.

@return Yok.
*/
void doCrossingover(Entity *firstEntity, Entity *secondEntity)
{
    for (int author = 0; author < 9; author++)
    {
        for (int word = 0; word < N; word += 2)
        {
            char *tmp = firstEntity->genes[author][word];
            firstEntity->genes[author][word] = secondEntity->genes[author][word];
            secondEntity->genes[author][word] = tmp;
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------------------

/*
@brief Belirlenen rastgelelik yüzdesine göre genleri sözlükten seçilen yeni kelimelerle değiştirir.

@param entity Mutasyona uğrayan birey.
@param dictionary Yeni kelimelerin alındığı sözlük.

@return Yok.
*/
void randomByDictionary(Entity *entity, Dictionary *dictionary)
{
    for (int author = 0; author < 9; author++)
    {
        for (int word = 0; word < N; word++)
        {
            if (rand() % 100 < randomnessPercentage)
            {
                char *newWord = strdup(getRandomWordFromDictionary(dictionary));
                free(entity->genes[author][word]);
                entity->genes[author][word] = newWord;
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------------------

/*
@brief Belirlenen rastgelelik yüzdesine göre genleri aynı bireyin başka yazar veya kelime konumlarından kopyalar.

@param entity Mutasyona uğrayan birey.

@return Yok.
*/
void randomByOtherGenes(Entity *entity)
{
    for (int author = 0; author < 9; author++)
    {
        for (int word = 0; word < N; word++)
        {
            if (rand() % 100 < randomnessPercentage)
            {
                int otherAuthor = rand() % 9;
                int otherWord = rand() % N;
                char *newWord = strdup(entity->genes[otherAuthor][otherWord]);
                free(entity->genes[author][word]);
                entity->genes[author][word] = newWord;
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------------------