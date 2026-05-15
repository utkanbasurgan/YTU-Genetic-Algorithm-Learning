//
// Utkan Başurgan
//
//--------------------------------------------------------------------------------------------------------------------------------

#include <stdarg.h>
#include <stdio.h>

//--------------------------------------------------------------------------------------------------------------------------------

/*
@brief Tek bir metin satırını günlük çıktı dosyasına ekler (append).

@param line Yazılacak satır (null sonlu).

@return Yok.
*/
void saveLog(const char *line)
{
    FILE *f = fopen(out_path, "a");
    if (!f)
    {
        return;
    }
    fputs(line, f);
    fclose(f);
}

//--------------------------------------------------------------------------------------------------------------------------------

/*
@brief Biçimlendirilmiş metni standart çıktıya yazdırır ve aynı içeriği günlük dosyasına ekler.

@param fmt printf tarzı biçim dizgesi.
@param ... Biçim dizgesine karşılık gelen değişken argümanlar.

@return Yok.
*/
void saveLogPrintf(const char *fmt, ...)
{
    char buf[4096];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    printf("%s", buf);
    saveLog(buf);
}

//--------------------------------------------------------------------------------------------------------------------------------
