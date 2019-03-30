#pragma once
#ifndef obiekty_hpp
#define obiekty_hpp

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

//Externy


//DEBUG MODE ACTIVATE
bool debug = 1;

bool bStraconeZycie = 0;

long long int punkty = 0;

long long int nowepunkty = punkty;

int zycia = 3;

bool ShowFPS = 1;

Clock tickclock;
Time ticktime;
int tick = 0;

int fps = 0;
Clock FPSClock;
Time FPSTime;

unsigned int maxpoints = 0;
unsigned long long int ActualCoins = 0;

float AktualnaIloscCzasu = 0;



View view;

RenderWindow okno;






//Minigry
enum minigry
{
	menu,
	EkranZPunktami,
	BiegZPrzeszkodami,
	ScieranieTablicy,
	HarryCotter,
	ZlowieszczeGabki
};
/*
enum XandY
{
	x,
	y
};*/

//Aktualna MiniGra
minigry AktualnaMinigra = menu;

//Czy dziala
bool running = 0;

//Font
Font font_arial;
Font font_dk;

//Tryb wyswietlania
VideoMode vidmode;
int resX;
int resY;

//Domyœlne wartoœci
int startowailoscczasu = 4;
int defResX = 1920;
int defResY = 1080;

//Domyslne bindy
Keyboard::Key bind_ruchwprawo = Keyboard::Right;
Keyboard::Key bind_ruchwlewo = Keyboard::Left;
Keyboard::Key bind_skok = Keyboard::Space;
//ExternEND

#endif // !obiekty_hpp
