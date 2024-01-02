#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>

struct Entry {
    char key[128];
    wchar_t* data = NULL;
};
int main(int argc, char* argv[])
{
    if (argv[1] == NULL || argv[2] == NULL) {
        fprintf(stderr, "Insufficient arguments provided.\n");
        return 1;
    }
    FILE *datFile, *bedFile;
    char* datFilePath = argv[1];
    char* bedFilePath = argv[2];
    char line[256];
    datFile = fopen(datFilePath, "rb");
    bedFile = fopen(bedFilePath, "r");

    if (datFile == NULL || bedFile == NULL) {
        fprintf(stderr, "Error opening files.\n");
        return 1;
    }

    long lSize;
    fseek(datFile, 0, SEEK_END);
    lSize = ftell(datFile);
    rewind(datFile);

    long numEntries, strPos, strLen, filePos;

    // first 4 bytes: number of strings in the .dat
    fread(&numEntries, 1, 4, datFile);

    struct Entry* entryList = (struct Entry*)malloc(numEntries * sizeof(struct Entry));

    for (int i = 0; i < numEntries; i++) {

        // next 4 bytes: position of a string
        fread(&strPos, 1, 4, datFile);

        // next 4 bytes: length of a string
        fread(&strLen, 1, 4, datFile);
        fflush(datFile);
        filePos = ftell(datFile);
        fseek(datFile, (long)strPos, SEEK_SET);
        entryList[i].data = (wchar_t*)malloc((strLen + 1) * sizeof(wchar_t));
        fread(entryList[i].data, sizeof(wchar_t), strLen, datFile);
        entryList[i].data[strLen] = L'\0';
        fseek(datFile, filePos, SEEK_SET);
    }



    while (fgets(line, sizeof(line), bedFile) != NULL) {
        if (line[0] == '\n' || line[0] == '\0') {
            continue;
        }

        char key[128];
        int id;

        if (sscanf(line, "%127s = %d", key, &id) == 2) {
            strcpy(entryList[id].key, key);
        }
        else {
            fprintf(stderr, "Error parsing line: %s", line);
        }
    }
    for (int i = 0; i < numEntries; i++) {
        printf("%s(%d) = %ls\n", entryList[i].key, i, entryList[i].data);
        free(entryList[i].data);
    }
    fclose(bedFile);
    free(entryList);
    fclose(datFile);


}