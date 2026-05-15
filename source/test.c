//
// Utkan Başurgan
//
//--------------------------------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

//--------------------------------------------------------------------------------------------------------------------------------

/*
@brief Ayrılmış test dosyalarında bireyi değerlendirir; doğruluk özeti, CSV satırları ve karışıklık matrisini günlüğe yazar.

@param entity Test edilecek birey (eğitilmiş genler).

@return Yok.
*/
void testLearningPrint(Entity *entity)
{
    int matrix[NUM_AUTHORS][NUM_AUTHORS];
    memset(matrix, 0, sizeof(matrix));

    int totalTests = 0;
    int correct    = 0;
    int missFiles  = 0;

    for (int author = 0; author < NUM_AUTHORS; author++)
    {
        for (int fileIdx = TEXT_INDEX_MAX - test_count + 1; fileIdx <= TEXT_INDEX_MAX; fileIdx++)
        {
            char path[1024];

            snprintf(
                path,
                sizeof(path),
                "%s/%s/%d.txt",
                data_root,
                AUTHOR_DIR_NAMES[author],
                fileIdx
            );

            int predicted = classifyText(entity, path);
            totalTests++;

            if (predicted < 0)
            {
                missFiles++;
                saveLogPrintf("[test] miss path=%s\n", path);
                continue;
            }

            if (predicted >= 0 && predicted < NUM_AUTHORS)
                matrix[author][predicted]++;

            if (predicted == author)
                correct++;
        }
    }

    float accPct =
        totalTests > 0 ? (float)correct / (float)totalTests * 100.0f : 0.0f;

    saveLogPrintf(
        "\n=== Test set (dosyalar %d..%d, yazar basina %d) ===\n\n",
        TEXT_INDEX_MAX - test_count + 1,
        TEXT_INDEX_MAX,
        test_count
    );
    saveLogPrintf(
        "Test | dogru: %d/%d (%.1f%%)  okunamayan: %d\n",
        correct,
        totalTests,
        accPct,
        missFiles
    );
    saveLogPrintf(
        "CSV_TEST_SUMMARY,total,%d,correct,%d,accuracy_pct,%.2f,miss_files,%d\n",
        totalTests,
        correct,
        (double)accPct,
        missFiles
    );

    for (int a = 0; a < NUM_AUTHORS; a++)
    {
        int rowSum = 0;
        for (int j = 0; j < NUM_AUTHORS; j++)
            rowSum += matrix[a][j];

        float recall = rowSum > 0 ? (float)matrix[a][a] / (float)rowSum * 100.0f : 0.0f;
        saveLogPrintf(
            "Test A%d | dogru: %d/%d (recall %.1f%%)\n",
            a,
            matrix[a][a],
            rowSum,
            recall
        );
        saveLogPrintf(
            "CSV_TEST_AUTHOR,author,%d,correct,%d,total,%d,recall_pct,%.2f\n",
            a,
            matrix[a][a],
            rowSum,
            (double)recall
        );
    }

    saveLogPrintf("\n");
    printConfusionMatrix(matrix);
}

//--------------------------------------------------------------------------------------------------------------------------------
