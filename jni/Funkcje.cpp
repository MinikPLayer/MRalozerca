#include "Funkcje.hpp"
//#include "Obiekty.hpp"
#include <iostream>

/*GotowySprite::GotowySprite(string txtname)
{
	texture.loadFromFile(txtname);
	sprite.setTexture(texture);
}*/

void GotowySprite::setPosition(Vector2f position)
{
	sprite.setPosition(position.x , position.y);
}

void GotowySprite::setPosition(float posX, float posY)
{
	sprite.setPosition(posX, posY);
}

void GotowySprite::move(Vector2f offset)
{
	sprite.move(offset.x, offset.y);
}

void GotowySprite::move(float moveX, float moveY)
{
	sprite.move(moveX, moveY);
}

void GotowySprite::setScale(float offsetX, float offsetY)
{
	sprite.setScale(offsetX, offsetY);
}

void GotowySprite::setScale(float factor)
{
	sprite.setScale(factor, factor);
}



void GotowySprite::setTexture(string txtname)
{
	texture.loadFromFile(txtname);
	sprite.setTexture(texture);
}

void GotowySprite::draw(RenderWindow * okno)
{
	//sprite.setTexture(texture);
	okno->draw(sprite);
}

void GotowySprite::draw(RenderWindow * okno, int arg)
{
	sprite.setTexture(texture);
	okno->draw(sprite);
}


GotowyText::GotowyText(std::string startup_text, int CharSize, Text::Style startup_style, Color clr, Font startup_font)
{
	str = startup_text;
	size = CharSize;
	style = startup_style;
	color = clr;
	font = startup_font;
	text.setFont(font);
	text.setString(str);
	text.setCharacterSize(size);
	text.setStyle(style);
	text.setFillColor(color);
}

GotowyText::GotowyText(std::string startup_text, int CharSize, Text::Style startup_style, Color clr, std::string FontFile)
{
    str = startup_text;
	size = CharSize;
	style = startup_style;
	color = clr;
	font.loadFromFile(FontFile);
	text.setFont(font);
	text.setString(str);
	text.setCharacterSize(size);
	text.setStyle(style);
	text.setFillColor(color);
}

GotowyText::GotowyText() {}

void GotowyText::draw(RenderWindow * okno)
{
	okno->draw(text);
}

void GotowyText::setString(std::string newstring)
{
    str = newstring;
	text.setString(str);
}

Textury_BiegZPrzeszkodami::Textury_BiegZPrzeszkodami()
{
	MRal.loadFromFile(FolderDanych + "MRal.png");
	KoncowyZjazd.loadFromFile(FolderDanych + "koniecbiegu.png");
	tlo.loadFromFile(FolderDanych + "tlo.png");
	for (int i = 0; i < BiegZPrzeszkodami_ilosctexturprzeszkod; i++)
	{
		string Przeszkoda_filename = FolderDanych + "przeszkoda" + to_string(i) + ".png";
		Przeszkoda[i].loadFromFile(Przeszkoda_filename);
	}
	for (int i = 0; i < BiegZPrzeszkodami_ilosctexturozdobtla; i++)
	{
		string ozdoba_filename = FolderDanych + "ozdobatla" + to_string(i) + ".png";
		OzdobaTla[i].loadFromFile(ozdoba_filename);
	}

	cout << "Koniec wczytywania textur" << endl;
}

void Textury_BiegZPrzeszkodami::WczytajDomyslneTextury()
{
	
}

Textury_EkranZWynikami::Textury_EkranZWynikami()
{
	for (int i = 0; i < EkranZWynikami_ilosctexturMRalow; i++)
	{
		MRal[i].loadFromFile(FolderDanych + "MRal (" + to_string(i + 1) + ").png");
	}
	zycie.loadFromFile(FolderDanych + "zycie.png");
	SmutnyMRal.loadFromFile(FolderDanych + "SmutnyMRal.png");
}

void obslugazdarzen(Event zdarzenie)
{
	if (zdarzenie.type == zdarzenie.Closed)
	{
		okno.close();
		exit(0);
	}
	if (zdarzenie.type == zdarzenie.Resized)
	{
		// update the view to the new size of the window
		//sf::FloatRect visibleArea(0, 0, zdarzenie.size.width, zdarzenie.size.height);
		//view.setSize(Vector2f(1080.0 * (zdarzenie.size.width / (float)zdarzenie.size.height), 1080));
		//okno->setView(sf::View(view));
	}
}

void wycentrujobiekt(Text & text, int wysokosc)
{
	FloatRect textrect = text.getGlobalBounds();
	text.setOrigin(textrect.left + textrect.width / 2.0f, textrect.top + textrect.height / 2.0f);
	text.setPosition(Vector2f(defResX / 2.0f, defResY / 2.0f + (wysokosc)));
}

void wycentrujobiekt(Sprite & sprite, int wysokosc)
{
	FloatRect spriterect = sprite.getGlobalBounds();
	sprite.setOrigin(spriterect.left + spriterect.width / 2.0f, spriterect.top + spriterect.height / 2.0f);
	sprite.setPosition(Vector2f(defResX / 2.0f, defResY / 2.0f + (wysokosc)));
}

void konfigurujnapis(Text & text, string text_string, int size, int pozycjaY, Text::Style styl, Color kolor, Font * wybranaczcionka)
{
	text.setFont(*wybranaczcionka);
	text.setCharacterSize(size);
	text.setStyle(styl);
	text.setString(text_string);
	text.setFillColor(kolor);
	wycentrujobiekt(text, pozycjaY);
}


bool RysujHUD(float CzasNaWykonanie, float startowyczasnawykonanie)
{
	bool bAlreadyLost = 0;

	RectangleShape TloPaskaPostepu;
	RectangleShape PasekPostepu;
	RectangleShape PasekZadan;

	const int WysokoscPrzycisku = WysokoscPaskowHUD / 2;

	const int WielkoscPrzyciskuPauzy = 2 * WysokoscPrzycisku;

	if (CzasNaWykonanie * 1920 / startowyczasnawykonanie - WielkoscPrzyciskuPauzy >= 0)
	{
		PasekPostepu.setSize(Vector2f(CzasNaWykonanie * 1920 / startowyczasnawykonanie - WielkoscPrzyciskuPauzy, WysokoscPrzycisku));
	}
	else
	{
		PasekPostepu.setSize(Vector2f(0, WysokoscPrzycisku));
		bAlreadyLost = 1;
	}
	PasekZadan.setSize(Vector2f(1920, WysokoscPrzycisku * 2));
	TloPaskaPostepu.setSize(Vector2f(1920 - WielkoscPrzyciskuPauzy, WysokoscPrzycisku));

	PasekPostepu.setPosition(Vector2f(WielkoscPrzyciskuPauzy, WysokoscPrzycisku));
	TloPaskaPostepu.setPosition(Vector2f(WielkoscPrzyciskuPauzy, WysokoscPrzycisku));

	PasekPostepu.setFillColor(Color(230, 230, 230));
	TloPaskaPostepu.setFillColor(Color(100, 100, 100));
	PasekZadan.setFillColor(Color(150, 150, 150));




	okno.draw(PasekZadan);
	okno.draw(TloPaskaPostepu);
	okno.draw(PasekPostepu);
	return bAlreadyLost;
}

double ustalskale(bool X0Y1)
{
	if (X0Y1 == 0)
	{
		return resX / 1920.0;
	}
	else
		return resY / 1080.0;
}

bool debug_write(int min_write_ticks)
{
	if (debug)
	{
		if (tick >= min_write_ticks)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else 
		return 0;
}

bool debug_write(int min_write_ticks, bool czyscic)
{
	if (debug)
	{
		if (tick >= min_write_ticks)
		{
			tick = 0;
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
		return 0;
}

void tickupdate()
{
	if (tick == 0) tickclock.restart();
	ticktime = tickclock.getElapsedTime();
	tick = ticktime.asMilliseconds();
	if (tick == 0) tick++;
}

void ustalfps(GotowyText & FPS)
{
	FPSTime = FPSClock.getElapsedTime();
	if (FPSTime.asSeconds() > 1)
	{
		FPSClock.restart();
		FPSTime = FPSClock.getElapsedTime();
		FPS.setString("FPS: " + to_string(fps));
		fps = 0;
	}
	if (FPSTime.asMicroseconds() > 0)
	{
		fps++;
	}
}

void straconezycie()
{
	zycia--;
	bStraconeZycie = 1;
}

short loadConfig()
{
	ifstream plik;
	plik.open("config.ini");
	if (!plik.good()) return 1;
	plik >> resX;
	plik >> resY;
	plik >> maxpoints;
	plik >> ActualCoins;
	return 0;
}

short SaveConfig()
{
	return 0;
}

void przyspieszgre(float & AktualnaIloscCzasu)
{
	if (AktualnaIloscCzasu > MinimalnaIloscCzasu)
	{
		AktualnaIloscCzasu -= PredkoscPrzyspieszeniaGry;
	}
	else
		AktualnaIloscCzasu = MinimalnaIloscCzasu;
}

Textury_ScieranieTablicy::Textury_ScieranieTablicy()
{
	for (int i = 0; i < ScieranieTablicy_ilosctxtMRalow; i++)
	{
		MRal[i].loadFromFile(FolderDanych + "MRal (" + to_string(i + 1) + ").png");
	}
	for (int i = 0; i < ScieranieTablicy_ilosctxtOzdob; i++)
	{
		ozdoba[i].loadFromFile(FolderDanych + "ozdoba (" + to_string(i + 1) + ").png");
	}
	tlo.loadFromFile(FolderDanych + "tlo.png");
	SmutnyMRal.loadFromFile(FolderDanych + "SmutnyMRal.png");
	Gabka.loadFromFile(FolderDanych + "gabeczka.png");
}

Textury_HarryCatter::Textury_HarryCatter()
{
	MRal.loadFromFile(FolderDanych + "MRal.png");
	tlo.loadFromFile(FolderDanych + "tlo.png");
	pocisk.loadFromFile(FolderDanych + "pilka.png");
}

Textury_WyscigMRalow::Textury_WyscigMRalow()
{
	MRal.loadFromFile(FolderDanych + "MRal.png");
	ZlyMRal.loadFromFile(FolderDanych + "ZlyMRal.png");
	tlo.loadFromFile(FolderDanych + "tlo.png");
	Meta.loadFromFile(FolderDanych + "koniec.png");
}
