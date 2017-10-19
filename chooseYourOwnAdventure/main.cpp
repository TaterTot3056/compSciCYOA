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
int m1r = 20;
int m1c = 50;
int m2r = 21;
int m2c = 45;

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

void setInfo(int atk, int hp, string location, int healthPotions)
{
    stringstream buffer;
    
    buffer << "ATK: " << atk << ", HP: " << hp << ", LOC: " << location << ", POTIONS: " << healthPotions;
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

void setMonsters(string location, int m1h, int m2h, int atk, int myR, int myC)
{
    if (location == "cave")
    {
        
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
            atk++;
            topMessage = "You killed the monster.\n";
        }
        if (m2h > 0)
        {
            screen[m2r][m2c] = MAP_MONSTER;
        }
        else
        {
            atk++;
            topMessage = "You killed the monster.\n";
        }
        if (myR > m1r && screen[m1r+1][m1c] == MAP_EMPTY && (m1c != m2c && m1r+1 != m2r))
        {
            m1r++;
        }
        else if (myR < m1r && screen[m1r-1][m1c] == MAP_EMPTY && (m1c != m2c && m1r-1 != m2r))
        {
            m1r--;
        }
        if (myC > m1c && screen[m1r][m1c+1] == MAP_EMPTY && (m1r != m2r && m1c+1 != m2c))
        {
            m1c++;
        }
        else if (myC < m1c && screen[m1r][m1c-1] == MAP_EMPTY && (m1r != m2c && m1c-1 != m2c))
        {
            m1c--;
        }
        
        if (myR > m2r && screen[m2r+1][m2c] == MAP_EMPTY && (m1c != m2c && m2r+1 != m1r))
        {
            m2r++;
        }
        else if (myR < m2r && screen[m2r-1][m2c] == MAP_EMPTY && (m1c != m2c && m2r-1 != m1r))
        {
            m2r--;
        }
        if (myC > m2c && screen[m2r][m2c+1] == MAP_EMPTY  && (m1r != m2r && m2c+1 != m1c))
        {
            m2c++;
        }
        else if (myC < m2c && screen[m2r][m2c-1] == MAP_EMPTY  && (m1r != m2r && m2c-1 != m1c))
        {
            m2c--;
        }
    }
    if (location == "upstairs")
    {
        screen [10][13] = MAP_MONSTER;
    }
}
void setExit(int myR, int myC, string location)
{
    if (location == "cave")
    {
        //ladders to upstairs
        screen[23][53] = MAP_LADDER;
        screen[23][54] = MAP_LADDER;
        screen[23][55] = MAP_LADDER;
        screen[23][56] = MAP_LADDER;
        //ladder to lower level
        screen[20][3] = MAP_LADDER;
        
    }
    if (location == "upstairs")
    {
        //north
        screen[0][31] = MAP_LADDER;
        screen[0][32] = MAP_LADDER;
        //east
        screen[23][31] = MAP_LADDER;
        screen[23][32] = MAP_LADDER;
        //south
        screen[11][0] = MAP_LADDER;
        screen[12][0] = MAP_LADDER;
        //west
        screen[11][59] = MAP_LADDER;
        screen[12][59] = MAP_LADDER;
    }


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
    int healingPotions = 5;

    
    while (choice != "q" && choice != "Q" && health > 0)
    {
        system("clear");
        
        initScreen();
        setInfo(atk, health, location, healingPotions);
        setMap();
        setPlayer(myR, myC);
        setMonsters(location, m1h, m2h, atk, myR, myC);
        setExit(myR, myC, location);
        setWeapons(location, hasSword);
        
        
        printScreen();
        
        cout << ">> ";
        cin >> choice;
        
        if (choice == "a" && screen[myR][myC-1] == MAP_EMPTY)
        {
            myC -= 1;
        }
        else if (choice == "d" && (screen[myR][myC+1] == MAP_EMPTY))
        {
            myC += 1;
        }
        else if (choice == "w" && (screen[myR-1][myC] == MAP_EMPTY))
        {
            myR -= 1;
        }
        else if (choice == "s" && (screen[myR+1][myC] == MAP_EMPTY))
        {
            myR += 1;
        }
        else if ((choice == "as" || choice == "sa") && (screen[myR+1][myC-1] == MAP_EMPTY))
        {
            myC -= 1;
            myR += 1;
        }
        else if ((choice == "aw" || choice == "wa") && (screen[myR-1][myC-1] == MAP_EMPTY))
        {
            myR -= 1;
            myC -= 1;
        }
        else if ((choice == "sd" || choice == "ds") && (screen[myR+1][myC+1] == MAP_EMPTY))
        {
            myR += 1;
            myC += 1;
        }
        else if ((choice == "dw" || choice == "wd") && (screen[myR-1][myC+1] == MAP_EMPTY))
        {
            myR -= 1;
            myC += 1;
        }
        else if (choice == "ww")
        {
            for (int i = 0; i < 5; i++)
            {
                if ((screen[myR-1][myC]) == MAP_EMPTY)
                {
                    myR--;
                }
            }
        }
        else if (choice == "aa")
        {
            for (int i = 0; i < 5; i++)
            {
                if ((screen[myR][myC-1]) == MAP_EMPTY)
                {
                    myC--;
                }
            }
        }
        else if (choice == "ss")
        {
            for (int i = 0; i < 5; i++)
            {
                if ((screen[myR+1][myC]) == MAP_EMPTY)
                {
                    myR++;
                }
            }
        }
        else if (choice == "dd")
        {
            for (int i = 0; i < 5; i++)
            {
                if ((screen[myR][myC+1]) == MAP_EMPTY)
                {
                    myC++;
                }
            }
        }
        else if (choice == "l" || choice == "look" || choice == "info")
        {
            if (location == "cave")
            {
                topMessage = "You see a shadow in the corner. There is additionally a slight shimmer infront of you.\n";
            }
        }
        else if (choice == "heal")
        {
            int healthSave = health + 20;
            while (health < 60 && health < healthSave)
            {
                health += 1;
            }
            healingPotions -= 1;
        }
        if (location == "cave")
        {
            if (screen[myR+1][myC] == MAP_WEAPON || screen[myR+1][myC+1] == MAP_WEAPON || screen[myR+1][myC-1] == MAP_WEAPON || screen[myR][myC+1] == MAP_WEAPON || screen[myR][myC-1] == MAP_WEAPON || screen[myR-1][myC] == MAP_WEAPON || screen[myR-1][myC+1] == MAP_WEAPON || screen[myR-1][myC-2] == MAP_WEAPON)
            {
                isNearSword = "true";
                topMessage = "You are near a weapon.\n";
            }
            if (screen[myR+1][myC] == MAP_MONSTER || screen[myR+1][myC+1] == MAP_MONSTER || screen[myR+1][myC-1] == MAP_MONSTER || screen[myR][myC+1] == MAP_MONSTER || screen[myR][myC-1] == MAP_MONSTER || screen[myR-1][myC] == MAP_MONSTER || screen[myR-1][myC+1] == MAP_MONSTER || screen[myR-1][myC-2] == MAP_MONSTER)
            {
                if ((myR == m1r || myR-1 == m1r || myR+1 == m1r) && (myC == m1c || myC == m1c-1 || myC == m1c+1))
                {
                    enemy1Nearby = "true";
                    
                }
                else if ((myR == m2r || myR-1 == m2r || myR+1 == m2r) && (myC == m2c || myC == m2c-1 || myC == m2c+1))
                {
                    enemy2Nearby = "true";
                }
                health -= 7;
                topMessage = "There is a monster nearby!\n";
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
            if ((screen[myR+1][myC] == MAP_LADDER || screen[myR+1][myC+1] == MAP_LADDER || screen[myR+1][myC-1] == MAP_LADDER || screen[myR][myC+1] == MAP_LADDER || screen[myR][myC-1] == MAP_LADDER || screen[myR-1][myC] == MAP_LADDER|| screen[myR-1][myC+1] == MAP_LADDER || screen[myR-1][myC-2] == MAP_LADDER))
            {
                if (choice == "c" || choice == "climb" || choice == "up")
                {
                    if (myR == 23)
                    {
                        location = "upstairs";
                    }
                    else
                    {
                        location = "beneath";
                    }
                }
                topMessage = "You are next to a ladder.\n";
            }
        }
        if (location == "upstairs")
        {
            topMessage = "You climb the ladder into another monster infested room. Darn.\n";
            myR = 22;
            if ((screen[myR+1][myC] == MAP_LADDER || screen[myR+1][myC+1] == MAP_LADDER || screen[myR+1][myC-1] == MAP_LADDER || screen[myR][myC+1] == MAP_LADDER || screen[myR][myC-1] == MAP_LADDER || screen[myR-1][myC] == MAP_LADDER|| screen[myR-1][myC+1] == MAP_LADDER || screen[myR-1][myC-2] == MAP_LADDER) && (choice == "c" || choice == "climb" || choice == "up"))
            {
                
                if (myR == 0)
                {
                    location = "north";
                }
                if (myR == 23)
                {
                    location = "south";
                }
                if (myC == 1)
                {
                    location = "west";
                }
                if (myC == 59)
                {
                    location = "east";
                }
            }
            
        }
        //if (location == "beneath")
        {
            
        }
        //if (location == "north")
        {
            
        }
        //if (location == "east")
        {
            
        }
        //if (location == "south")
        {
            
        }
        //if (location == "west")
        {
            
        }
        if (health <= 0)
        {
            topMessage = "You died!\n";
        }
        
    }
    return 0;
}
