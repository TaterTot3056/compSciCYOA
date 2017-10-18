//
// RogueHack.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <time.h>
#include <iostream>

using namespace std;

#define SCREEN_ROWS 25
#define SCREEN_COLS 60
vector<string> screen;

#define MAP_EMPTY '.'
#define MAP_VWALL '|'
#define MAP_HWALL '='
#define MAP_PLAYER '@'
#define MAP_MONSTER '~'
#define MAP_WEAPON 'w'
#define MAP_LADDER 'H'

string topMessage = "";

void initScreen()
{
    string defaultRow = "";
    screen.clear();
    
    for (int c = 0; c < SCREEN_COLS; c++)
        defaultRow += MAP_EMPTY;
    
    for (int r = 0; r < SCREEN_ROWS; r++)
        screen.push_back(defaultRow);
}

void printScreen()
{
    cout << topMessage;
    for (int r = 0; r < SCREEN_ROWS; r++)
    {
        cout << screen[r] << endl;
    }
}

void setInfo(int atk, int hp, string location)
{
    stringstream buffer;
    
    buffer << "ATK: " << atk << ", HP: " << hp << ", LOC: " << location;
    screen[SCREEN_ROWS - 1] = buffer.str();
}

void setMap()
{
    for (int r = 0; r < SCREEN_ROWS-1; r++)  //-1 because statInfo takes up the last row
    {
        screen[r][0] = MAP_VWALL;
        screen[r][SCREEN_COLS - 1] = MAP_VWALL;
    }
    
    for (int c = 0; c < SCREEN_COLS; c++)
    {
        screen[0][c] = MAP_HWALL;
        screen[SCREEN_ROWS - 2][c] = MAP_HWALL;
    }
    
}

void setWeapons(string location, string hasWeapon)
{
    if (location == "cave" && hasWeapon == "false")
        screen[7][47] = MAP_WEAPON;
}

void setPlayer(int r, int c)
{
    screen[r][c] = MAP_PLAYER;
}

int randomNum()
{
    srand( (int)time(NULL) ); //Randomize seed initialization
    int randNum = rand() % 3; // Generate a random number between 0 and 2
    randNum -= 1;
    return randNum;
}

void setMonsters(string location, int m1h, int m2h, int atk)
{
    if (location == "cave")
    {
        int m1r = 20;
        int m1c = 50;
        int m2r = 21;
        int m2c = 45;

        //m1x += randomNum();
        //m1y += randomNum();
        //m2x += randomNum();
        //m2y += randomNum();
        if (m1h > 0)
        {
            screen[m1r][m1c] = MAP_MONSTER;
        }
        else
        {
            atk += 5;
        }
        if (m2h > 0)
        {
            screen[m2r][m2c] = MAP_MONSTER;
        }
        else
        {
            atk += 5;
        }

    }
    if (location == "upstairs")
    {
        screen [10][13] = MAP_MONSTER;
    }
}
void setExit(int myR, int myC)
{
    if(screen[23][53] != MAP_PLAYER)
       screen[23][53] = MAP_LADDER;
    
    screen[23][54] = MAP_LADDER;
    screen[23][55] = MAP_LADDER;
    screen[23][56] = MAP_LADDER;

/*
    if (myR != 23 && myC != 53)
    {
    screen[23][53] = MAP_LADDER;
    }
    if (myR != 23 && myC != 54)
    {
    screen[23][54] = MAP_LADDER;
    }
    if (myR != 23 && myC != 55)
    {
    screen[23][55] = MAP_LADDER;
    }
    if (myR != 23 && myC != 56)
    {
    screen[23][56] = MAP_LADDER;
    }*/
}
int main()
{
    string choice = "";
    string location = "cave";
    int myR = 2;
    int myC = 3;
    string isNearSword = "false";
    int atk = 3;
    string enemy1Nearby = "false";
    string enemy2Nearby = "false";
    int health = 60;
    int m1h = 20;
    int m2h = 20;
    string hasSword = "false";
    topMessage = "You find yourself in a cave.\n";
    
    
    while (choice != "q" && choice != "Q" && health > 0)
    {
        system("clear");
        
        initScreen();
        setInfo(atk, health, location);
        setMap();
        setPlayer(myR, myC);
        setMonsters(location, m1h, m2h, atk);
        setExit(myR, myC);
        setWeapons(location, hasSword);
        
        
        printScreen();
        
        cout << ">> ";
        cin >> choice;
        
        if (choice == "a" && (screen[myR][myC-1] == MAP_EMPTY || screen[myR][myC-1] == MAP_LADDER))
        {
            myC -= 1;
        }
        else if (choice == "d" && (screen[myR][myC+1] == MAP_EMPTY || screen[myR][myC+1] == MAP_LADDER))
        {
            myC += 1;
        }
        else if (choice == "w" && (screen[myR-1][myC] == MAP_EMPTY || screen[myR-1][myC] == MAP_LADDER))
        {
            myR -= 1;
        }
        else if (choice == "s" && (screen[myR+1][myC] == MAP_EMPTY || screen[myR+1][myC] == MAP_LADDER))
        {
            myR += 1;
        }
        else if ((choice == "as" || choice == "sa") && (screen[myR+1][myC-1] == MAP_EMPTY || screen[myR+1][myC-1] == MAP_LADDER))
        {
            myC -= 1;
            myR +=1;
        }
        else if ((choice == "aw" || choice == "wa") && (screen[myR-1][myC-1] == MAP_EMPTY || screen[myR-1][myC-1] == MAP_LADDER))
        {
            myR -= 1;
            myC -= 1;
        }
        else if ((choice == "sd" || choice == "ds") && (screen[myR+1][myC+1] == MAP_EMPTY || screen[myR+1][myC+1] == MAP_LADDER))
        {
            myR += 1;
            myC += 1;
        }
        else if ((choice == "dw" || choice == "wd") && (screen[myR-1][myC+1] == MAP_EMPTY || screen[myR-1][myC+1] == MAP_LADDER))
        {
            myR -= 1;
            myC += 1;
        }
        else if (choice == "l" || choice == "look" || choice == "info")
        {
            if (location == "cave")
            {
                topMessage = "You see a shadow in the corner. There is additionally a slight shimmer infront of you.\n";
            }
        }
        if (location == "cave")
        {
            if (screen[myR+1][myC] == MAP_WEAPON || screen[myR+1][myC+1] == MAP_WEAPON || screen[myR+1][myC-1] == MAP_WEAPON || screen[myR][myC+1] == MAP_WEAPON || screen[myR][myC-1] == MAP_WEAPON || screen[myR-1][myC] == MAP_WEAPON || screen[myR-1][myC+1] == MAP_WEAPON || screen[myR-1][myC-2] == MAP_WEAPON)
            {
                isNearSword = "true";
            }
            if (screen[myR+1][myC] == MAP_MONSTER || screen[myR+1][myC+1] == MAP_MONSTER || screen[myR+1][myC-1] == MAP_MONSTER || screen[myR][myC+1] == MAP_MONSTER || screen[myR][myC-1] == MAP_MONSTER || screen[myR-1][myC] == MAP_MONSTER || screen[myR-1][myC+1] == MAP_MONSTER || screen[myR-1][myC-2] == MAP_MONSTER)
            {
                if ((myR == 20 || myR == 19 || myR == 21) && (myC == 49 || myC == 50 || myC == 51))
                {
                    enemy1Nearby = "true";
                }
                if ((myR == 20 || myR == 22 || myR == 21) && (myC == 44 || myC == 45 || myC == 46))
                {
                    enemy2Nearby = "true";
                }
                health -= 7;
            }
            if ((choice == "t" || choice == "take") && (isNearSword == "true"))
            {
                atk = 15;
                hasSword = "true";
            }
            if (choice == "atk" || choice == "attack")
            {
                if (enemy1Nearby == "true")
                {
                    m1h -= atk;
                }
                else if (enemy2Nearby == "true")
                {
                    m2h -= atk;
                }
            }
        }
        if (myR == 24 && (myC == 53 || myC == 54 || myC == 55 || myC == 56))
        {
            topMessage = "You climb the ladder into another monster infested room. Darn.";
            location = "upstairs";
        }
        //if (location == "upstairs")
        {
            
        }
        
    }
    return 0;
}
