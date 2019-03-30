#include "ZlowieszczeGabki.hpp"

Textury_ZlowieszczeGabki textury;

void DodajGabke(vector<Sprite> & gabki)
{
	gabki.push_back(Sprite());
	int liczba = gabki.size() - 1;
	gabki[liczba].setTexture(textury.Gabka);
}

bool Minigra_ZlowieszczeGabki(float czasnawykonanie)
{
	const float predkoscGabek = 5;

	const int iloscGabek = 10;
	float predkoscRuchu = 0;

	Clock SpawnGabekClock;
	Time SpawnGabekTimer;

	Clock DeltaTimeClock;
	Time DeltaTimeTimer;
	float DeltaTime;

	Sprite MRal;
	Sprite tlo;
	vector<Sprite> gabki;


	MRal.setTexture(textury.MRal);
	tlo.setTexture(textury.tlo);

	MRal.setOrigin(Vector2f(MRal.getGlobalBounds().width / 2.0,0));

	MRal.setPosition(960, 900 - MRal.getGlobalBounds().height);
	DeltaTimeClock.restart();
	while (true)
	{
		DeltaTimeTimer = DeltaTimeClock.restart();
		DeltaTime = DeltaTimeTimer.asMicroseconds() / 1000.0;

		predkoscRuchu = (startowailoscczasu / czasnawykonanie) * predkoscGabek * DeltaTime;

		SpawnGabekTimer = SpawnGabekClock.getElapsedTime();

		if (SpawnGabekTimer.asMicroseconds() >= czasnawykonanie * 1000.0 / (float)iloscGabek)
		{
			gabki.push_back(Sprite());
			int kat = rand() % 180 - 90;
			gabki[gabki.size() - 1].setPosition(Vector2f(MRal.getGlobalBounds().top + MRal.getGlobalBounds().height / 2.0 , -100))
		}

		Event zdarzenie;
		if (okno.pollEvent(zdarzenie))
		{
			obslugazdarzen(zdarzenie);
		}
		okno.clear();
		okno.draw(tlo);
		okno.draw(MRal);
		for (int i = 0; i < gabki.size(); i++)
		{
			okno.draw(gabki[i]);
		}
		okno.display();
	}
}
