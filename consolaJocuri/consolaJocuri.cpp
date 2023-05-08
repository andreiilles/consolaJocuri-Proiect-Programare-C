#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

using namespace std;

struct game
{
	char gameName[255];
	double gameSize;
}gamesInstalled[100],gamesAvailable[100];

const double consoleStorage = 1024;
double availableStorage = consoleStorage;
int installedGamesCount = 0;
int availableGamesCount = 0;

void printMainMenu(double availableStorage, const double consoleStorage);
void saveInstalledGamesToFile(int* installedGamesCount, struct game* gamesInstalled);
void saveAvailableGamesToFile(int* availableGamesCount, struct game* gamesAvailable);

void loadInstalledGamesFromFile(int* installedGamesCount, double* availableStorage, struct game* gamesInstalled)
{
	FILE* fileInput;
	fileInput = fopen("installedGames.txt", "r");
	int installedGamesTemp = 0;
	int gamesRead = 0;
	int temp = 0;
	if (fileInput != NULL)
	{
		fscanf(fileInput, "%d", &installedGamesTemp);
		fscanf(fileInput, "%d", &temp);
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
			*availableStorage -=  gamesInstalled[gamesRead].gameSize;
			fscanf(fileInput, "%d", &temp);
		}
		fclose(fileInput);
	}
	else
	{
		printf("Eroare la accesarea stocarii!\n");
		exit(0);
	}
}

void loadAvailableGamesFromFile(int* availableGamesCount, struct game* gamesAvailable)
{
	FILE* fileInput;
	fileInput = fopen("availableGames.txt", "r");
	int gamesRead = 0;
	int temp = 0;
	if (fileInput != NULL)
	{
		fscanf(fileInput, "%d", availableGamesCount);
		fscanf(fileInput, "%d", temp);
		for (int i = 1;i <= (*availableGamesCount);i++)
		{
			gamesRead++;
			char tempName[255];
			if (fgets(tempName, 255, fileInput) != NULL)
			{
				tempName[strcspn(tempName, "\n")] = 0;
				strcpy(gamesAvailable[gamesRead].gameName, tempName);
			}
			fscanf(fileInput, "%lf", &gamesAvailable[gamesRead].gameSize);
			fscanf(fileInput, "%d", &temp);
		}
		fclose(fileInput);
	}
	else
	{
		printf("Eroare la accesarea stocarii!\n");
		exit(0);
	}
}

void installGame(int* gameIndex, double* availableStorage, int* installedGamesCount, int* availableGamesCount, struct game* gamesInstalled, struct game* gamesAvailable)
{
	double installSize = gamesAvailable[*gameIndex].gameSize;
	int tempIndex = *gameIndex;
	if (*availableStorage - installSize >= 0)
	{
		(*installedGamesCount)++;
		(*availableGamesCount)--;
		(*availableStorage) -= installSize;
		if (gamesAvailable[tempIndex].gameName != NULL)
		{
			strcpy(gamesInstalled[*installedGamesCount].gameName, gamesAvailable[tempIndex].gameName);
			*gamesAvailable[tempIndex].gameName = NULL;
			for (int i = tempIndex;i <= *availableGamesCount;i++)
			{
				strcpy(gamesAvailable[i].gameName, gamesAvailable[i + 1].gameName);
				gamesAvailable[i].gameSize = gamesAvailable[i + 1].gameSize;				
			}
		}
		else
			printf("Nume joc null!\n");
		gamesInstalled[*installedGamesCount].gameSize = gamesAvailable[tempIndex].gameSize;
		printf("Instalat cu succes!");
		Sleep(2000);
		saveInstalledGamesToFile(installedGamesCount, gamesInstalled);
		saveAvailableGamesToFile(availableGamesCount, gamesAvailable);
		printMainMenu(*availableStorage, consoleStorage);
	}
	else
	{
		printf("Nu exista destul spatiu disponibil!");
	}
}

void uninstallGame(int* gameIndex, double* availableStorage, int* installedGamesCount, int* availableGamesCount, struct game* gamesInstalled, struct game* gamesAvailable)
{
	double uninstallSize = gamesInstalled[*gameIndex].gameSize;
	int tempIndex = *gameIndex;
	(*installedGamesCount)--;
	(*availableGamesCount)++;
	if (gamesInstalled[tempIndex].gameName != NULL)
	{
		*availableStorage += uninstallSize;
		strcpy(gamesAvailable[*availableGamesCount].gameName, gamesInstalled[tempIndex].gameName);
		gamesAvailable[*availableGamesCount].gameSize = gamesInstalled[tempIndex].gameSize;
		*gamesInstalled[tempIndex].gameName = NULL;
		for (int i = tempIndex;i <= *installedGamesCount;i++)
		{
			strcpy(gamesInstalled[i].gameName, gamesInstalled[i + 1].gameName);
			gamesInstalled[i].gameSize = gamesInstalled[i + 1].gameSize;
		}
		printf("Dezinstalat cu succes!");
		Sleep(2000);
		saveInstalledGamesToFile(installedGamesCount, gamesInstalled);
		saveAvailableGamesToFile(availableGamesCount, gamesAvailable);
		printMainMenu(*availableStorage, consoleStorage);
	}
}

void saveInstalledGamesToFile(int* installedGamesCount, struct game* gamesInstalled)
{
	FILE* fileOutput;
	int gamesWritten = 0;
	fileOutput = fopen("installedGames.txt", "w");
	if (fileOutput != NULL)
	{
		gamesWritten++;
		fprintf(fileOutput,"%d\n", *installedGamesCount);
		for (int i = 1;i <= *installedGamesCount;i++)
		{
			if (gamesInstalled[gamesWritten].gameName!=NULL)
			{
				fprintf(fileOutput, "%s\n", gamesInstalled[gamesWritten].gameName);
				fprintf(fileOutput, "%lf\n", gamesInstalled[gamesWritten].gameSize);
			}			
			gamesWritten++;
		}
	}
	else
	{
		printf("Eroare la accesarea stocarii!\n");
		exit(0);
	}
}

void saveAvailableGamesToFile(int* availableGamesCount, struct game* gamesAvailable)
{
	FILE* fileOutput;
	int gamesWritten = 0;
	fileOutput = fopen("availableGames.txt", "w");
	if (fileOutput != NULL)
	{
		gamesWritten++;
		fprintf(fileOutput, "%d\n", *availableGamesCount);
		for (int i = 1;i <= *availableGamesCount;i++)
		{
			if (gamesAvailable[gamesWritten].gameName != NULL)
			{
				fprintf(fileOutput, "%s\n", gamesAvailable[gamesWritten].gameName);
				fprintf(fileOutput, "%lf\n", gamesAvailable[gamesWritten].gameSize);
			}
			gamesWritten++;
		}
		fclose(fileOutput);
	}
	else
	{
		printf("Eroare la accesarea stocarii!\n");
		exit(0);

	}
}

void printInstalledGames()
{
	char optionSelected;
	printf("-----Installed games count: %d ----\n", installedGamesCount);
	for (int i = 1; i <= installedGamesCount; i++)
	{
		printf("---%d. %s %.1f GB --- \n", i, gamesInstalled[i].gameName, gamesInstalled[i].gameSize);
	}
	printf("-------------Q.Inapoi--------------\n");
	printf("---Selecteaza indexul jocului de dezinstalat: ---\n");
	while (1)
	{
		if (kbhit())
		{
			optionSelected = getch();
			if (optionSelected == 'Q' || optionSelected == 'q')
			{
				system("cls");
				printMainMenu(availableStorage, consoleStorage);
			}
			else
			{
				int tempInt = optionSelected - '0';
				uninstallGame(&tempInt, &availableStorage, &installedGamesCount, &availableGamesCount, gamesInstalled, gamesAvailable);
			}
		}
	}
}

void printAvailableGames()
{
	char optionSelected;
	printf("-----Available games count: %d ----\n", availableGamesCount);
	for (int i = 1; i <= availableGamesCount; i++)
	{
		printf("----- %d. %s %.1f GB --- \n", i, gamesAvailable[i].gameName, gamesAvailable[i].gameSize);
	}
	printf("-------------Q.Inapoi--------------\n");
	printf("---Selecteaza indexul jocului de instalat: ---\n");
	while (1)
	{
		if (kbhit())
		{
			optionSelected = getch();
			if (optionSelected == 'Q' || optionSelected == 'q')
			{
				system("cls");
				printMainMenu(availableStorage, consoleStorage);
			}
			else
			{
				int tempInt = optionSelected-'0';
				installGame(&tempInt, &availableStorage, &installedGamesCount, &availableGamesCount, gamesInstalled, gamesAvailable);
			}
		}
	}
}

void printMainMenu(double availableStorage, const double consoleStorage)
{
	char optionSelected;
	system("cls");
	printf("------Consola jocuri video v2------\n");
	printf("----------%.1f/%.1f GB----------\n", availableStorage, consoleStorage);
	printf("-----1.Afisare jocuri instalate----\n");
	printf("-----2.Afisare jocuri disponibile--\n");
	printf("-------------Q.Iesire--------------\n");
	printf("-----------------------------------\n");
	while (1)
	{
		if (kbhit())
		{
			optionSelected = getch();
			if (optionSelected == 'Q' || optionSelected == 'q')
			{
				saveInstalledGamesToFile(&installedGamesCount, gamesInstalled);
				saveAvailableGamesToFile(&availableGamesCount, gamesAvailable);
				exit(0);
			}
			else if (optionSelected == '1')
			{
				system("cls");
				printInstalledGames();
				break;
			}
			else if (optionSelected == '2')
			{
				system("cls");
				printAvailableGames();
			}
		}
	}
}

int main()
{
	loadInstalledGamesFromFile(&installedGamesCount, &availableStorage, gamesInstalled);
	loadAvailableGamesFromFile(&availableGamesCount, gamesAvailable);
	printMainMenu(availableStorage, consoleStorage);
	return 0;
}