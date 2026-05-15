//
// Utkan Başurgan
//
//--------------------------------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//--------------------------------------------------------------------------------------------------------------------------------

typedef struct
{
    char *word;
    int occupied;
} 
dictionaryEntry;

typedef struct
{
    dictionaryEntry dictionarySlot[HASH_SIZE];
    int total_words;
} 
Dictionary;

//--------------------------------------------------------------------------------------------------------------------------------

/*
@brief Kelime dizgesi için özet (hash) değeri üretir ve tablo boyutuna göre mod alır.

@param word Özetlenecek null sonlu dizge.

@return 0 ile HASH_SIZE-1 aralığında yuva indeksi.
*/
static unsigned int hashWord(const char *word)
{
    unsigned int hash = 5381;

    while (*word)
    {
        hash = ((hash << 5) + hash) + (unsigned char)(*word);
        word++;
    }
    
    return hash % HASH_SIZE;
}

/*
@brief Sözlüğe kelime ekler; kelime zaten varsa yinelenen kayıt oluşturmaz.

@param dict Hedef sözlük yapısı.
@param word Eklenecek kelime (içerik kopyalanır).

@return Yok.
*/
static void insertWord(Dictionary *dict, const char *word)
{
    unsigned int current = hashWord(word);

    while (dict->dictionarySlot[current].occupied)
    {
        if (strcmp(dict->dictionarySlot[current].word, word) == 0)
        {
            return;
        }
        current = (current + 1) % HASH_SIZE;
    }

    dict->dictionarySlot[current].word = strdup(word);
    dict->dictionarySlot[current].occupied = 1;
    dict->total_words++;
}

/*
@brief Kaynak dizgeden yalnızca harfleri alır, küçük harfe çevirip hedef tampona yazar.

@param dest Çıktı tamponu (yeterli uzunlukta olmalıdır).
@param src Okunacak ham kelime veya token.

@return Yok.
*/
static void cleanWord(char *dest, const char *src)
{
    int j = 0;

    for (int i = 0; src[i]; i++)
    {
        if (isalpha((unsigned char)src[i]))
            dest[j++] = tolower((unsigned char)src[i]);
    }

    dest[j] = '\0';
}

//--------------------------------------------------------------------------------------------------------------------------------

/*
@brief Metin dosyasından kelimeleri okur, temizler ve sözlüğe ekler.

@param dict Hedef sözlük.
@param filepath Okunacak dosyanın yolu.

@return Yok.
*/
static void processFile(Dictionary *dict, const char *filepath)
{
    FILE *file = fopen(filepath, "r");

    if (!file)
    {
        fprintf(stderr, "Cannot open: %s\n", filepath);
        return;
    }

    char raw[256];
    char clean[256];

    while (fscanf(file, "%255s", raw) == 1)
    {
        cleanWord(clean, raw);

        if (strlen(clean) > 1)
            insertWord(dict, clean);
    }

    fclose(file);
}

/*
@brief Yeni sözlük ayırır ve tüm yazarların eğitim metin dosyalarından kelime havuzunu kurar.

@return Oluşturulan sözlük işaretçisi.
*/
Dictionary *createDictionary()
{
    Dictionary *dict = malloc(sizeof(Dictionary));
    memset(dict->dictionarySlot, 0, sizeof(dict->dictionarySlot));
    dict->total_words = 0;

    for (int a = 0; a < NUM_AUTHORS; a++)
    {
        for (int x = TEXT_INDEX_MIN; x <= TEXT_INDEX_MAX; x++)
        {
            char filepath[1024];
            snprintf(
                filepath,
                sizeof(filepath),
                "%s/%s/%d.txt",
                data_root,
                AUTHOR_DIR_NAMES[a],
                x
            );
            processFile(dict, filepath);
        }
    }

    saveLogPrintf("Sözlük: %d tane kelimeli\n", dict->total_words);
    return dict;
}

//--------------------------------------------------------------------------------------------------------------------------------