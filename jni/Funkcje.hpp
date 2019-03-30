#pragma once
#ifndef Funkcje_hpp
#define Funkcje_hpp

#define KolorTablicy Color(31, 64, 53)

#include <SFML/Graphics.hpp>
#include <fstream>
//#include "Obiekty.hpp"

using namespace std;
using namespace sf;

class GotowySprite
{
public:
	Sprite sprite;
	Texture texture;
	//GotowySprite(string txtname);
	//GotowySprite();
	void setPosition(Vector2f pos);
	void setPosition(float posX, float posY);
	void move(Vector2f move);
	void move(float moveX, float moveY);
	void setScale(float factorX, float factorY);
	void setScale(float factor);
	void setTexture(string txtname);
	void draw(RenderWindow * okno);
	void draw(RenderWindow * okno, int arg);
};



const float PredkoscPrzyspieszeniaGry = 0.2;
const float MinimalnaIloscCzasu = 2.0;


//Zmienne
const short BiegZPrzeszkodami_ilosctexturprzeszkod = 3;
const short BiegZPrzeszkodami_ilosctexturozdobtla = 6;

const short ScieranieTablicy_ilosctxtMRalow = 6;
const short ScieranieTablicy_ilosctxtOzdob = 5;

const short EkranZWynikami_ilosctexturMRalow = 9;






//Textury
class Textury_BiegZPrzeszkodami
{
protected:
	string FolderDanych = "images/biegzprzeszkodami/";

public:
	Textury_BiegZPrzeszkodami();
	void WczytajDomyslneTextury();

	Texture MRal;
	Texture KoncowyZjazd;
	Texture tlo;
	Texture Przeszkoda[BiegZPrzeszkodami_ilosctexturprzeszkod];
	Texture OzdobaTla[BiegZPrzeszkodami_ilosctexturozdobtla];
};

class Textury_ScieranieTablicy
{
private:
	string FolderDanych = "images/scieranietablicy/";

public:
	Textury_ScieranieTablicy();

	Texture MRal[ScieranieTablicy_ilosctxtMRalow];
	Texture tlo;
	Texture ozdoba[ScieranieTablicy_ilosctxtOzdob];
	Texture SmutnyMRal;
	Texture Gabka;
};

class Textury_HarryCatter
{
private:
	string FolderDanych = "images/szeryfnaposterunku/";
public:
	Textury_HarryCatter();

	Texture MRal;
	Texture tlo;
	Texture pocisk;
};

class Textury_WyscigMRalow
{
private:
	string FolderDanych = "images/wyscigmralow/";
public:
	Textury_WyscigMRalow();

	Texture MRal;
	Texture ZlyMRal;
	Texture tlo;
	Texture Meta;
};

class Textury_EkranZWynikami
{
private:
	string FolderDanych = "images/ekranzwynikami/";
public:
	Textury_EkranZWynikami();
	Texture zycie;
	Texture MRal[EkranZWynikami_ilosctexturMRalow];
	Texture SmutnyMRal;
};

class GotowyText
{
public:
	string str;
	Text text;
	Font font;
	int size;
	Color color;
	Text::Style style;
	GotowyText(std::string startup_text, int CharSize, Text::Style startup_style, Color clr, Font startup_font);
	GotowyText(std::string startup_text, int CharSize, Text::Style startup_style, Color clr, std::string FontFile);
	GotowyText();
	void draw(RenderWindow * okno);
	void setString(std::string newstring);
};



const int WysokoscPaskowHUD = 30 * 2;


//Externy

extern bool debug;

const int ptsperwin = 100;

extern bool bStraconeZycie;

extern long long int punkty;

extern long long int nowepunkty;

extern int zycia;

extern bool ShowFPS;

extern Clock tickclock;
extern Time ticktime;
extern int tick;

extern int fps;
extern Clock FPSClock;
extern Time FPSTime;

extern unsigned int maxpoints;
extern unsigned long long int ActualCoins;

extern float AktualnaIloscCzasu;

//Minigry

enum XandY
{
    x,
    y
};

//Aktualna MiniGra
//extern minigry AktualnaMinigra;

//Czy dziala
extern bool running;

//Font
extern Font font_arial;
extern Font font_dk;

//Tryb wyswietlania
extern VideoMode vidmode;
extern int resX;
extern int resY;

//Domyœlne wartoœci
extern int startowailoscczasu;
extern int defResX;
extern int defResY;

extern View view;
extern RenderWindow okno;

//Domyslne bindy
extern Keyboard::Key bind_ruchwprawo;
extern Keyboard::Key bind_ruchwlewo;
extern Keyboard::Key bind_skok;
//ExternEND


void obslugazdarzen(Event zdarzenie);
void wycentrujobiekt(Text & text, int wysokosc);
void wycentrujobiekt(Sprite & sprite, int wysokosc);
void konfigurujnapis(Text & text, string text_string, int size, int pozycjaY, Text::Style styl, Color kolor, Font * wybranaczcionka);
bool RysujHUD(float CzasNaWykonanie, float startowyczasnawykonanie);
void Skaluj(Text & text);
void Skaluj(Sprite & sprite);
double ustalskale(bool X0Y1);
bool debug_write(int min_write_ticks);
bool debug_write(int min_write_ticks,bool czyscic);
void tickupdate();
void ustalfps(GotowyText & FPS);
void straconezycie();
short loadConfig();
short SaveConfig();
void przyspieszgre(float & AktualnaIloscCzasu);

#endif // !Funkcje_hpp
