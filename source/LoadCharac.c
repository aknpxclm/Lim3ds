#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "Sinner_Enemy_defin.h"

#define BinDirLen 12 // "./LimChars/" inlcuding null terminator

static unsigned int MaxLen = 0;

char *AllocPathBuf()
{
    DIR *Lim3ds;
    struct dirent *entry;
    char *Path;
    int CurrentLen = 0;

    Lim3ds = opendir("./LimChars");
    if(Lim3ds == NULL) return;

    while((entry = readdir(Lim3ds)) != NULL) //finds the length of the .bin file with the longest len
    {
        if(entry->d_type == DT_REG)
        {
            CurrentLen = strlen(entry->d_name);
            if(CurrentLen > MaxLen) MaxLen = CurrentLen + 1;
        } 
    }
    closedir(Lim3ds);

    Path = (char*)malloc(sizeof(char) * (MaxLen + BinDirLen));
    strncpy(Path, "./LimChars/", MaxLen + BinDirLen);
    Path[MaxLen + BinDirLen] = '\0'; //end of allocated mem is null terminated

    return Path;
}

void LoadSinInfo(SkillInfo *Sinner, char *Path)
{
    FILE *SinBin;
    SinBin = fopen(Path, "rb");

    fread(Sinner, sizeof(SkillInfo), 3, SinBin); //copy skill numbers for each rank 1 -> 3 from a .bin file

    fclose(SinBin);
}

void CharIdPath(int SinId, char *Path)
{
    Path[BinDirLen] = '\0'; //ignore previous path -> "./LimChars/\0....."
    switch(SinId) //add path to Id skill file
    {
        case 1:
        strncat(Path, "IshCap.bin", MaxLen); 
        break;
    }
}

void FreePath(char *Path)
{
    free(Path);
}