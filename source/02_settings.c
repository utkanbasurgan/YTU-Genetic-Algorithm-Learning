//
// Utkan Başurgan
//
//--------------------------------------------------------------------------------------------------------------------------------

const char *data_root = "90koseyazisi/raw_texts";
const char *out_path = "zresult.csv";

//--------------------------------------------------------------------------------------------------------------------------------

// Dosya Sistemi
#define NUM_AUTHORS 9
#define TEXT_INDEX_MIN 1
#define TEXT_INDEX_MAX 10

static const char *AUTHOR_DIR_NAMES[NUM_AUTHORS] = 
{
    "dhizlan",
    "ecelebi",
    "ekumcu",
    "eozkok",
    "esaglam",
    "faltayli",
    "gtezmen",
    "psuda",
    "sturgut"
};

//--------------------------------------------------------------------------------------------------------------------------------

// Sözlük Sistemi
#define HASH_SIZE 100003

//--------------------------------------------------------------------------------------------------------------------------------

// Genel Değişkenler
const int dictionarySize = 100003;
static const int optimization_count = 7;
static const int test_count = 3;

//--------------------------------------------------------------------------------------------------------------------------------

// Sınırlar
const int iterationThreshold = 50;
const int scoreThreshold = 20;

//--------------------------------------------------------------------------------------------------------------------------------

// Hiper Parametreler
const int N = 100;
const int populationSize = 200;
const int randomnessPercentage = 20; // 100 == 100%

//--------------------------------------------------------------------------------------------------------------------------------