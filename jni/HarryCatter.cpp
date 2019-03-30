#include "HarryCatter.hpp"

Textury_HarryCatter textury_HarryCatter;

void cleanup(vector<Sprite> & pociskiMRala, vector<Sprite> & pociskiZlegoMRala)
{
	pociskiMRala.clear();
	pociskiZlegoMRala.clear();
}

void SprawdzKolizje(vector<Sprite> & pociskiMRala, vector<Sprite> & pociskiZlegoMRala, Sprite MRal, Sprite ZlyMRal, bool & bWon, bool & bLost)
{
	if (pociskiMRala.size() > 0 && pociskiZlegoMRala.size() > 0)
	{
		if (pociskiMRala[0].getGlobalBounds().intersects(pociskiZlegoMRala[0].getGlobalBounds()))
		{
			pociskiMRala.erase(pociskiMRala.begin());
			pociskiZlegoMRala.erase(pociskiZlegoMRala.begin());
			cout << "Kontakt" << endl;
		}
	}
	if (pociskiMRala.size() > 0)
	{
		if (pociskiMRala[0].getGlobalBounds().intersects(ZlyMRal.getGlobalBounds()))
		{
			bWon = 1;
			cout << "Won" << endl;
			pociskiMRala.erase(pociskiMRala.begin());
		}
	}
	if (pociskiZlegoMRala.size() > 0)
	{
		if (pociskiZlegoMRala[0].getGlobalBounds().intersects(MRal.getGlobalBounds()))
		{
			bLost = 1;
			cout << "lost" << endl;
			pociskiZlegoMRala.erase(pociskiZlegoMRala.begin());
		}
	}
}

void RuszPociski(vector<Sprite> & pociskiMRala, vector<Sprite> & pociskiZlegoMRala, const float PredkoscRuchuPociskow, float DeltaTime)
{
	for (int i = 0; i < pociskiMRala.size(); i++)
	{
		pociskiMRala[i].move(Vector2f(PredkoscRuchuPociskow * DeltaTime, 0));
	}
	for (int i = 0; i < pociskiZlegoMRala.size(); i++)
	{
		pociskiZlegoMRala[i].move(Vector2f(-PredkoscRuchuPociskow * DeltaTime, 0));
	}
}

void AnimacjaMRala(Sprite & MRal, bool klatkaAnimacji, float skalaMRala, Texture txt_MRal)
{
	//MRal.setTextureRect(IntRect((MRal.getGlobalBounds().width / 2.0 / skalaMRala) * ( klatkaAnimacji ), 0, (MRal.getGlobalBounds().width / 2.0 / skalaMRala ) * ( klatkaAnimacji + 1 ), MRal.getGlobalBounds().height / skalaMRala));
	MRal.setTextureRect(IntRect(txt_MRal.getSize().x / 2.0 * (klatkaAnimacji), 0, (txt_MRal.getSize().x / 2.0) * (klatkaAnimacji + 1), txt_MRal.getSize().y));
	cout << "Size1: " << txt_MRal.getSize().x / 2.0 / skalaMRala * (klatkaAnimacji) << " Size2: " << (txt_MRal.getSize().x / 2.0 / skalaMRala) * (klatkaAnimacji + 1) << endl;
}

int Minigra_HarryCatter(float czasnawykonanie)
{
	bool TimeUp = 0;
	bool bWon = 0;
	bool bLost = 0;
	bool EndGameTimerRestarted = 0;

	//W milisekundach
	float OdstepWWystrzeliwywaniuPociskow = 400;

	float skalaPocisku = 0.25;
	float skalaMRala = 0.25;
	float wysokoscMRala = 300;

	Clock ZegarZlegoMRala;
	Time TimerZlegoMrala;

	Clock DeltaTimeClock;
	Time DeltaTimeTimer;

	Clock ZegarPozostalegoCzasu;
	Time TimerPozostalegoCzasu;

	Clock EndGameClock;
	Time EndGameTimer;

	float DeltaTime;

	const float PredoscRuchuPociskow = 1.5;

	float aktualnapredkoscpociskow = PredoscRuchuPociskow;

	bool klatkaanimacji = 0;
	

	Sprite MRal;
	Sprite ZlyMRal;
	Sprite tlo;

	vector<Sprite> pociskiMRala;
	vector<Sprite> pociskiZlegoMRala;

	tlo.setTexture(textury_HarryCatter.tlo);

	MRal.setTexture(textury_HarryCatter.MRal);
	MRal.setScale(Vector2f(skalaMRala, skalaMRala));
	MRal.setPosition(Vector2f(100, wysokoscMRala));

	MRal.setTextureRect(IntRect(0, 0,MRal.getGlobalBounds().width / 2.0 / skalaMRala,MRal.getGlobalBounds().height / skalaMRala));


	ZlyMRal.setTexture(textury_HarryCatter.MRal);
	ZlyMRal.setScale(Vector2f(-skalaMRala, skalaMRala));
	ZlyMRal.setTextureRect(IntRect(0, 0, ZlyMRal.getGlobalBounds().width / 2.0 / skalaMRala, ZlyMRal.getGlobalBounds().height / skalaMRala));
	ZlyMRal.setPosition(Vector2f(1920 - ZlyMRal.getGlobalBounds().width / 2.0, wysokoscMRala));
	ZlyMRal.setColor(Color(255, 0, 0));

	DeltaTimeClock.restart();
	ZegarZlegoMRala.restart();
	ZegarPozostalegoCzasu.restart();
	while (true)
	{
		TimerZlegoMrala = ZegarZlegoMRala.getElapsedTime();
		DeltaTimeTimer = DeltaTimeClock.restart();
		DeltaTime = DeltaTimeTimer.asMicroseconds() / 1000.0;

		if (TimerZlegoMrala.asMilliseconds() >= OdstepWWystrzeliwywaniuPociskow)
		{
			ZegarZlegoMRala.restart();
			pociskiZlegoMRala.push_back(Sprite());
			pociskiZlegoMRala[pociskiZlegoMRala.size() - 1].setTexture(textury_HarryCatter.pocisk);
			pociskiZlegoMRala[pociskiZlegoMRala.size() - 1].setOrigin(Vector2f(0, pociskiZlegoMRala[pociskiZlegoMRala.size() - 1].getGlobalBounds().height / 2.0));
			pociskiZlegoMRala[pociskiZlegoMRala.size() - 1].setScale(-skalaPocisku, skalaPocisku);
			pociskiZlegoMRala[pociskiZlegoMRala.size() - 1].setPosition(Vector2f(ZlyMRal.getGlobalBounds().left - pociskiZlegoMRala[pociskiZlegoMRala.size() - 1].getGlobalBounds().width / 2.0 , ZlyMRal.getGlobalBounds().top + textury_HarryCatter.MRal.getSize().y / 2.0 * skalaMRala));
			
		}
		Event zdarzenie;
		if (okno.pollEvent(zdarzenie))
		{
			obslugazdarzen(zdarzenie);
			if (!TimeUp)
			{
				if (zdarzenie.type == zdarzenie.KeyPressed && zdarzenie.key.code == Keyboard::Space)
				{
					pociskiMRala.push_back(Sprite());
					pociskiMRala[pociskiMRala.size() - 1].setTexture(textury_HarryCatter.pocisk);
					pociskiMRala[pociskiMRala.size() - 1].setOrigin(Vector2f(0, pociskiMRala[pociskiMRala.size() - 1].getGlobalBounds().height / 2.0));
					pociskiMRala[pociskiMRala.size() - 1].setPosition(Vector2f(MRal.getGlobalBounds().left + textury_HarryCatter.MRal.getSize().x / (2.0 / skalaMRala), MRal.getGlobalBounds().top + textury_HarryCatter.MRal.getSize().y / 2.0 * skalaMRala));
					pociskiMRala[pociskiMRala.size() - 1].setScale(skalaPocisku, skalaPocisku);
					if (klatkaanimacji == 0)
					{
						klatkaanimacji = 1;
					}
					else
						klatkaanimacji = 0;
					cout << "klatka: " << klatkaanimacji << endl;
					AnimacjaMRala(MRal, klatkaanimacji, skalaMRala, textury_HarryCatter.MRal);
				}
			}
		}
		if (TimeUp)
		{
			OdstepWWystrzeliwywaniuPociskow = 250;
		}
		if (!bLost && !bWon)
		{
			RuszPociski(pociskiMRala, pociskiZlegoMRala, aktualnapredkoscpociskow, DeltaTime);
			SprawdzKolizje(pociskiMRala, pociskiZlegoMRala, MRal, ZlyMRal, bWon, bLost);
			TimerPozostalegoCzasu = ZegarPozostalegoCzasu.getElapsedTime();
			
		}
		else
		{
			aktualnapredkoscpociskow = PredoscRuchuPociskow / 4.0;
			RuszPociski(pociskiMRala, pociskiZlegoMRala, aktualnapredkoscpociskow, DeltaTime);
			OdstepWWystrzeliwywaniuPociskow = 100000000;
			EndGameTimer = EndGameClock.getElapsedTime();
			if (EndGameTimer.asSeconds() >= 2)
			{
				cleanup(pociskiMRala, pociskiZlegoMRala);
				return bLost;
			}
		}
		if (bLost)
		{
			MRal.move(-DeltaTime / 10, DeltaTime / 10);
			MRal.setRotation(MRal.getRotation() - ( DeltaTime / 20.0 ));
			if (!EndGameTimerRestarted)
				EndGameClock.restart();
			EndGameTimerRestarted = 1;
		}
		if (bWon)
		{
			ZlyMRal.move(DeltaTime / 10, DeltaTime / 10);
			ZlyMRal.setRotation(ZlyMRal.getRotation() + (DeltaTime / 20.0));
			if (!EndGameTimerRestarted)
				EndGameClock.restart();
			EndGameTimerRestarted = 1;
		}
		
		okno.clear();
		okno.draw(tlo);
		okno.draw(MRal);
		okno.draw(ZlyMRal);
		for (int i = 0; i < pociskiMRala.size(); i++)
		{
			okno.draw(pociskiMRala[i]);
		}
		for (int i = 0; i < pociskiZlegoMRala.size(); i++)
		{
			okno.draw(pociskiZlegoMRala[i]);
		}
		TimeUp = RysujHUD(czasnawykonanie - TimerPozostalegoCzasu.asMilliseconds() / 1000.0, czasnawykonanie);
		okno.display();
	}
}
