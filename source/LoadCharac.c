#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "Sinner_Enemy_defin.h"

#define BinDirLen 16 // "romfs:/LimChars/" excluding null terminator
#define Array_Column_Len 4

static size_t MaxLen = 0;

/// @brief Searches for the directory containing sinner skill info
/// @return String of the path to the folder
char *AllocPathBuf()
{
    DIR *Lim3ds;
    struct dirent *entry;
    char *Path;
    int CurrentLen = 0;

    Lim3ds = opendir("romfs:/LimChars");
    if(Lim3ds == NULL) return NULL;

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
    strncpy(Path, "romfs:/LimChars/", (MaxLen + BinDirLen));
    Path[MaxLen + BinDirLen] = '\0'; //end of allocated mem is null terminated

    return Path;
}

/// @brief Finds and adds the Id path for loading its skill info
/// @param SinId Id to load the info of
/// @param Path String path to add the location of the Id file onto
void CharIdPath(u16 SinId, char *Path)
{
    if(SinId == 0) return;
    Path[BinDirLen + 1] = '\0'; //ignore previous path -> "romfs:/LimChars/\0....."
    switch(SinId) //add path to Id skill file
    {
        case 1:
        strncat(Path, "IshCap.bin", MaxLen); 
        break;
    }
}

/// @brief creates a buffer to store SkillInfo data in when reading from a file
/// @return Pointer to the allocated memory
SkillInfo *SkillInfoBuf()
{
    return (SkillInfo*)malloc(sizeof(SkillInfo) * Array_Column_Len); //skill struct arrays and extra to store defence skill data
}

/// @brief Loads
/// @param Sinners_Skills SkillInfo buffer
/// @param Path String Path to the file containing Id info
void LoadSinInfo(SkillInfo *Sinners_Skills, char *Path)
{
    FILE *SinBin;
    SinBin = fopen(Path, "rb");

    fread(Sinners_Skills, sizeof(SkillInfo), Array_Column_Len, SinBin); //copy skill numbers for each rank 1 -> 3, and the defence skill from a .bin file

    fclose(SinBin);
}

/// @brief Copy skill info onto a Sinner SkillInfo struct from an Id
/// @param Sinner Character struct array of the Sinners
/// @param SkillBuf Skill buffer containing the Id info
/// @param LoadOnSin Sinner struct to the load the info on
void PassInSkillInfo(SkillInfo Sinner[][Array_Column_Len], SkillInfo *SkillBuf, u8 LoadOnSin)
{
    for(int i = 0; i < Array_Column_Len; i++)
    {
        Sinner[LoadOnSin][i].coins = SkillBuf[i].coins; //can represent the type of defence skill in the fourth array for each sinner
        Sinner[LoadOnSin][i].Skillbase = SkillBuf[i].Skillbase;
        Sinner[LoadOnSin][i].SkillcoinPow = SkillBuf[i].SkillcoinPow;
    }
}

/// @brief Free the Path and Skill buffer
/// @param Path 
/// @param SkillBuf 
void FreeSkillFileInfo(char *Path, SkillInfo *SkillBuf)
{
    free(SkillBuf);
    free(Path);
}