//
// RogueHack.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <time.h>
#include <iostream>

//Edited by Raaghav

//Things to improve:
//smaller rooms
//add ending
//amulet doesn't do anything
//inventory and help
//monster same space problem
//message disappearing

//Things Raaghav added:
//inventory changed to list instead of concatenating a single string
//help command
//message disappearing
//added function that checks if next to object
//added amulet use and temporary ending
//classes
//monster move multiple spaces at a time if player moves multiple spaces at a time but weird crashing (not on east room monsters yet) 
using namespace std;

#define SCREEN_ROWS 25
#define SCREEN_COLS 60
vector<string> screen;

vector<string> inventoryList;

#define MAP_EMPTY '.'
#define MAP_VWALL '|'
#define MAP_HWALL '='
#define MAP_PLAYER '@'
#define MAP_MONSTER 'm'
#define MAP_WEAPON 'w'
#define MAP_LADDER 'H'
#define MAP_GOGGLES '8'
#define MAP_AMULET '&'
#define MAP_POTION 'P'
#define MAP_SCROLL 'S'
#define MAP_BOX 'B'
#define MAP_BOSS 'M'
#define MAP_PORTAL '*'

string topMessage = "";
int m1r = 20;
int m1c = 50;
int m2r = 21;
int m2c = 45;
string inventory = "";
int m3r = 10;
int m3c = 13;
int em1r = 9;
int em1c = 22;
int em2r = 15;
int em2c = 7;
int em3r = 19;
int em3c = 50;
int em4r = 5;
int em4c = 30;
int em5r = 20;
int em5c = 13;
int bossR = 11;
int bossC = 30;

void initScreen()
{
	string defaultRow = "";
	screen.clear();

	for (int c = 0; c < SCREEN_COLS; c++)
		defaultRow += MAP_EMPTY;

	for (int r = 0; r < SCREEN_ROWS; r++)
		screen.push_back(defaultRow);
}

void printInventory()
{
	//cout << "Inventory:\n" << inventory << "\n";
	cout << "Inventory: ";
	if (inventoryList.size() != 0)
	{
		for (int d = 0; d < inventoryList.size(); d++)
		{
			if (d != inventoryList.size() - 1) //prints commas between each item except last item
			{
				cout << inventoryList[d] << ", ";
			}
			else
			{
				cout << inventoryList[d] << endl;
			}
		}
	}
}

void printInventoryTop()
{
	string i = "Inventory: ";
	if (inventoryList.size() != 0)
	{
		for (int d = 0; d < inventoryList.size(); d++)
		{
			if (d != inventoryList.size() - 1)
			{
				i += (inventoryList[d] + ", ");
			}
			else
			{
				i += (inventoryList[d] + "\n");
			}
		}
		cout << i << endl;
	}
}

void printScreen()
{
	cout << topMessage;
	for (int r = 0; r < SCREEN_ROWS; r++)
	{
		cout << screen[r] << endl;
	}
	printInventory();
}

void setInfo(int atk, int hp, string location, int healthPotions)
{
	stringstream buffer;

	buffer << "ATK: " << atk << ", HP: " << hp << ", LOC: " << location << ", POTIONS: " << healthPotions;
	screen[SCREEN_ROWS - 1] = buffer.str();
}

void setMap()
{
	for (int r = 0; r < SCREEN_ROWS - 1; r++)  //-1 because statInfo takes up the last row
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

bool nearObject(int myR, int myC, char object)
{
	if ((screen[myR + 1][myC] == object || screen[myR + 1][myC + 1] == object || screen[myR + 1][myC - 1] == object || screen[myR][myC + 1] == object || screen[myR][myC - 1] == object || screen[myR - 1][myC] == object || screen[myR - 1][myC + 1] == object || screen[myR - 1][myC - 1] == object))
	{
		return true;
	}
	else
	{
		return false;
	}
}


void setWeapons(string location, string hasWeapon, bool hasSuperWeapon, bool hasNightVisionGoggles, bool hasScroll, bool hasAmulet)
{
	if (location == "cave" && hasWeapon == "false")
		screen[7][47] = MAP_WEAPON;
	if (location == "beneath" && hasSuperWeapon == false && hasNightVisionGoggles == true)
		screen[9][20] = MAP_WEAPON;
	if (location == "west" && hasNightVisionGoggles == false)
		screen[3][3] = MAP_GOGGLES;
	if (location == "south" && hasScroll == false)
		screen[21][2] = MAP_SCROLL;
	if (location == "end" && hasAmulet == true)
		screen[10][10] = MAP_PORTAL;
}

void setPlayer(int r, int c)
{
	screen[r][c] = MAP_PLAYER;
}

int randomNum()
{
	srand((int)time(NULL)); //Randomize seed initialization
	int randNum = rand() % 3; // Generate a random number between 0 and 2
	randNum -= 1;
	return randNum;
}

void setMonsters(string choice, string location, int m1h, int m2h, int atk, int myR, int myC, int m3h, bool hasOpenedBox, bool hasAmulet, int em1h, int em2h, int em3h, int em4h, int em5h, int bossHp)
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
			//atk++;
			//topMessage = "You killed the monster.\n";
			cout << "You killed the monster.\n";
		}
		if (m2h > 0)
		{
			screen[m2r][m2c] = MAP_MONSTER;
		}
		else
		{
			//atk++;
			//topMessage = "You killed the monster.\n";
			cout << "You killed the mosnter.\n";
		}
		//monster movement
		if (myR > m1r && screen[m1r + 1][m1c] == MAP_EMPTY && (m1c != m2c && m1r + 1 != m2r))
		{
			if ((choice == "ww" || choice == "aa" || choice == "ss" || choice == "dd"))
			{
				for (int i = 0; i < 5; i++)
				{
					if ((screen[m1r + 1][m1c]) == MAP_EMPTY)
					{
						m1r++;
					}
				}
			}

			else
				m1r++;
		}
		else if (myR < m1r && screen[m1r - 1][m1c] == MAP_EMPTY && (m1c != m2c && m1r - 1 != m2r))
		{
			if ((choice == "ww" || choice == "aa" || choice == "ss" || choice == "dd"))
			{
				for (int i = 0; i < 5; i++)
				{
					if ((screen[m1r - 1][m1c]) == MAP_EMPTY)
					{
						m1r--;
					}
				}
			}

			else
				m1r--;
		}
		if (myC > m1c && screen[m1r][m1c + 1] == MAP_EMPTY && (m1r != m2r && m1c + 1 != m2c))
		{
			if ((choice == "ww" || choice == "aa" || choice == "ss" || choice == "dd"))
			{
				for (int i = 0; i < 5; i++)
				{
					if ((screen[m1r][m1c + 1]) == MAP_EMPTY)
					{
						m1c++;
					}
				}
			}

			else
				m1c++;
		}
		else if (myC < m1c && screen[m1r][m1c - 1] == MAP_EMPTY && (m1r != m2c && m1c - 1 != m2c))
		{
			if ((choice == "ww" || choice == "aa" || choice == "ss" || choice == "dd"))
			{
				for (int i = 0; i < 5; i++)
				{
					if ((screen[m1r][m1c - 1]) == MAP_EMPTY)
					{
						m1c--;
					}
				}
			}

			else
				m1c--;
		}

		if (myR > m2r && screen[m2r + 1][m2c] == MAP_EMPTY && (m1c != m2c && m2r + 1 != m1r))
		{
			if ((choice == "ww" || choice == "aa" || choice == "ss" || choice == "dd"))
			{
				for (int i = 0; i < 5; i++)
				{
					if ((screen[m2r + 1][m2c]) == MAP_EMPTY)
					{
						m2r++;
					}
				}
			}

			else
				m2r++;
		}
		else if (myR < m2r && screen[m2r - 1][m2c] == MAP_EMPTY && (m1c != m2c && m2r - 1 != m1r))
		{
			if ((choice == "ww" || choice == "aa" || choice == "ss" || choice == "dd"))
			{
				for (int i = 0; i < 5; i++)
				{
					if ((screen[m2r - 1][m2c]) == MAP_EMPTY)
					{
						m2r--;
					}
				}
			}

			else
				m2r--;
		}
		if (myC > m2c && screen[m2r][m2c + 1] == MAP_EMPTY && (m1r != m2r && m2c + 1 != m1c))
		{
			if ((choice == "ww" || choice == "aa" || choice == "ss" || choice == "dd"))
			{
				for (int i = 0; i < 5; i++)
				{
					if ((screen[m2r][m2c + 1]) == MAP_EMPTY)
					{
						m2c++;
					}
				}
			}

			else
				m2c++;
		}
		else if (myC < m2c && screen[m2r][m2c - 1] == MAP_EMPTY && (m1r != m2r && m2c - 1 != m1c))
		{
			if ((choice == "ww" || choice == "aa" || choice == "ss" || choice == "dd"))
			{
				for (int i = 0; i < 5; i++)
				{
					if ((screen[m2r][m2c - 1]) == MAP_EMPTY)
					{
						m2c--;
					}
				}
			}

			else
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
		if (myR > m3r && screen[m3r + 1][m3c] == MAP_EMPTY)
		{
			if ((choice == "ww" || choice == "aa" || choice == "ss" || choice == "dd"))
			{
				for (int i = 0; i < 5; i++)
				{
					if ((screen[m3r + 1][m3c]) == MAP_EMPTY)
					{
						m3r++;
					}
				}
			}

			else
				m3r++;
		}
		else if (myR < m3r && screen[m3r - 1][m3c] == MAP_EMPTY)
		{
			if ((choice == "ww" || choice == "aa" || choice == "ss" || choice == "dd"))
			{
				for (int i = 0; i < 5; i++)
				{
					if ((screen[m3r - 1][m3c]) == MAP_EMPTY)
					{
						m3r--;
					}
				}
			}
			else
				m3r--;
		}
		if (myC > m3c && screen[m3r][m3c + 1] == MAP_EMPTY)
		{
			if ((choice == "ww" || choice == "aa" || choice == "ss" || choice == "dd"))
			{
				for (int i = 0; i < 5; i++)
				{
					if ((screen[m3r][m3c + 1]) == MAP_EMPTY)
					{
						m3c++;
					}
				}
			}
			else
				m3c++;
		}
		else if (myC < m3c && screen[m3r][m3c - 1] == MAP_EMPTY)
		{
			if ((choice == "ww" || choice == "aa" || choice == "ss" || choice == "dd"))
			{
				for (int i = 0; i < 5; i++)
				{
					if ((screen[m3r][m3c - 1]) == MAP_EMPTY)
					{
						m3c--;
					}
				}
			}
			else
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
	if (location == "east")
	{
		//set monsters
		if (em1h > 0)
		{
			screen[em1r][em1c] = MAP_MONSTER;
		}
		else
		{
			atk++;
			topMessage = "You killed the monster.\n";
		}
		if (em2h > 0)
		{
			screen[em2r][em2c] = MAP_MONSTER;
		}
		else
		{
			atk++;
			topMessage = "You killed the monster.\n";
		}
		if (em3h > 0)
		{
			screen[em3r][em3c] = MAP_MONSTER;
		}
		else
		{
			atk++;
			topMessage = "You killed the monster.\n";
		}
		if (em4h > 0)
		{
			screen[em4r][em4c] = MAP_MONSTER;
		}
		else
		{
			atk++;
			topMessage = "You killed the monster.\n";
		}
		if (em5h > 0)
		{
			screen[em5r][em5c] = MAP_MONSTER;
		}
		else
		{
			atk++;
			topMessage = "You killed the monster.\n";
		}
		if (bossHp > 0)
		{
			screen[bossR][bossC] = MAP_MONSTER;
		}
		else
		{
			atk++;
			topMessage = "You killed the boss. Congrats! You can now use the door in the back of the room.\n";
		}
		//monster 1 movement
		if (myR > em1r && screen[em1r + 1][em1c] == MAP_EMPTY)
		{
			em1r++;
		}
		else if (myR < em1r && screen[em1r - 1][em1c] == MAP_EMPTY)
		{
			em1r--;
		}
		if (myC > em1c && screen[em1r][em1c + 1] == MAP_EMPTY)
		{
			em1c++;
		}
		else if (myC < em1c && screen[em1r][em1c - 1] == MAP_EMPTY)
		{
			em1c--;
		}
		//monster 2 movement
		if (myR > em2r && screen[em2r + 1][em2c] == MAP_EMPTY)
		{
			em2r++;
		}
		else if (myR < em2r && screen[em2r - 1][em2c] == MAP_EMPTY)
		{
			em1r--;
		}
		if (myC > em2c && screen[em2r][em2c + 1] == MAP_EMPTY)
		{
			em2c++;
		}
		else if (myC < em2c && screen[em2r][em2c - 1] == MAP_EMPTY)
		{
			em2c--;
		}
		//monster 3 movement
		if (myR > em3r && screen[em3r + 1][em3c] == MAP_EMPTY)
		{
			em1r++;
		}
		else if (myR < em3r && screen[em3r - 1][em3c] == MAP_EMPTY)
		{
			em3r--;
		}
		if (myC > em3c && screen[em3r][em3c + 1] == MAP_EMPTY)
		{
			em3c++;
		}
		else if (myC < em3c && screen[em3r][em3c - 1] == MAP_EMPTY)
		{
			em3c--;
		}
		//monster 4 movement
		if (myR > em4r && screen[em4r + 1][em4c] == MAP_EMPTY)
		{
			em4r++;
		}
		else if (myR < em4r && screen[em4r - 1][em4c] == MAP_EMPTY)
		{
			em4r--;
		}
		if (myC > em4c && screen[em4r][em4c + 1] == MAP_EMPTY)
		{
			em4c++;
		}
		else if (myC < em4c && screen[em4r][em4c - 1] == MAP_EMPTY)
		{
			em4c--;
		}
		//monster 5 movement
		if (myR > em5r && screen[em5r + 1][em5c] == MAP_EMPTY)
		{
			em5r++;
		}
		else if (myR < em5r && screen[em5r - 1][em5c] == MAP_EMPTY)
		{
			em5r--;
		}
		if (myC > em5c && screen[em5r][em5c + 1] == MAP_EMPTY)
		{
			em5c++;
		}
		else if (myC < em5c && screen[em5r][em5c - 1] == MAP_EMPTY)
		{
			em5c--;
		}
		//monster boss movement
		if (myR > bossR && screen[bossR + 1][bossC] == MAP_EMPTY)
		{
			bossR++;
		}
		else if (myR < bossR && screen[bossR - 1][bossC] == MAP_EMPTY)
		{
			bossR--;
		}
		if (myC > bossC && screen[bossR][bossC + 1] == MAP_EMPTY)
		{
			bossC++;
		}
		else if (myC < bossC && screen[bossR][bossC - 1] == MAP_EMPTY)
		{
			bossC--;
		}

	}
}

void setInventory(int healingPotions, string hasSword, bool hasNightVisionGoggles, bool hasSuperWeapon, bool hasScroll, bool hasAmulet)
{
	//inventory = "";
	inventoryList.clear();

	if (healingPotions > 0)
	{
		//inventory += to_string(healingPotions);
		//inventory += " Healing Potions, ";
		inventoryList.push_back(to_string(healingPotions) + " Healing_Potions");
	}
	if (hasSword == "true")
	{
		//inventory += "Sword, ";
		inventoryList.push_back("Sword");
	}
	if (hasNightVisionGoggles == true)
	{
		inventory += "Night Vision Goggles, ";
		inventoryList.push_back("Night_Vision_Goggles");
	}
	if (hasSuperWeapon == true)
	{
		inventory += "Broadsword of the Divine, ";
		inventoryList.push_back("Broadsword_of_the_Divine");
	}
	if (hasScroll == true)
	{
		inventory += "Scroll, ";
		inventoryList.push_back("Scroll");
	}

	if (hasAmulet == true)
	{
		inventory += "Amulet, ";
		inventoryList.push_back("Amulet");
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
		//end
		screen[0][51] = MAP_LADDER;
		screen[0][52] = MAP_LADDER;
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
	if (location == "end")
	{
		//back to upstairs
		screen[SCREEN_ROWS - 2][51] = MAP_LADDER;
		screen[SCREEN_ROWS - 2][52] = MAP_LADDER;
	}
}

int main()
{
	string choice = "";
	string classChoice = "";
	string location = "cave";
	int myR = 2;
	int myC = 3;
	string isNearSword = "false";
	int atk;
	string enemy1Nearby = "false";
	string enemy2Nearby = "false";
	int health;
	int m1h = 20;
	int m2h = 20;
	string hasSword = "false";
	//topMessage = "You find yourself in a cave.\n";
	int healingPotions;
	bool hasNightVisionGoggles = false;
	bool hasSuperWeapon = false;
	bool isNearSuperSword = false;
	int m3h = 25;
	bool isNearGoggles = false;
	bool hasScroll = false;
	bool isNearScroll = false;
	bool isNearBox = false;
	bool isNearPortal = false;
	bool hasAmulet = false;
	bool hasOpenedBox = false;
	int em1h = 20;
	int em2h = 25;
	int em3h = 22;
	int em4h = 33;
	int em5h = 42;
	int bossHp = 300;

	cout << "You find yourself in a cave.\n";

	while (classChoice != "wizard" && classChoice != "knight" && classChoice != "healer")
	{
		cout << "Would you like to be a wizard, a knight, or a healer?\n";
		cin >> classChoice;
		if (classChoice != "wizard" && classChoice != "knight" && classChoice != "healer")
		{
			cout << "Please enter a valid choice.\n";
		}
		else if (classChoice == "wizard")
		{
			atk = 5;
			health = 45;
			healingPotions = 5;
		}
		else if (classChoice == "knight")
		{
			atk = 3;
			health = 60;
			healingPotions = 10;
		}
		else if (classChoice == "healer")
		{
			atk = 4;
			health = 20;
			healingPotions = 10;
		}
	}



	while (choice != "q" && choice != "Q" && health > 0)
	{

		initScreen();
		setInfo(atk, health, location, healingPotions);
		setMap();
		setPlayer(myR, myC);
		setMonsters(choice, location, m1h, m2h, atk, myR, myC, m3h, hasOpenedBox, hasAmulet, em1h, em2h, em3h, em4h, em5h, bossHp);
		setExit(myR, myC, location);
		setWeapons(location, hasSword, hasSuperWeapon, hasNightVisionGoggles, hasScroll, hasAmulet);
		setInventory(healingPotions, hasSword, hasNightVisionGoggles, hasSuperWeapon, hasScroll, hasAmulet);


		printScreen();

		cout << ">> ";
		cin >> choice;

		system("cls");

		if (choice == "a" && screen[myR][myC - 1] == MAP_EMPTY)
		{
			myC -= 1;
		}
		else if (choice == "d" && (screen[myR][myC + 1] == MAP_EMPTY))
		{
			myC += 1;
		}
		else if (choice == "w" && (screen[myR - 1][myC] == MAP_EMPTY))
		{
			myR -= 1;
		}
		else if (choice == "s" && (screen[myR + 1][myC] == MAP_EMPTY))
		{
			myR += 1;
		}
		else if ((choice == "as" || choice == "sa") && (screen[myR + 1][myC - 1] == MAP_EMPTY))
		{
			myC -= 1;
			myR += 1;
		}
		else if ((choice == "aw" || choice == "wa") && (screen[myR - 1][myC - 1] == MAP_EMPTY))
		{
			myR -= 1;
			myC -= 1;
		}
		else if ((choice == "sd" || choice == "ds") && (screen[myR + 1][myC + 1] == MAP_EMPTY))
		{
			myR += 1;
			myC += 1;
		}
		else if ((choice == "dw" || choice == "wd") && (screen[myR - 1][myC + 1] == MAP_EMPTY))
		{
			myR -= 1;
			myC += 1;
		}
		else if (choice == "ww")
		{
			for (int i = 0; i < 5; i++)
			{
				if ((screen[myR - 1][myC]) == MAP_EMPTY)
				{
					myR--;
				}
			}
		}
		else if (choice == "aa")
		{
			for (int i = 0; i < 5; i++)
			{
				if ((screen[myR][myC - 1]) == MAP_EMPTY)
				{
					myC--;
				}
			}
		}
		else if (choice == "ss")
		{
			for (int i = 0; i < 5; i++)
			{
				if ((screen[myR + 1][myC]) == MAP_EMPTY)
				{
					myR++;
				}
			}
		}
		else if (choice == "dd")
		{
			for (int i = 0; i < 5; i++)
			{
				if ((screen[myR][myC + 1]) == MAP_EMPTY)
				{
					myC++;
				}
			}
		}
		else if (choice == "l" || choice == "look" || choice == "info")
		{
			if (location == "cave")
			{
				//topMessage = "You see a shadow in the corner. There is additionally a slight shimmer infront of you.\n";
				cout << "You see a shadow in the corner. There is additionally a slight shimmer infront of you.\n";
			}
			if (location == "upstairs")
			{
				//topMessage = "There are four doors. Where do they lead?\n";
				cout << "There are four doors. Where do they lead?\n";
			}
			if (location == "beneath")
			{
				if (hasNightVisionGoggles == true)
				{
					//topMessage = "The room is barren aside from a large sword laying on the floor.\n";
					cout << "The room is barren aside from a large sword laying on the floor.\n";
				}
				else
				{
					//topMessage = "Your eyes are filled with vast darkness.\n";
					cout << "Your eyes are filled with vast darkness.\n";
				}
			}
			if (location == "end")
			{
				if (hasAmulet == true)
				{
					cout << "It looks like a portal has appeared...\n";
				}
				else
				{
					cout << "There is nothing in this room. Perhaps you should come back later...\n";
				}
			}
		}
		else if (choice == "i" || choice == "inventory" || choice == "I" || choice == "Inventory")
		{
			printInventoryTop();
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
		else if (choice == "help")
		{
			cout << "List of commands : (All commands must be followed with enter)\n"
				<< "W - move up one space\n"
				<< "S - move down one space\n"
				<< "A - move left one space\n"
				<< "D - move right one space\n"
				<< "WW - move up five spaces\n"
				<< "SS - move down five spaces\n"
				<< "AA - move left five spaces\n"
				<< "DD - move right five spaces\n"
				<< "WD (or DW) - move diagonally one space up and to the right\n"
				<< "WA (or AW) - move diagonally one space up and to the left\n"
				<< "SD (or DS) - move diagonally one space down and to the right\n"
				<< "SA (or AS) - move diagonally one space down and to the left\n"
				<< "I - view inventory\n"
				<< "Heal - use a healing potion\n"
				<< "L - get info about a room\n"
				<< "Help - list of commands and objects"
				<< "Q - quit game\n"
				<< "\n"
				<< "List of objects:\n"
				<< "@ - your player\n"
				<< ". - empty space on map\n"
				<< "| - vertical wall\n"
				<< "= - horizontal wall\n"
				<< "+ - a door\n"
				<< "W - a weapon\n"
				<< "H - a ladder or door\n"
				<< "8 - a pair of night vision goggles\n"
				<< "& - a magical amulet\n"
				<< "P - a potion\n"
				<< "S - a scroll\n"
				<< "B - a box\n"
				<< "m - a monster\n"
				<< "M - a boss monster\n"
				<< "* - a magical portal\n"
				;
			/*cout << "List of commands: (All commands must be followed with enter)\n"
			<< "W - move up one space\n"
			<< "S - move down one space\n"
			<< "A - move left one space\n"
			<< "D - move right one space\n"
			<< "WW - move up five spaces\n"
			<< "SS - move down five spaces\n"
			<< "AA - move left five spaces\n"
			<< "DD - move right five spaces\n"
			<< "WD (or DW) - move diagonally one space up and to the right\n"
			<< "WA (or AW) - move diagonally one space up and to the left\n"
			<< "SD (or DS) - move diagonally one space down and to the right\n"
			<< "SA (or AS) - move diagonally one space down and to the left\n"
			<< "I - view inventory\n"
			<< "Heal - use a healing potion\n"
			<< "L - get info about a room\n"
			<< "Help - list of commands and objects"
			<< "Q - quit game\n"
			<< "\n"
			<< "List of objects:\n"
			<< "@ - your player\n"
			<< ". - empty space on map\n"
			<< "| - vertical wall\n"
			<< "= - horizontal wall\n"
			<< "+ - a door\n"
			<< "W - a weapon\n"
			<< "H - a ladder or door\n"
			<< "8 - a pair of night vision goggles\n"
			<< "& - a magical amulet\n"
			<< "P - a potion\n"
			<< "S - a scroll\n"
			<< "B - a box\n"
			<< "m - a monster\n"
			<< "M - a boss monster\n"
			;*/
		}
		else if (choice == "cheats")
		{
			location = "upstairs";
		}
		else if (choice != "climb" || choice != "open" || choice != "door" || choice != "take" || choice != "Purple56")
		{
			//topMessage = "I do not understand that.\n";
			cout << "I do not understand that.\n";
		}
		//locations
		if (location == "cave")
		{
			if (nearObject(myR, myC, MAP_WEAPON))
			{
				isNearSword = "true";
				//topMessage = "You are near a weapon.\n";
				cout << "You are near a weapon.\n";
			}
			if (nearObject(myR, myC, MAP_MONSTER))
			{
				if ((myR == m1r || myR - 1 == m1r || myR + 1 == m1r) && (myC == m1c || myC == m1c - 1 || myC == m1c + 1))
				{
					enemy1Nearby = "true";

				}
				else if ((myR == m2r || myR - 1 == m2r || myR + 1 == m2r) && (myC == m2c || myC == m2c - 1 || myC == m2c + 1))
				{
					enemy2Nearby = "true";
				}
				health -= 7;
				//topMessage = "There is a monster nearby!\n";
				cout << "There is a monster nearby!\n";
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
			if (nearObject(myR, myC, MAP_LADDER))
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
				//topMessage = "You are next to a ladder.\n";
				cout << "You are next to a ladder.\n";
			}
		}
		else if (location == "upstairs")
		{
			//topMessage = "You enter another monster infested room. Darn.\n";
			cout << "You enter another monster infested room. Darn.\n";
			if (nearObject(myR, myC, MAP_LADDER) && (choice == "c" || choice == "climb" || choice == "up" || choice == "down" || choice == "through" || choice == "open" || choice == "door"))
			{
				if (myR == 1)
				{

					if (myC == 31 || myC == 32)
					{
						myR = 22;
						location = "north";
					}
					else
					{
						myR = 22;
						location = "end";
					}
				}
				else if (myR == 22)
				{
					if (myC == 31 || myC == 32)
					{
						myR = 1;
						location = "south";
					}
					else if (myC == 53 || myC == 54 || myC == 55 || myC == 56)
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
			if (nearObject(myR, myC, MAP_MONSTER))
			{
				health -= 7;
				//topMessage = "There is a monster nearby!\n";
				cout << "There is a monster nearby!\n";
				if (choice == "atk" || choice == "attack")
				{
					m3h -= atk;
				}
			}
		}
		else if (location == "beneath")
		{
			if (hasNightVisionGoggles == true)
			{
				//topMessage = "You can now see in the darkness.\n";
				cout << "You can now see in the darkness.\n";
				if (screen[myR + 1][myC] == MAP_WEAPON || screen[myR + 1][myC + 1] == MAP_WEAPON || screen[myR + 1][myC - 1] == MAP_WEAPON || screen[myR][myC + 1] == MAP_WEAPON || screen[myR][myC - 1] == MAP_WEAPON || screen[myR - 1][myC] == MAP_WEAPON || screen[myR - 1][myC + 1] == MAP_WEAPON || screen[myR - 1][myC - 1] == MAP_WEAPON)
				{
					isNearSuperSword = "true";
					//topMessage = "You are near a powerful weapon.\n";
					cout << "You are near a powerful weapon.\n";
				}
				if ((choice == "t" || choice == "take") && (isNearSuperSword == true))
				{
					atk += 50;
					//topMessage = "Well done, you have found the Broadsword of the Divine. Your attack damage has been increased by 50!\n";
					cout << "Well done, you have found the Broadsword of the Divine. Your attack damage has been increased by 50!\n";
					hasSuperWeapon = "true";
				}
			}
			else
				//topMessage = "You descend the ladder into a dark room. You cannot see anything.\n";
				cout << "You descend the ladder into a dark room. You cannot see anything.\n";
			if (nearObject(myR, myC, MAP_LADDER))
			{
				//topMessage = "You are next to a ladder.\n";
				cout << "You are next to a ladder.\n";
				if (choice == "c" || choice == "climb" || choice == "up")
				{
					location = "cave";

				}
			}
		}
		else if (location == "north")
		{
			if (nearObject(myR, myC, MAP_LADDER))
			{
				//topMessage = "You are next to a ladder.\n";
				cout << "You are next to a ladder.\n";
				if (choice == "through" || choice == "door" || choice == "use" || choice == "open")
				{
					location = "upstairs";
					myR = 1;

				}
			}
			if (nearObject(myR, myC, MAP_BOX))
			{
				isNearBox = true;
				//topMessage = "You are near a password protected box. Enter the password to unlock it.\n";
				cout << "You are near a password protected box. Enter the password to unlock it.\n";
			}
			if ((choice == "Purple56") && (isNearBox == true))
			{
				hasOpenedBox = true;
				//topMessage = "An amulet has appeared on the ground. Maybe it has a function?\n";
				cout << "An amulet has appeared on the ground. Maybe it has a function?\n";
			}
			if (nearObject(myR, myC, MAP_AMULET))
			{
				//topMessage = "You are near an amulet.\n";
				cout << "You are near an amulet.\n";
				if (choice == "take" || choice == "t")
				{
					//topMessage = "You have taken the amulet.\n";
					cout << "You have taken the amulet.\n";
					hasAmulet = true;
				}
			}
		}
		else if (location == "east")
		{
			if (nearObject(myR, myC, MAP_LADDER))
			{
				if (choice == "through" || choice == "door" || choice == "use" || choice == "open")
				{
					location = "upstairs";
					myC = 58;
				}
				//topMessage = "You are next to a ladder.\n";
				cout << "You are next to a ladder.\n";
			}

		}
		else if (location == "south")
		{
			if (nearObject(myR, myC, MAP_LADDER))
			{
				if (choice == "through" || choice == "door" || choice == "use" || choice == "open")
				{
					location = "upstairs";
					myR = 22;
				}
				//topMessage = "You are next to a ladder.\n";
				cout << "You are next to a ladder.\n";
			}
			if (nearObject(myR, myC, MAP_SCROLL))
			{
				isNearScroll = true;
				//topMessage = "You are near an important scroll.\n";
				cout << "You are near an important scroll.\n";
			}
			if ((choice == "t" || choice == "take") && (isNearScroll == true))
			{
				hasScroll = true;
				//topMessage = "The password is Purple56.\n";
				cout << "The password is Purple56.\n";
			}
		}
		else if (location == "west")
		{
			if (nearObject(myR, myC, MAP_LADDER))
			{
				if (choice == "through" || choice == "door" || choice == "use" || choice == "open")
				{
					location = "upstairs";
					myC = 1;
				}
				//topMessage = "You are next to a ladder.\n";
				cout << "You are next to a ladder.\n";
			}
			if (nearObject(myR, myC, MAP_GOGGLES))
			{
				isNearGoggles = true;
				//topMessage = "You are near night vision goggles.\n";
				cout << "You are near night vision goggles.\n";
			}
			if ((choice == "t" || choice == "take") && (isNearGoggles == true))
			{
				hasNightVisionGoggles = true;
			}
		}
		else if (location == "end")
		{
			if (hasAmulet == true)
			{
				//topMessage = "You can now see in the darkness.\n";
				cout << "A portal magically appears.\n";
				if (screen[myR + 1][myC] == MAP_PORTAL || screen[myR + 1][myC + 1] == MAP_PORTAL || screen[myR + 1][myC - 1] == MAP_PORTAL || screen[myR][myC + 1] == MAP_PORTAL || screen[myR][myC - 1] == MAP_PORTAL || screen[myR - 1][myC] == MAP_PORTAL || screen[myR - 1][myC + 1] == MAP_PORTAL || screen[myR - 1][myC - 1] == MAP_PORTAL)
				{
					isNearPortal = "true";
					//topMessage = "You are near a powerful weapon.\n";
					cout << "You are near a powerful portal. You can feel the magic!\n";
				}
				if ((choice == "through" || choice == "open") && (isNearPortal == true))
				{
					//temporary ending
					cout << "YOU HAVE WON!!!\n";
					string wait;
					cin >> wait;
				}
			}
			else
			{
				//topMessage = "You descend the ladder into a dark room. You cannot see anything.\n";
				cout << "This room feels strange...\n";
			}
			if (nearObject(myR, myC, MAP_LADDER))
			{
				//topMessage = "You are next to a ladder.\n";
				cout << "You are next to a ladder.\n";
				if (choice == "c" || choice == "climb" || choice == "up" || choice == "open" || choice == "door")
				{
					myR = 22;
					location = "upstairs";

				}
			}
		}
		if (health <= 0)
		{
			//topMessage = "You died!\n";
			cout << "You died!\n";
		}

	}
	return 0;
}

