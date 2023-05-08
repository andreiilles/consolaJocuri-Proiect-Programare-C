#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>

using namespace std;

struct game
{
	char gameName[255];
	double gameSize;
}gamesInstalled[100];

const double consoleStorage = 1024;
int installedGamesCount = 0;

void loadInstalledGamesFromFile(int* installedGamesCount, double* availableStorage, struct game* gamesInstalled)
{
	FILE* fileInput;
	fileInput = fopen("installedGames.txt", "r");
	int installedGamesTemp = 0;
	int gamesRead = 0;
	int tempSize = 0;
	if (fileInput != NULL)
	{
		fscanf(fileInput, "%d", &installedGamesTemp);
		fscanf(fileInput, "%d", &tempSize);
		(*installedGamesCount) = installedGamesTemp;
		for (int i = 1; i <= installedGamesTemp; i++)
		{
			gamesRead++;
			char tempName[255];
			if (fgets(tempName, sizeof(tempName), fileInput) != NULL)
			{
				tempName[strcspn(tempName, "\n")] = 0;
				strcpy(gamesInstalled[gamesRead].gameName, tempName);
			}
			fscanf(fileInput, "%lf", &gamesInstalled[gamesRead].gameSize);
			fscanf(fileInput, "%d", &tempSize);
		}
		fclose(fileInput);
	}
	else
	{
		printf("Eroare la accesarea stocarii!\n");
		exit(0);
	}
}


void printInstalledGames()
{
	printf("-----Installed games count: %d ----\n", installedGamesCount);
	for (int i = 1; i <= installedGamesCount; i++)
	{
		printf("-----%s %.1f GB --- \n", gamesInstalled[i].gameName, gamesInstalled[i].gameSize);
	}

}

void printMainMenu()
{
	char optionSelected;
	system("cls");
	printf("------Consola jocuri video v2------\n");
	printf("-----1.Afisare jocuri instalate----\n");
	printf("-----2.Afisare jocuri disponibile--\n");
	printf("-------------Q.Iesire--------------\n");
	printf("-----------------------------------\n");
	while (1)
	{
		if (kbhit())
		{
			optionSelected = getch();
			if (optionSelected == 'Q'||optionSelected=='q')
			{
				exit(0);
			}
			else if (optionSelected == '1')
			{
				system("cls");
				printInstalledGames();
				break;
			}
		}
	}
}

int main()
{
	double availableStorage = consoleStorage;
	loadInstalledGamesFromFile(&installedGamesCount, &availableStorage, gamesInstalled);
	printMainMenu();
	return 0;
}