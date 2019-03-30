#include "WyscigMRalow.hpp"
#include "LineShape.cpp"

Textury_WyscigMRalow textury;

const int odstepanimacjiMRala = 50;
const int iloscKlatekAnimacjiMRala = 2;
const int szerokoscSpriteMRala = 169; //Szerokosc sprite MRala w px
const int wysokoscSpriteMrala = 111;

void dodajprzeszkode(vector<Text> & przeszkody, Sprite MRal, Vector2f position)
{
	przeszkody.push_back(Text());
	przeszkody[przeszkody.size() - 1].setFont(font_dk);
	przeszkody[przeszkody.size() - 1].setCharacterSize(MRal.getGlobalBounds().height);
	przeszkody[przeszkody.size() - 1].setPosition(position);
	przeszkody[przeszkody.size() - 1].setString(to_string(rand()%10));
}

void animacjaMRala(Sprite & MRal, Clock & ZegarAnimacji, Time & CzasAnimacji)
{
	CzasAnimacji = ZegarAnimacji.getElapsedTime();
	if (CzasAnimacji.asMilliseconds() < odstepanimacjiMRala * (iloscKlatekAnimacjiMRala))
	{
		int klatka = CzasAnimacji.asMilliseconds() / odstepanimacjiMRala;
		MRal.setTextureRect(IntRect(szerokoscSpriteMRala * klatka, 0, szerokoscSpriteMRala, wysokoscSpriteMrala));
	}
	else
	{
		MRal.setTextureRect(IntRect(0, 0, szerokoscSpriteMRala, wysokoscSpriteMrala));
		ZegarAnimacji.restart();
	}
}

int Minigra_WyscigMRalow(float czasnawykonanie)
{

	bool bLost = 0;
	bool bWin = 0;

	Vector2f poz_myszy;

	const int koniecPoziomu = 8000;
	
	const int MRalMaxDown = 800;
	const int MRalMaxUp = 400;

	float RuchPionowy = 0;

	bool bButtonClicked = 0;

	bool bCrashed = 0;

	const float MRalFasterMultiplier = 1.05 + ((startowailoscczasu / czasnawykonanie) * 2) - (1 * 2);
	cout << startowailoscczasu / czasnawykonanie << endl;

	const short ilosctel = 10;

	const float predkoscZlegoMRala = 1;
	float predkoscDobregoMRala = 5;

	float predkoscRuchu = 0;

	Clock ZegarAnimacji;
	Time CzasAnimacji;

	Clock EndGameClock;
	Time EndGameTimer;

	Clock DeltaClock;
	Time DeltaTimer;
	float DeltaTime = 0;

	Sprite MRal;
	Sprite ZlyMRal;
	Sprite tlo[ilosctel];

	Sprite PrzyciskWGore;
	Sprite PrzyciskWDol;

	Sprite Meta[2];

	Texture txt_Przycisk;

	vector<Text> przeszkody;



	txt_Przycisk.create(1920, 540);

	PrzyciskWGore.setTexture(txt_Przycisk);
	PrzyciskWDol.setTexture(txt_Przycisk);
	PrzyciskWDol.setPosition(Vector2f(0, 540));

	for (int i = 0; i < ilosctel; i++)
	{
		tlo[i].setTexture(textury.tlo);
		tlo[i].setPosition(Vector2f(1920 * i, 0));
		if (i % 2 == 1)
		{
			tlo[i].setScale(-1, 1);
			tlo[i].move(1920, 0);
		}
	}

	MRal.setTexture(textury.MRal);
	ZlyMRal.setTexture(textury.ZlyMRal);

	MRal.setPosition(Vector2f(960, 600));
	ZlyMRal.setPosition(Vector2f(960, 100));

	//dodajprzeszkode(przeszkody, MRal, Vector2f(2000, 500));
	sfLine liniagorna(Vector2f(-1000, MRalMaxUp), Vector2f(1920 * ilosctel, MRalMaxUp));
	sfLine liniadolna(Vector2f(-1000, MRalMaxDown + MRal.getGlobalBounds().height), Vector2f(1920 * ilosctel, MRalMaxDown + MRal.getGlobalBounds().height));

	liniagorna.setColor(Color::White);
	liniadolna.setColor(Color::White);

	Meta[0].setTexture(textury.Meta);
	Meta[1].setTexture(textury.Meta);

	Meta[0].setOrigin(Vector2f(0, 300));
	Meta[1].setOrigin(Vector2f(0, Meta[1].getGlobalBounds().height));

	Meta[0].setPosition(Vector2f(koniecPoziomu, MRalMaxDown));
	Meta[1].setPosition(Vector2f(koniecPoziomu, MRalMaxUp));
	
	for (int i = 2000; i < koniecPoziomu - 400; i += ( rand() % 300 ) + 400)
	{
		dodajprzeszkode(przeszkody, MRal, Vector2f(i, rand() % (MRalMaxDown - MRalMaxUp) + MRalMaxUp));
	}

	DeltaClock.restart();
	
	while (true)
	{
		DeltaTimer = DeltaClock.restart();
		DeltaTime = DeltaTimer.asMicroseconds() / 1000.0;
		predkoscRuchu = (startowailoscczasu / czasnawykonanie) * DeltaTime * predkoscZlegoMRala;
		if (!bCrashed)
		{
			predkoscDobregoMRala = predkoscRuchu * MRalFasterMultiplier;
		}
		else
		{
			predkoscDobregoMRala += predkoscRuchu / 5000.0;
			if (predkoscDobregoMRala >= predkoscRuchu / DeltaTime)
			{
				predkoscDobregoMRala = predkoscRuchu;
				bCrashed = 0;
			}
		}
		Event zdarzenie;
		
		Vector2f LastPozMyszy = poz_myszy;
		if (okno.pollEvent(zdarzenie))
		{
			obslugazdarzen(zdarzenie);
			
		}
		Mouse mysz;
		if (mysz.isButtonPressed(Mouse::Left))
		{
			poz_myszy.x = mysz.getPosition().x;
			poz_myszy.y = mysz.getPosition().y;
			if (!bButtonClicked)
			{
				LastPozMyszy = poz_myszy;
			}
			bButtonClicked = 1;
			RuchPionowy = (poz_myszy.y - LastPozMyszy.y);
			//cout << "Poz_myszy: " << poz_myszy.y << " LastPozMyszy: " << LastPozMyszy.y << endl;
		}
		else
		{
			bButtonClicked = 0;
		}
		if (!bButtonClicked)
		{
			RuchPionowy = 0;
		}
		animacjaMRala(MRal, ZegarAnimacji, CzasAnimacji);
		animacjaMRala(ZlyMRal, ZegarAnimacji, CzasAnimacji);
		view.setCenter(MRal.getGlobalBounds().left + MRal.getGlobalBounds().width / 2.0, 540);
		MRal.move(predkoscDobregoMRala, RuchPionowy);
		for (int i = 0; i < przeszkody.size(); i++)
		{
			if (przeszkody[i].getGlobalBounds().intersects(MRal.getGlobalBounds()) && !bCrashed)
			{
				bCrashed = 1;
				predkoscDobregoMRala = 0;
			}
		}
		if (MRal.getPosition().y >= MRalMaxDown)
		{
			MRal.setPosition(Vector2f(MRal.getPosition().x, MRalMaxDown));
		}
		if (MRal.getPosition().y <= MRalMaxUp)
		{
			MRal.setPosition(Vector2f(MRal.getPosition().x, MRalMaxUp));
		}
		ZlyMRal.move(predkoscRuchu, 0);

		if (MRal.getPosition().x >= koniecPoziomu)
		{
			if (!bLost)
			{
				
				if(!bWin) EndGameClock.restart();
				bWin = 1;
			}
		}
		if (ZlyMRal.getPosition().x >= koniecPoziomu)
		{
			if (!bWin)
			{
				if(!bLost) EndGameClock.restart();
				bLost = 1;
			}
		}

		if (bWin || bLost)
		{
			EndGameTimer = EndGameClock.getElapsedTime();
			if (EndGameTimer.asSeconds() > 2)
			{
				return bLost;
			}
		}

		if(!bWin && !bLost) okno.setView(view);
		okno.clear();
		for (int i = 0; i < ilosctel; i++)
		{
			okno.draw(tlo[i]);
		}
		for (int i = 0; i < przeszkody.size(); i++)
		{
			okno.draw(przeszkody[i]);
		}
		okno.draw(liniadolna);
		okno.draw(liniagorna);
		okno.draw(MRal);
		okno.draw(ZlyMRal);
		okno.draw(Meta[1]);
		okno.draw(Meta[0]);
		okno.display();
	}
}
