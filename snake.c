#include<stdio.h>
#include<conio.h>
#include<windows.h>
#include<time.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
#define Width 20
#define Height 20

void welcomeScreen();
void showMenu();
void difficultyMenu();
void pauseGame();
void setup();
void draw();
void input();
void logic();
void setCursorPosition(int x, int y);
void gameOverMenu();
void instructions();

void loadHighScore();
void saveHighScore();

int gameOver;
int x,y;
int fruitX , fruitY;
int score;
int highScore;
int tailRow[100];
int tailColumn[100];

int nTail;
int gameSpeed;
HANDLE hConsole;
int difficulty;
int menuChoice = 0;
time_t startTime;

int goldenFruitX = -1;
int goldenFruitY = -1;

time_t goldenSpawnTime;
int goldenVisible = 0;

int poisonFruitX = -1;
int poisonFruitY = -1;

int poisonVisible = 0;
time_t poisonSpawnTime;

int obstacleRow[30];
int obstacleColumn[30];
int obstacleCount;

int menuOption = 0;

enum Direction
{
	STOP = 0,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

enum Direction dir;

int main()
{
	srand(time(0));
	
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	welcomeScreen();
	while(1)
	{
        if(menuChoice != 1)
    	{
    	    showMenu();
    	}
    	
        setup();
        
    	while(!gameOver)
    	{
        	draw();
        	input();
        	logic();
        	
        	Sleep(gameSpeed);
    	}
    	gameOverMenu();
	}
	
    return 0;
}

void showMenu()
{

    SetConsoleTitle("Snake Game - By Raghav");

    while(1)
    {
        system("cls");

        SetConsoleTextAttribute(hConsole,11);
        printf("=====================================\n");
        printf("           SNAKE GAME\n");
        printf("=====================================\n\n");

        SetConsoleTextAttribute(hConsole,10);
        if(menuOption==0)
        {
            printf("➤ Start Game\n");
        }
        else
        {
            printf("  Start Game\n");
        }

        SetConsoleTextAttribute(hConsole,14);
        if(menuOption==1)
        {
            printf("➤ Difficulty\n");
        }
        else
        {
            printf("  Difficulty\n");
        }

        SetConsoleTextAttribute(hConsole,13);
        if(menuOption==2)
        {
            printf("➤ High Score\n");
        }
        else
        {
            printf("  High Score\n");
        }

        SetConsoleTextAttribute(hConsole,11);
        if(menuOption==3)
        {
            printf("➤ Instructions\n");
        }
        else
        {
            printf("  Instructions\n");
        }
        
        SetConsoleTextAttribute(hConsole,12);
        if(menuOption==4)
        {
            printf("➤ Exit\n");
        }
        else
        {
            printf("  Exit\n");
        }

        SetConsoleTextAttribute(hConsole,15);
        int key = getch();

        if(key==224)
        {
            key=getch();
        }

        if(key==72)
        {
            menuOption--;

            if(menuOption<0)
            menuOption=4;
        }

        if(key==80)
        {
            menuOption++;

            if(menuOption>4)
                menuOption=0;
        }

        if(key==13)
        {
            switch(menuOption)
            {
                case 0:
                    menuChoice=1;
                    return;

                case 1:
                    difficultyMenu();
                    
                    if(menuChoice==1)
                    {
                        return;
                    }
                    break;

                case 2:
                    loadHighScore();

                    system("cls");

                    printf("High Score : %d",highScore);

                    getch();

                    break;

                case 3:
                    instructions();
                    break;

                case 4:
                    exit(0);
            }
        }
    }
}

void instructions()
{
	PlaySound(TEXT("sounds\\click.wav.wav"), NULL, SND_FILENAME | SND_ASYNC);
	
    system("cls");

    printf("=========== CONTROLS ===========\n\n");

    printf("W  -> Move Up\n");
    printf("S  -> Move Down\n");
    printf("A  -> Move Left\n");
    printf("D  -> Move Right\n");
    printf("P  -> Pause Game\n");
    printf("Q  -> Quit Game\n");

    printf("\nEat (*) to increase score.\n");
    printf("Don't hit your own tail.\n");

    printf("\nPress any key to go back...");

    getch();
}

void difficultyMenu()
{
    PlaySound(TEXT("sounds\\click.wav.wav"), NULL, SND_FILENAME | SND_ASYNC);
    int choice;

    while(1)
    {
        system("cls");

        SetConsoleTextAttribute(hConsole,11);
        printf("=========== DIFFICULTY ===========\n\n");

        SetConsoleTextAttribute(hConsole,10);
        printf("1. Easy\n");

        SetConsoleTextAttribute(hConsole,14);
        printf("2. Medium\n");

        SetConsoleTextAttribute(hConsole,12);
        printf("3. Hard\n");

        SetConsoleTextAttribute(hConsole,15);
        printf("4. Back\n\n");

        printf("Enter Choice : ");
        scanf("%d",&choice);

        switch(choice)
        {
            case 1:
                difficulty = 1;
                menuChoice = 1;
                PlaySound(TEXT("sounds\\click.wav.wav"), NULL, SND_FILENAME | SND_ASYNC);
                return;

            case 2:
                difficulty = 2;
                menuChoice = 1;
                PlaySound(TEXT("sounds\\click.wav.wav"), NULL, SND_FILENAME | SND_ASYNC);
                return;

            case 3:
                difficulty = 3;
                menuChoice = 1;
                PlaySound(TEXT("sounds\\click.wav.wav"), NULL, SND_FILENAME | SND_ASYNC);
                return;

            case 4:
            	PlaySound(TEXT("sounds\\click.wav.wav"), NULL, SND_FILENAME | SND_ASYNC);
                return;

            default:
                printf("Invalid Choice!");
                PlaySound(TEXT("sounds\\fah.wav.wav"), NULL, SND_FILENAME | SND_ASYNC);
                Sleep(1000);
        }
    }
}

void setup()
{
	goldenFruitX = -1;
	goldenFruitY = -1;

    goldenVisible = 0;
    goldenSpawnTime = time(NULL);
	
	poisonFruitX = -1;
	poisonFruitY = -1;

    poisonVisible = 0;
    poisonSpawnTime = time(NULL);
	
	system("cls");
	
	gameOver = 0;
	nTail = 0;
	
	dir = STOP;
	
	x = Width / 2;
	y = Height / 2;
	
	fruitX = rand() % Width;
	fruitY = rand() % Height;
	
	score = 0;
	
	loadHighScore();
	
	if(difficulty == 1)
	{
    	gameSpeed = 150;
	}
	else if(difficulty == 2)
	{
	    gameSpeed = 100;
	}
	else
	{
	    gameSpeed = 60;
	}
	if(difficulty==1)
	{
	    obstacleCount = 0;
	}
	else if(difficulty==2)
	{
	    obstacleCount = 5;
	}
	else
	{
	    obstacleCount = 10;
	}
		
	startTime = time(NULL);
	
	int i, j;
	int valid;
	
	for(i = 0; i < obstacleCount; i++)
	{
    	do
    	{
        	valid = 1;
		
    	    obstacleRow[i] = rand() % Height;
    	    obstacleColumn[i] = rand() % Width;
    	    
    	    if(obstacleRow[i] == x && obstacleColumn[i] == y)
    	    valid = 0;
		
    	    if(obstacleRow[i] == fruitX && obstacleColumn[i] == fruitY)
            valid = 0;
	
    	    for(j = 0; j < i; j++)
        	{
        	    if(obstacleRow[i] == obstacleRow[j] && obstacleColumn[i] == obstacleColumn[j])
            	{
                valid = 0;
            	}
                break;
            }
        }
		while(!valid);
	}
}

void draw()
{
	setCursorPosition(0, 0);
	int i;
	int j;
	int k;
	
	for(i = 0; i < Width + 2; i++) 
	{
		SetConsoleTextAttribute(hConsole, 9);
		printf("#");
	}
	
	printf("\n");
	
	for(i = 0; i < Height; i++)
	{
		SetConsoleTextAttribute(hConsole,9);
		printf("#");
		
		for(j = 0; j < Width; j++)
		{
			int isTail = 0;
                
            for(k = 0; k < nTail; k++)
            {
                if(tailRow[k] == i && tailColumn[k] == j)
                {
                    isTail = 1;
                	break;
                }
            }
            
			if(i == x && j == y)
			{
				SetConsoleTextAttribute(hConsole, 10);
				printf("@");
			}
			else if(isTail)
			{
				SetConsoleTextAttribute(hConsole, 10);
				printf("O");
			}
			else if(i == fruitX && j == fruitY)
			{
				SetConsoleTextAttribute(hConsole, 12);
				printf("$");
			}
			else if(goldenVisible && i == goldenFruitX && j == goldenFruitY)
			{
                if((time (NULL) % 2 == 0))
                {
				    SetConsoleTextAttribute(hConsole, 14);
				    printf("&");
			    }
            }
            else if(poisonVisible && i == poisonFruitX && j == poisonFruitY)
            {
                SetConsoleTextAttribute(hConsole,13);

                printf("X");
            }
			else
			{
				SetConsoleTextAttribute(hConsole, 15);
                printf(" ");
			}
			
		}
		
		SetConsoleTextAttribute(hConsole, 9);
		printf("#");
		
		printf("\n");
	}
	
	for(i = 0; i < Width + 2; i++)
	{
		SetConsoleTextAttribute(hConsole,9);
		printf("#");
	}
	
	printf("\n");
	
	int currentLevel = (120 - gameSpeed) / 5 + 1;
	
	time_t currentTime = time(NULL);

	int elapsed = (int)(currentTime - startTime);
	
	int minutes = elapsed / 60;
	int seconds = elapsed % 60;
	
	SetConsoleTextAttribute(hConsole,14);
	printf("Score: %d  |  High Score: %d  |  Level: %d\n",score, highScore, currentLevel);
	
	printf("Length : %d\n", nTail + 1);
	
	printf("Time : %02d:%02d\n", minutes, seconds);
	
	SetConsoleTextAttribute(hConsole,15);
}

void input()
{
	int key;
	if (kbhit())
	{
		key = getch();
		
		switch(key)
		{
			case 'a':
				dir = LEFT;
				break;
			
			case 'd':
				dir = RIGHT;
				break;
			
			case 'w':
				dir = UP;
				break;
			
			case 's':
				dir = DOWN;
				break;
				
			case 'p':
			case 'P':
    			pauseGame();
    			break;
			
			case 'q':
    			gameOver = 1;
    			break;
		}
	}	
}

void logic()
{
    time_t currentTime = time(NULL);

    if(!poisonVisible && currentTime - poisonSpawnTime >= 35)
    {
        poisonFruitX = rand() % Height;
        poisonFruitY = rand() % Width;

        poisonVisible = 1;
        poisonSpawnTime = currentTime;
    }

    if(poisonVisible && currentTime - poisonSpawnTime >= 8)
    {
        poisonVisible = 0;

        poisonFruitX = -1;
        poisonFruitY = -1;

        poisonSpawnTime = currentTime;
    }

    int i;

    int prevX = x;
    int prevY = y;

    if(!goldenVisible && currentTime - goldenSpawnTime >= 20)
    {
        goldenFruitX = rand() % Height;
        goldenFruitY = rand() % Width;

        goldenVisible = 1;
        goldenSpawnTime = currentTime;
    }

    if(goldenVisible && currentTime - goldenSpawnTime >= 5)
    {
        goldenVisible = 0;
        goldenFruitX = -1;
        goldenFruitY = -1;

        goldenSpawnTime = currentTime;
    }

    switch(dir)
    {
        case LEFT:
            y--;
            break;

        case RIGHT:
            y++;
            break;

        case UP:
            x--;
            break;

        case DOWN:
            x++;
            break;
    }

    if(x >= Height)
        x = 0;
    else if(x < 0)
        x = Height - 1;

    if(y >= Width)
        y = 0;
    else if(y < 0)
        y = Width - 1;

    if(nTail > 0)
    {
        int tempX, tempY;

        for(i = 0; i < nTail; i++)
        {
            tempX = tailRow[i];
            tempY = tailColumn[i];

            tailRow[i] = prevX;
            tailColumn[i] = prevY;

            prevX = tempX;
            prevY = tempY;
        }
    }

    for(i = 0; i < nTail; i++)
    {
        if(x == tailRow[i] && y == tailColumn[i])
        {
        	PlaySound(TEXT("sounds\\fah.wav.wav"), NULL, SND_FILENAME | SND_ASYNC);
            gameOver = 1;
        }
    }
    
    if(x == fruitX && y == fruitY)
    {
        PlaySound(TEXT("sounds\\eat.wav.wav"), NULL, SND_FILENAME | SND_ASYNC);

        score++;
        
        if(score > highScore)
		{
    		highScore = score;
    		saveHighScore();
		}

        if(goldenVisible && x == goldenFruitX && y == goldenFruitY)
        {
            score += 5;

            PlaySound(TEXT("sounds\\eat.wav.wav"),NULL,SND_FILENAME | SND_ASYNC);

            goldenVisible = 0;
            goldenFruitX = -1;
            goldenFruitY = -1;
        }
        if(poisonVisible && x == poisonFruitX && y == poisonFruitY)
        {
            PlaySound(TEXT("sounds\\fah.wav.wav"),NULL,SND_FILENAME | SND_ASYNC);
        
            score -= 3;

            if(score < 0)
            {
            score = 0;
            }

            if(nTail >= 2)
            {
                nTail -= 2;
            }
            else
            {
                gameOver = 1;
            }
            poisonVisible = 0;

            poisonFruitX = -1;
            poisonFruitY = -1;
        }
		
        nTail++;

        if(gameSpeed > 30)
            gameSpeed -= 5;

        while(1)
        {
            int ok = 1;

            fruitX = rand() % Width;
            fruitY = rand() % Height;

            if(fruitX == x && fruitY == y)
                ok = 0;

            for(i = 0; i < nTail; i++)
            {
                if(fruitX == tailRow[i] && fruitY == tailColumn[i])
                {
                    ok = 0;
                    break;
                }
            }

            if(ok)
                break;
        }
    }
}

void setCursorPosition(int x, int y)
{
    COORD coord = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void pauseGame()
{
	PlaySound(TEXT("sounds\\click.wav.wav"), NULL, SND_FILENAME | SND_ASYNC);
    setCursorPosition(0, Height + 4);

    printf("\n===== GAME PAUSED =====");
    printf("\nPress P to Resume...");

    while(1)
    {
        if(kbhit())
        {
            char ch = getch();

            if(ch=='p' || ch=='P')
            {
                break;
            }
        }

        Sleep(50);
    }

    setCursorPosition(0, Height + 4);
    printf("                          ");
    setCursorPosition(0, Height + 5);
    printf("                          ");
}

void loadHighScore()
{
    FILE *fp;

    fp = fopen("highscore.txt", "r");

    if(fp == NULL)
    {
        highScore = 0;
        return;
    }

    fscanf(fp, "%d", &highScore);

    fclose(fp);
}

void saveHighScore()
{
    FILE *fp;

    fp = fopen("highscore.txt", "w");

    if(fp == NULL)
        return;

    fprintf(fp, "%d", highScore);

    fclose(fp);
}

void setColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void gameOverMenu()
{
    system("cls");

    PlaySound(TEXT("sounds\\fah.wav.wav"), NULL, SND_FILENAME | SND_ASYNC);

    SetConsoleTextAttribute(hConsole,12);

    printf("\n\n");

    printf("#########################################\n");
    printf("              GAME OVER\n");
    printf("#########################################\n\n");

    SetConsoleTextAttribute(hConsole,14);

    printf("Final Score : %d\n", score);
    printf("High Score  : %d\n\n", highScore);

    printf("1. Play Again\n");
    printf("2. Main Menu\n");
    printf("3. Exit\n\n");

    printf("Enter Choice : ");

    int choice;

    scanf("%d",&choice);

    switch(choice)
    {
        case 1:
            menuChoice = 1;
            return;

        case 2:
            menuChoice = 0;
            return;

        case 3:
            exit(0);
    }
}

void welcomeScreen()
{
    system("cls");

    SetConsoleTextAttribute(hConsole, 10);

    printf("\n");

    Sleep(120);
    printf("                    /^\\/^\\\\\n");

    Sleep(120);
    printf("                  _|__|  O|\n");

    Sleep(120);
    printf("         \\/     /~     \\_/ \\\\\n");

    Sleep(120);
    printf("          \\____|__________/  \\\\\n");

    Sleep(120);
    printf("                 \\_______      \\\\\n");

    Sleep(120);
    printf("                         \\      \\\\\n");

    Sleep(120);
    printf("                          |      |\n");

    Sleep(120);
    printf("                         /       /\n");

    Sleep(120);
    printf("                  ____--/      /\n");

    Sleep(120);
    printf("           _____-/            /\n");

    Sleep(120);
    printf("          /__________________/\n");

    Sleep(300);

    SetConsoleTextAttribute(hConsole,14);

    printf("\n");
    printf("====================================================\n");

    SetConsoleTextAttribute(hConsole,11);

    printf("                 S N A K E   G A M E\n");

    SetConsoleTextAttribute(hConsole,14);

    printf("====================================================\n");

    SetConsoleTextAttribute(hConsole,10);

    printf("\n");
    printf("             Developed by Raghav Chaturvedi\n");

    SetConsoleTextAttribute(hConsole,15);

    printf("\n\nInitializing Snake Engine...\n\n");

    printf("[");

    int i;

    for(i=0;i<30;i++)
    {
        printf("%c",219);
        Sleep(70);
    }

    printf("]");

    printf("\n\nLoading Assets...\n");

    printf("[");

    for(i=0;i<30;i++)
    {
        printf("%c",219);
        Sleep(40);
    }

    printf("]");

    printf("\n\nPreparing Game...\n");

    printf("[");

    for(i=0;i<30;i++)
    {
        printf("%c",219);
        Sleep(25);
    }

    printf("]");
            
    Sleep(500);

    printf("\n\nReady...");

    Sleep(700);

    printf("\n3");
    Sleep(500);

    printf("\n2");
    Sleep(500);

    printf("\n1");
    Sleep(500);

    PlaySound(TEXT("sounds\\gamestart.wav.wav"),NULL,SND_FILENAME | SND_ASYNC);

    printf("\n\nPress any key to continue...");
    getch();

    system("cls");
}
