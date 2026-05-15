//
// Utkan Başurgan
//
//--------------------------------------------------------------------------------------------------------------------------------

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wgnu-folding-constant"

//--------------------------------------------------------------------------------------------------------------------------------

#include "02_settings.c"
#include "logs.c"
#include "dictionary.c"
#include "entities.c"
#include "mutation.c"
#include "learning.c"
#include "test.c"

//--------------------------------------------------------------------------------------------------------------------------------

/*
@brief Program giriş noktası: günlük kaydı, sözlük kurulumu, popülasyon başlatma, genetik iterasyon ve test çıktısını yürütür.

@return İşletim sistemine dönen çıkış kodu; başarıda 0.
*/
int main()
{
    srand((unsigned int)time(NULL));

    saveLogPrintf(
        "Hiper parametreler: N=%d populationSize=%d randomnessPercentage=%d "
        "iterationThreshold=%d scoreThreshold=%d\n",
        N,
        populationSize,
        randomnessPercentage,
        iterationThreshold,
        scoreThreshold
    );

    Dictionary *dictionary = createDictionary();
 
    char trainFiles[NUM_AUTHORS][optimization_count][1024];

    for (int a = 0; a < NUM_AUTHORS; a++)
    {
        for (int t = 0; t < optimization_count; t++)
        {
            snprintf(
                trainFiles[a][t],
                sizeof(trainFiles[a][t]),
                "%s/%s/%d.txt",
                data_root,
                AUTHOR_DIR_NAMES[a],
                t + 1
            );
        }
    }
 
    Entity population[populationSize];
    setupEntities(population, dictionary);
    
    //geneticIteration(population, dictionary, trainFiles);
    geneticIteration2(population, dictionary, trainFiles);

    testLearningPrint(&population[0]);

    return 0;
}

#pragma GCC diagnostic pop

//--------------------------------------------------------------------------------------------------------------------------------