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
#define MAP_GOGGLES '8'
#define MAP_AMULET '&'
#define MAP_POTION 'P'
#define MAP_SCROLL 'S'
#define MAP_BOX 'B'

string topMessage = "";
int m1r = 20;
int m1c = 50;
int m2r = 21;
int m2c = 45;
string inventory = "";
int m3r = 10;
int m3c = 13;


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
    cout << "Inventory:\n" << inventory << "\n";
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

void setWeapons(string location, string hasWeapon, bool hasSuperWeapon, bool hasNightVisionGoggles, bool hasScroll)
{
    if (location == "cave" && hasWeapon == "false")
        screen[7][47] = MAP_WEAPON;
    if (location == "beneath" && hasSuperWeapon == false  && hasNightVisionGoggles == true)
        screen[9][20] = MAP_WEAPON;
    if (location == "west" && hasNightVisionGoggles == false)
        screen[3][3] = MAP_GOGGLES;
    if (location == "south" && hasScroll == false)
        screen[21][2] = MAP_SCROLL;
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

void setMonsters(string location, int m1h, int m2h, int atk, int myR, int myC, int m3h, bool hasOpenedBox, bool hasAmulet)
{
    if (location == "cave")
    {
        //unused random monster movement
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
        //monster movement
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
        if (m3h > 0)
        {
            screen[m3r][m3c] = MAP_MONSTER;
        }
        else
        {
            atk++;
            topMessage = "You killed the monster.\n";
        }
        //monster movement
        if (myR > m3r && screen[m3r+1][m3c] == MAP_EMPTY)
        {
            m3r++;
        }
        else if (myR < m3r && screen[m3r-1][m3c] == MAP_EMPTY)
        {
            m3r--;
        }
        if (myC > m3c && screen[m3r][m3c+1] == MAP_EMPTY)
        {
            m3c++;
        }
        else if (myC < m3c && screen[m3r][m3c-1] == MAP_EMPTY)
        {
            m3c--;
        }
    }
    if (location == "north")
    {
        if (hasOpenedBox == false)
        {
            screen[5][31] = MAP_BOX;
        }
        else if (hasAmulet == false)
        {
            screen[5][31] = MAP_AMULET;
        }
        
    }
}
void setInventory(int healingPotions, string hasSword, bool hasNightVisionGoggles, bool hasSuperWeapon, bool hasScroll, bool hasAmulet)
{
    inventory = "";
    if (healingPotions > 0)
    {
        inventory += healingPotions;
        inventory += "Healing Potions, ";
    }
    if (hasSword == "true")
    {
        inventory += "Sword, ";
    }
    if (hasNightVisionGoggles == true)
    {
        inventory += "Night Vision Goggles, ";
    }
    if (hasSuperWeapon == true)
    {
        inventory += "Broadsword of the Divine, ";
    }
    if (hasScroll == true)
        inventory += "Scroll, ";
    if (hasAmulet == true)
        inventory += "Amulet, ";
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
        //back down to cave
        screen[23][53] = MAP_LADDER;
        screen[23][54] = MAP_LADDER;
        screen[23][55] = MAP_LADDER;
        screen[23][56] = MAP_LADDER;
    }
    if (location == "beneath")
    {
        //up to cave
        screen[20][3] = MAP_LADDER;
    }
    if (location == "south")
    {
        screen[0][31] = MAP_LADDER;
        screen[0][32] = MAP_LADDER;
    }
    if (location == "north")
    {
        screen[23][31] = MAP_LADDER;
        screen[23][32] = MAP_LADDER;
    }
    if (location == "east")
    {
        screen[11][0] = MAP_LADDER;
        screen[12][0] = MAP_LADDER;
    }
    if (location == "west")
    {
        screen[11][59] = MAP_LADDER;
        screen[12][59] = MAP_LADDER;
    }
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
    bool hasNightVisionGoggles = false;
    bool hasSuperWeapon = false;
    bool isNearSuperSword = false;
    int m3h = 25;
    bool isNearGoggles = false;
    bool hasScroll = false;
    bool isNearScroll = false;
    bool isNearBox = false;
    bool hasAmulet = false;
    bool hasOpenedBox = false;
    
    while (choice != "q" && choice != "Q" && health > 0)
    {
        system("clear");
        
        initScreen();
        setInfo(atk, health, location, healingPotions);
        setMap();
        setPlayer(myR, myC);
        setMonsters(location, m1h, m2h, atk, myR, myC, m3h, hasOpenedBox, hasAmulet);
        setExit(myR, myC, location);
        setWeapons(location, hasSword, hasSuperWeapon, hasNightVisionGoggles, hasScroll);
        setInventory(healingPotions, hasSword, hasNightVisionGoggles, hasSuperWeapon, hasScroll, hasAmulet);
        
        
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
            if (location == "upstairs")
            {
                topMessage = "There are four doors. Where do they lead?\n";
            }
            if (location == "beneath")
            {
                if (hasNightVisionGoggles == true)
                {
                    topMessage = "The room is barren aside from a large sword laying on the floor.\n";
                }
                else
                {
                    topMessage = "Your eyes are filled with vast darkness.\n";
                }
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
                atk += 15;
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
                if (choice == "c" || choice == "climb" || choice == "up" || choice == "down")
                {
                    if (myC == 53 || myC == 54 || myC == 55 || myC == 56)
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
        else if (location == "upstairs")
        {
            topMessage = "You enter another monster infested room. Darn.\n";
            if ((screen[myR+1][myC] == MAP_LADDER || screen[myR+1][myC+1] == MAP_LADDER || screen[myR+1][myC-1] == MAP_LADDER || screen[myR][myC+1] == MAP_LADDER || screen[myR][myC-1] == MAP_LADDER || screen[myR-1][myC] == MAP_LADDER|| screen[myR-1][myC+1] == MAP_LADDER || screen[myR-1][myC-2] == MAP_LADDER) && (choice == "c" || choice == "climb" || choice == "up" || choice == "down" || choice == "through" || choice == "open" || choice == "door"))
            {
                
                if (myR == 1)
                {
                    myR = 22;
                    location = "north";
                }
                else if (myR == 22)
                {
                    if (myC == 31 || myC == 32)
                    {
                        myR = 1;
                        location = "south";
                    }
                    else if (myC == 53 || myC == 54 || myC == 55 ||myC == 56)
                    {
                        location = "cave";
                    }

                }
                else if (myC == 1)
                {
                    myC = 58;
                    location = "west";
                }
                else if (myC == 58)
                {
                    myC = 1;
                    location = "east";
                }
            }
            if (screen[myR+1][myC] == MAP_MONSTER || screen[myR+1][myC+1] == MAP_MONSTER || screen[myR+1][myC-1] == MAP_MONSTER || screen[myR][myC+1] == MAP_MONSTER || screen[myR][myC-1] == MAP_MONSTER || screen[myR-1][myC] == MAP_MONSTER || screen[myR-1][myC+1] == MAP_MONSTER || screen[myR-1][myC-2] == MAP_MONSTER)
            {
                health -= 7;
                topMessage = "There is a monster nearby!\n";
                if (choice == "atk" || choice == "attack")
                {
                    m3h -= atk;
                }
            }
        }
        else if (location == "beneath")
        {
            topMessage = "You descend the ladder into a dark room. You cannot see anything.\n";
            if (hasNightVisionGoggles == true)
            {
                if (screen[myR+1][myC] == MAP_WEAPON || screen[myR+1][myC+1] == MAP_WEAPON || screen[myR+1][myC-1] == MAP_WEAPON || screen[myR][myC+1] == MAP_WEAPON || screen[myR][myC-1] == MAP_WEAPON || screen[myR-1][myC] == MAP_WEAPON || screen[myR-1][myC+1] == MAP_WEAPON || screen[myR-1][myC-2] == MAP_WEAPON)
                {
                    isNearSuperSword = "true";
                    topMessage = "You are near a powerful weapon.\n";
                }
                if ((choice == "t" || choice == "take") && (isNearSuperSword == true))
                {
                    atk += 50;
                    topMessage = "Well done, you have found the Broadsword of the Divine. Your attack damage has been increased by 50!\n";
                    hasSuperWeapon = "true";
                }
            }
            if ((screen[myR+1][myC] == MAP_LADDER || screen[myR+1][myC+1] == MAP_LADDER || screen[myR+1][myC-1] == MAP_LADDER || screen[myR][myC+1] == MAP_LADDER || screen[myR][myC-1] == MAP_LADDER || screen[myR-1][myC] == MAP_LADDER|| screen[myR-1][myC+1] == MAP_LADDER || screen[myR-1][myC-2] == MAP_LADDER))
            {
                if (choice == "c" || choice == "climb" || choice == "up")
                {
                        location = "cave";
                   
                }
                topMessage = "You are next to a ladder.\n";
            }
        }
        else if (location == "north")
        {
            if ((screen[myR+1][myC] == MAP_LADDER || screen[myR+1][myC+1] == MAP_LADDER || screen[myR+1][myC-1] == MAP_LADDER || screen[myR][myC+1] == MAP_LADDER || screen[myR][myC-1] == MAP_LADDER || screen[myR-1][myC] == MAP_LADDER|| screen[myR-1][myC+1] == MAP_LADDER || screen[myR-1][myC-2] == MAP_LADDER))
            {
                if (choice == "through" || choice == "door" || choice == "use" || choice == "open")
                {
                    location = "upstairs";
                    myR = 1;
                    
                }
                topMessage = "You are next to a ladder.\n";
            }
            if (screen[myR+1][myC] == MAP_BOX || screen[myR+1][myC+1] == MAP_BOX || screen[myR+1][myC-1] == MAP_BOX || screen[myR][myC+1] == MAP_BOX || screen[myR][myC-1] == MAP_BOX || screen[myR-1][myC] == MAP_BOX || screen[myR-1][myC+1] == MAP_BOX || screen[myR-1][myC-2] == MAP_BOX)
            {
                isNearBox = true;
                topMessage = "You are near a password protected box. Enter the password to unlock it.\n";
            }
            if ((choice == "Purple56") && (isNearBox == true))
            {
                hasOpenedBox = true;
                topMessage = "An amulet has appeared on the ground. Maybe it has a function?\n";
            }
            if (screen[myR+1][myC] == MAP_AMULET || screen[myR+1][myC+1] == MAP_AMULET || screen[myR+1][myC-1] == MAP_AMULET || screen[myR][myC+1] == MAP_AMULET || screen[myR][myC-1] == MAP_AMULET || screen[myR-1][myC] == MAP_AMULET || screen[myR-1][myC+1] == MAP_AMULET || screen[myR-1][myC-2] == MAP_AMULET)
            {
                topMessage = "You are near an amulet.\n";
                if (choice == "take" || choice == "t")
                {
                    topMessage = "You have taken the amulet.\n";
                    hasAmulet = true;
                }
            }
        }
        else if (location == "east")
        {
            if ((screen[myR+1][myC] == MAP_LADDER || screen[myR+1][myC+1] == MAP_LADDER || screen[myR+1][myC-1] == MAP_LADDER || screen[myR][myC+1] == MAP_LADDER || screen[myR][myC-1] == MAP_LADDER || screen[myR-1][myC] == MAP_LADDER|| screen[myR-1][myC+1] == MAP_LADDER || screen[myR-1][myC-2] == MAP_LADDER))
            {
                if (choice == "through" || choice == "door" || choice == "use" || choice == "open")
                {
                    location = "upstairs";
                    myC = 58;
                }
                topMessage = "You are next to a ladder.\n";
            }
            
        }
        else if (location == "south")
        {
            if ((screen[myR+1][myC] == MAP_LADDER || screen[myR+1][myC+1] == MAP_LADDER || screen[myR+1][myC-1] == MAP_LADDER || screen[myR][myC+1] == MAP_LADDER || screen[myR][myC-1] == MAP_LADDER || screen[myR-1][myC] == MAP_LADDER|| screen[myR-1][myC+1] == MAP_LADDER || screen[myR-1][myC-2] == MAP_LADDER))
            {
                if (choice == "through" || choice == "door" || choice == "use" || choice == "open")
                {
                    location = "upstairs";
                    myR = 22;
                }
                topMessage = "You are next to a ladder.\n";
            }
            if (screen[myR+1][myC] == MAP_SCROLL || screen[myR+1][myC+1] == MAP_SCROLL || screen[myR+1][myC-1] == MAP_SCROLL || screen[myR][myC+1] == MAP_SCROLL || screen[myR][myC-1] == MAP_SCROLL || screen[myR-1][myC] == MAP_SCROLL || screen[myR-1][myC+1] == MAP_SCROLL || screen[myR-1][myC-2] == MAP_SCROLL)
            {
                isNearScroll = true;
                topMessage = "You are near an important scroll.\n";
            }
            if ((choice == "t" || choice == "take") && (isNearScroll == true))
            {
                hasScroll = true;
                topMessage = "The password is Purple56.\n";
            }
        }
        else if (location == "west")
        {
            if ((screen[myR+1][myC] == MAP_LADDER || screen[myR+1][myC+1] == MAP_LADDER || screen[myR+1][myC-1] == MAP_LADDER || screen[myR][myC+1] == MAP_LADDER || screen[myR][myC-1] == MAP_LADDER || screen[myR-1][myC] == MAP_LADDER|| screen[myR-1][myC+1] == MAP_LADDER || screen[myR-1][myC-2] == MAP_LADDER))
            {
                if (choice == "through" || choice == "door" || choice == "use" || choice == "open")
                {
                    location = "upstairs";
                    myC = 1;
                }
                topMessage = "You are next to a ladder.\n";
            }
            if (screen[myR+1][myC] == MAP_GOGGLES || screen[myR+1][myC+1] == MAP_GOGGLES || screen[myR+1][myC-1] == MAP_GOGGLES || screen[myR][myC+1] == MAP_GOGGLES || screen[myR][myC-1] == MAP_GOGGLES || screen[myR-1][myC] == MAP_GOGGLES || screen[myR-1][myC+1] == MAP_GOGGLES || screen[myR-1][myC-2] == MAP_GOGGLES)
            {
                isNearGoggles = true;
                topMessage = "You are near night vision goggles.\n";
            }
            if ((choice == "t" || choice == "take") && (isNearGoggles == true))
            {
                hasNightVisionGoggles = true;
            }
        }
        if (health <= 0)
        {
            topMessage = "You died!\n";
        }
        
    }
    return 0;
}
