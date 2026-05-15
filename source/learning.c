//
// Utkan Başurgan
//
//--------------------------------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//--------------------------------------------------------------------------------------------------------------------------------

/*
@brief Popülasyonu fitness değerine göre azalan düzende kabarcık sıralaması ile sıralar.

@param population Sıralanacak birey dizisi.

@return Yok.
*/
void bubbleSort(Entity population[])
{
    for (int i = 0; i < populationSize - 1; i++)
    {
        for (int j = 0; j < populationSize - 1 - i; j++)
        {
            if (population[j].fitness < population[j + 1].fitness)
            {
                Entity tmp = population[j];
                population[j] = population[j + 1];
                population[j + 1] = tmp;
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------------------

/*
@brief Dosyadaki kelimeleri bireyin genleriyle eşleştirerek en çok eşleşen yazar indeksini bulur; eşitlikte sınıf rastgele seçilir.

@param entity Sınıflandırma için kullanılan birey.
@param filepath Okunacak metin dosyasının yolu.

@return Tahmin edilen yazar indeksi (0..NUM_AUTHORS-1); dosya açılamazsa -1.
*/
int classifyText(Entity *entity, const char *filepath)
{
    FILE *file = fopen(filepath, "r");
    if (!file)
        return -1;

    int counts[9] = {0};
    char raw[256];
    char clean[256];

    while (fscanf(file, "%255s", raw) == 1)
    {
        cleanWord(clean, raw);

        for (int author = 0; author < 9; author++)
        {
            for (int word = 0; word < N; word++)
            {
                if (strcmp(entity->genes[author][word], clean) == 0)
                {
                    counts[author]++;
                    break;
                }
            }
        }
    }

    fclose(file);

    int maxCount = counts[0];
    for (int i = 1; i < 9; i++)
    {
        if (counts[i] > maxCount)
            maxCount = counts[i];
    }

    int tied[9];
    int nTied = 0;
    for (int i = 0; i < 9; i++)
    {
        if (counts[i] == maxCount)
        {
            tied[nTied++] = i;
        }
    }

    return tied[rand() % nTied];
}

/*
@brief Her bireyin fitness değerini eğitim dosyalarındaki doğru sınıflandırma sayısı olarak günceller.

@param population Değerlendirilecek popülasyon.
@param trainFiles Yazar ve eğitim dosyası indeksine göre dosya yolu matrisi.

@return Yok.
*/
void checkScore(Entity population[], char trainFiles[NUM_AUTHORS][optimization_count][1024])
{
    for (int entity = 0; entity < populationSize; entity++)
    {
        population[entity].fitness = 0;

        for (int author = 0; author < 9; author++)
        {
            for (int t = 0; t < optimization_count; t++)
            {
                int predicted = classifyText(&population[entity], trainFiles[author][t]);

                if (predicted == author)
                    population[entity].fitness++;
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------------------

/*
@brief Tek birey için eğitim kümesi üzerinde gerçek ve tahmin edilen yazar sayılarını matrise yazar.

@param entity Değerlendirilen birey.
@param trainFiles Eğitim dosya yolları matrisi.
@param matrix Çıktı; önce sıfırlanır, [gerçek yazar][tahmin] hücreleri artırılır.

@return Yok.
*/
void buildConfusionMatrix(Entity *entity, char trainFiles[NUM_AUTHORS][optimization_count][1024], int matrix[9][9])
{
    memset(matrix, 0, sizeof(int) * 9 * 9);

    for (int actual = 0; actual < NUM_AUTHORS; actual++)
    {
        for (int t = 0; t < optimization_count; t++)
        {
            int predicted = classifyText(entity, trainFiles[actual][t]);

            if (predicted >= 0 && predicted < NUM_AUTHORS)
                matrix[actual][predicted]++;
        }
    }
}

/*
@brief 9x9 karışıklık matrisini satır bazlı geri çağırım ve sütun bazlı kesinlik özetleriyle günlüğe yazdırır.

@param matrix Yazdırılacak karışıklık matrisi.

@return Yok.
*/
void printConfusionMatrix(int matrix[9][9])
{
    saveLogPrintf("\n=== Confusion Matrix (rows=actual, cols=predicted) ===\n\n");
    saveLogPrintf("       ");

    for (int i = 0; i < NUM_AUTHORS; i++)
        saveLogPrintf("  A%-2d", i);

    saveLogPrintf("\n");

    for (int i = 0; i < NUM_AUTHORS; i++)
    {
        saveLogPrintf("A%-2d  |", i);

        for (int j = 0; j < NUM_AUTHORS; j++)
        {
            if (i == j)
            {
                saveLogPrintf(" [%2d]", matrix[i][j]);
            }
            else
            {
                saveLogPrintf("  %2d ", matrix[i][j]);
            }
        }

        int rowTotal = 0;
        for (int j = 0; j < NUM_AUTHORS; j++)
        {
            rowTotal += matrix[i][j];
        }

        int correct = matrix[i][i];
        float recall = rowTotal > 0 ? (float)correct / rowTotal * 100.0f : 0.0f;

        saveLogPrintf(" | recall: %5.1f%%\n", recall);
    }

    saveLogPrintf("\n       ");
    for (int i = 0; i < NUM_AUTHORS; i++)
        saveLogPrintf("------");

    saveLogPrintf("\nprec:  ");

    for (int j = 0; j < NUM_AUTHORS; j++)
    {
        int colTotal = 0;
        for (int i = 0; i < NUM_AUTHORS; i++)
        {
            colTotal += matrix[i][j];
        }

        float precision = colTotal > 0 ? (float)matrix[j][j] / colTotal * 100.0f : 0.0f;
        saveLogPrintf("%4.0f%%", precision);
    }

    saveLogPrintf("\n\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

/*
@brief Bir bireyin yazar satırları ve kelime sütunları şeklinde gen tablosunu günlüğe yazdırır.

@param entity Listelenecek birey.

@return Yok.
*/
void printBestEntityWordMatrix(Entity *entity)
{
    saveLogPrintf("\n=== Best entity word matrix (%d x %d) ===\n", NUM_AUTHORS, N);
    saveLogPrintf("     |");
    for (int w = 0; w < N; w++)
        saveLogPrintf(" w%-2d", w);
    saveLogPrintf("\n");

    for (int a = 0; a < NUM_AUTHORS; a++)
    {
        saveLogPrintf("A%-2d |", a);
        for (int w = 0; w < N; w++)
            saveLogPrintf(" %s", entity->genes[a][w]);
        saveLogPrintf("\n");
    }
    saveLogPrintf("\n");
}

//--------------------------------------------------------------------------------------------------------------------------------

/*
@brief Popülasyondaki tüm bireylerin fitness ortalamasını hesaplar.

@param population Birey dizisi.

@return Ortalama fitness (kayan nokta).
*/
static double computeAverageFitness(Entity population[])
{
    double sum = 0.0;
    for (int i = 0; i < populationSize; i++)
        sum += population[i].fitness;
    return sum / populationSize;
}

/*
@brief Verilen iterasyonda en iyi, ortalama ve en kötü fitness ile yüzde istatistiklerini günlüğe yazar.

@param iter İterasyon numarası.
@param population İstatistiği üretilecek popülasyon (önceden fitness’a göre azalan sıralı olmalıdır).

@return Yok.
*/
static void logIterationStats(int iter, Entity population[])
{
    double avg    = computeAverageFitness(population);
    int    best   = population[0].fitness;
    int    worst  = population[populationSize - 1].fitness;
    int    total  = NUM_AUTHORS * optimization_count;
    float  bestPct = (float)best / total * 100.0f;
    float  avgPct  = (float)avg  / total * 100.0f;

    saveLogPrintf(
        "Iter %4d | best: %d/%d (%.1f%%)  avg: %.1f/%.0f (%.1f%%)  worst: %d/%d\n",
        iter,
        best,  total, bestPct,
        avg,   (float)total, avgPct,
        worst, total
    );
}

//--------------------------------------------------------------------------------------------------------------------------------

/*
@brief Yüzde dilimli operatörlerle genetik döngüyü çalıştırır: üst %15 çapraz, %30–%60 diğer genlerden mutasyon, %60–%100 sözlük mutasyonu; ardından karışıklık ve kelime matrisi çıktısı üretir.

@param population Evrilecek popülasyon.
@param dict Mutasyonlarda kullanılan sözlük.
@param trainFiles Eğitim dosya yolları matrisi.

@return Yok.
*/
void geneticIteration(Entity population[], Dictionary *dict, char trainFiles[NUM_AUTHORS][optimization_count][1024])
{
    int iter;
    for (iter = 0; iter < iterationThreshold; iter++)
    {
        checkScore(population, trainFiles);
        bubbleSort(population);
        logIterationStats(iter, population);

        if (population[0].fitness >= scoreThreshold)
        {
            saveLogPrintf("Erken durdurma: en iyi skor >= scoreThreshold (%d).\n", scoreThreshold);
            break;
        }

        int p15 = populationSize * 15 / 100;
        int p30 = populationSize * 30 / 100;
        int p60 = populationSize * 60 / 100;

        for (int i = 0; i < p15; i++)
            doCrossingover(&population[i], &population[p15 + i]);

        for (int i = p30; i < p60; i++)
            randomByOtherGenes(&population[i]);

        for (int i = p60; i < populationSize; i++)
            randomByDictionary(&population[i], dict);
    }

    checkScore(population, trainFiles);
    bubbleSort(population);
    if (iter >= iterationThreshold)
        logIterationStats(iterationThreshold, population);

    int confMatrix[9][9];
    buildConfusionMatrix(&population[0], trainFiles, confMatrix);
    printConfusionMatrix(confMatrix);
    printBestEntityWordMatrix(&population[0]);
}

//--------------------------------------------------------------------------------------------------------------------------------

/*
@brief Elit koruma, üst %20 ile çapraz, %60–%85 diğer gen mutasyonu ve %85–%100 sözlük keşfi ile genetik döngüyü çalıştırır; sonunda karışıklık ve kelime matrisi yazar.

@param population Evrilecek popülasyon.
@param dict Sözlük tabanlı mutasyon için kelime havuzu.
@param trainFiles Eğitim dosya yolları matrisi.

@return Yok.
*/
void geneticIteration2(Entity population[], Dictionary *dict, char trainFiles[NUM_AUTHORS][optimization_count][1024])
{
    int iter;
    for (iter = 0; iter < iterationThreshold; iter++)
    {
        checkScore(population, trainFiles);
        bubbleSort(population);
        logIterationStats(iter, population);

        if (population[0].fitness >= scoreThreshold)
        {
            saveLogPrintf("Erken durdurma: en iyi skor >= scoreThreshold (%d).\n", scoreThreshold);
            break;
        }

        int p20 = populationSize * 20 / 100;
        int p60 = populationSize * 60 / 100;
        int p85 = populationSize * 85 / 100;

        for (int i = p20; i < p60; i++)
            doCrossingover(&population[i], &population[rand() % p20]);

        for (int i = p60; i < p85; i++)
            randomByOtherGenes(&population[i]);

        for (int i = p85; i < populationSize; i++)
            randomByDictionary(&population[i], dict);
    }

    checkScore(population, trainFiles);
    bubbleSort(population);
    if (iter >= iterationThreshold)
        logIterationStats(iterationThreshold, population);

    int confMatrix[9][9];
    buildConfusionMatrix(&population[0], trainFiles, confMatrix);
    printConfusionMatrix(confMatrix);
    printBestEntityWordMatrix(&population[0]);
}

//--------------------------------------------------------------------------------------------------------------------------------