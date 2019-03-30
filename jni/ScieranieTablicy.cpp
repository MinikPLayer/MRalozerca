#include "ScieranieTablicy.hpp"
#include "LineShape.cpp"
#include <bitset>

float ScieranieTablicy_promienrysowania = 200;

Textury_ScieranieTablicy textury_ScieranieTablicy;

enum ScieranieTablicy_PozycjaMRala
{
	LewyGorny,
	PrawyGorny,
	PrawyDolny,
	LewyDolny
};

class PunktSprawdzajcy
{
public:
	PunktSprawdzajcy();
	Sprite Pkt;
	Texture txt;
	bool Aktywowany = 0;
};

PunktSprawdzajcy::PunktSprawdzajcy()
{
	Image img;
	img.create(ScieranieTablicy_promienrysowania, ScieranieTablicy_promienrysowania);
	for (int i = 0; i < ScieranieTablicy_promienrysowania; i++)
	{
		for (int j = 0; j < ScieranieTablicy_promienrysowania; j++)
		{
			//img.setPixel(i, j, KolorTablicy);
			img.setPixel(i, j, Color(255, 255, 255));
		}
	}
	txt.create(ScieranieTablicy_promienrysowania, ScieranieTablicy_promienrysowania);
	txt.loadFromImage(img);
	Pkt.setTexture(txt);
}

void cleanup(vector<sfLine> & zamalowanelinie, vector<Sprite> & ozdoby, vector<PunktSprawdzajcy> & pktsprawdzajace)
{
	zamalowanelinie.clear();
	ozdoby.clear();
	pktsprawdzajace.clear();
}

bool Completed(int CompletedPts, int punktysprawdzajaceSize)
{
	if (CompletedPts == punktysprawdzajaceSize)
	{
		return 1;
	}
	else
		return 0;
}

int Minigra_ScieranieTablicy(/*RenderWindow * okno,*/ float czasnawykonanie)
{
	int PozycjaMRala = rand() % 4;

	Clock EndGameClock;
	Time EndGameTimer;

	Clock TimeElapsedClock;
	Time TimeElapsedTimer;


	bool bNoTimeLeft = 0;
	bool bLost = 0;
	bool bWon = 0;

	float promienrysowania = ScieranieTablicy_promienrysowania;
	int iloscozdob = 0;

	int CompletedPts = 0;


	//UWAGAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA VECTOR
	vector<sfLine> zamalowanelinie;
	vector<Sprite> ozdoby;

	

	Vector2i PoprzedniaPozycjaMyszy;
	Vector2i poz_myszy;

	bool LPMTrzymany = 0;

	Clock DeltaTimeClock;
	Time DeltaTimeTimer;

	Texture txt_wyczyszczonaCzesc;
	Sprite WyczyszczonaCzesc;

	//UWAGAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA VECTOR
	vector<PunktSprawdzajcy> punktysprawdzajace;

	float DeltaTime;
	

	Sprite MRal;
	Sprite Gabka;
	RectangleShape tlo;
	tlo.setFillColor(Color(31, 64, 53));
	tlo.setSize(Vector2f(5000, 5000));

	Image image;
	image.create(1920, 1080, Color(0, 0, 0));

	MRal.setTexture(textury_ScieranieTablicy.MRal[rand() % ScieranieTablicy_ilosctxtMRalow]);
	//MRal.setPosition(0, WysokoscPaskowHUD);
	switch (PozycjaMRala)
	{
	case ScieranieTablicy_PozycjaMRala::LewyGorny:
		MRal.setPosition(0, WysokoscPaskowHUD);
		break;
	case ScieranieTablicy_PozycjaMRala::PrawyGorny:
		MRal.setPosition(1920 - MRal.getGlobalBounds().width, WysokoscPaskowHUD);
		break;
	case ScieranieTablicy_PozycjaMRala::PrawyDolny:
		MRal.setPosition(1920 - MRal.getGlobalBounds().width, 1080 - MRal.getGlobalBounds().height);
		break;
	case ScieranieTablicy_PozycjaMRala::LewyDolny:
		MRal.setPosition(0, 1080 - MRal.getGlobalBounds().height);
	}
	tlo.setOrigin(982 + 960, 0);
	//tlo.setTexture(textury.tlo);
	tlo.setPosition(960, 0);
	Gabka.setTexture(textury_ScieranieTablicy.Gabka);
	Gabka.setOrigin(Vector2f(Gabka.getGlobalBounds().width / 2.0, Gabka.getGlobalBounds().height / 2.0));


	const float skalaozdob = 2.0;

	for (int i = 0; i < 1920 - ( 400 * skalaozdob ); i += 400 * skalaozdob)
	{
		for (int j = WysokoscPaskowHUD; j < 1080 - ( 110 * skalaozdob ); j += 150 * skalaozdob)
		{
			if (!MRal.getGlobalBounds().contains(Vector2f(i,j)) && !MRal.getGlobalBounds().contains(i + 400 * skalaozdob, j + 110 * skalaozdob && !MRal.getGlobalBounds().contains(i + 400 * skalaozdob, j) && !MRal.getGlobalBounds().contains(i, j + 110 * skalaozdob)))
			{
				if (PozycjaMRala == PrawyDolny || PozycjaMRala == PrawyGorny)
				{
					if (PozycjaMRala == PrawyDolny)
					{
						if (i < 1920 - (400 * skalaozdob) - MRal.getGlobalBounds().width && j < 1080 - (120 * skalaozdob) - MRal.getGlobalBounds().height)
						{
							Sprite tsprt;
							ozdoby.push_back(tsprt);
							ozdoby[ozdoby.size() - 1].setPosition(Vector2f(i, j));
							ozdoby[ozdoby.size() - 1].setTexture(textury_ScieranieTablicy.ozdoba[rand() % ScieranieTablicy_ilosctxtOzdob]);
							ozdoby[ozdoby.size() - 1].setScale(skalaozdob, skalaozdob);

							i += rand() % 100 * skalaozdob;
						}
					}
					if (PozycjaMRala == PrawyGorny)
					{
						if (i<1920 - (400 * skalaozdob) - MRal.getGlobalBounds().width && j > MRal.getGlobalBounds().height)
						{
							Sprite tsprt;
							ozdoby.push_back(tsprt);
							ozdoby[ozdoby.size() - 1].setPosition(Vector2f(i, j));
							ozdoby[ozdoby.size() - 1].setTexture(textury_ScieranieTablicy.ozdoba[rand() % ScieranieTablicy_ilosctxtOzdob]);
							ozdoby[ozdoby.size() - 1].setScale(skalaozdob, skalaozdob);

							i += rand() % 100 * skalaozdob;
						}
					}
				}
				else
				{
					if (i < 1920 - (400 * skalaozdob))
					{
						Sprite tsprt;
						ozdoby.push_back(tsprt);
						ozdoby[ozdoby.size() - 1].setPosition(Vector2f(i, j));
						ozdoby[ozdoby.size() - 1].setTexture(textury_ScieranieTablicy.ozdoba[rand() % ScieranieTablicy_ilosctxtOzdob]);
						ozdoby[ozdoby.size() - 1].setScale(skalaozdob, skalaozdob);

						i += rand() % 100 * skalaozdob;
					}
				}
			}
		}
	}
	

	/*const int OdstepOdMrala = 200;

	for (int i = 0; i < 1920; i += ScieranieTablicy_promienrysowania + 10)
	{
		for (int j = 200; j < 1080; j += ScieranieTablicy_promienrysowania + 10)
		{

			//if (!((MRal.getGlobalBounds().left + MRal.getGlobalBounds().width + OdstepOdMrala > i && MRal.getGlobalBounds().left - OdstepOdMrala < i) && (MRal.getGlobalBounds().top - OdstepOdMrala < i && MRal.getGlobalBounds().top + MRal.getGlobalBounds().height + OdstepOdMrala > i)))
			//{
				bool KontaktZOzdobami = 0;
				for (int k = 0; k < ozdoby.size(); k++)
				{
					//cout << "I: " << i << " j: " << j << " GGB Left: " << ozdoby[i].getGlobalBounds().left << " GGB Width: " << ozdoby[i].getGlobalBounds().width << " GGB Top: " << ozdoby[i].getGlobalBounds().top << " GGB Height: " << ozdoby[i].getGlobalBounds().height << endl;
					if (ozdoby[k].getGlobalBounds().contains(Vector2f(i, j)))
					{
						KontaktZOzdobami = 1;
					}
				}
				if (KontaktZOzdobami == 1)
				{
					punktysprawdzajace.push_back(PunktSprawdzajcy());
					punktysprawdzajace[punktysprawdzajace.size() - 1].Pkt.setPosition(i, j);
					cout << "X: " << punktysprawdzajace[punktysprawdzajace.size() - 1].Pkt.getPosition().x << " Y: " << punktysprawdzajace[punktysprawdzajace.size() - 1].Pkt.getPosition().y << " I: " << i << " j: " << j << endl;

				}
			//}
		}
	}*/

	for (int i = 0; i < ozdoby.size(); i++)
	{
		for (int j = 0; j < 400.0 * skalaozdob / ScieranieTablicy_promienrysowania; j++)
		{
			punktysprawdzajace.push_back(PunktSprawdzajcy());
			punktysprawdzajace[punktysprawdzajace.size() - 1].Pkt.setPosition(ozdoby[i].getPosition().x + j * ScieranieTablicy_promienrysowania, ozdoby[i].getPosition().y);
			cout << punktysprawdzajace[punktysprawdzajace.size() - 1].Pkt.getPosition().x << ", " << punktysprawdzajace[punktysprawdzajace.size() - 1].Pkt.getPosition().y << endl;
		}
	}

	sfLine LinieDookolaMRala[4]{ { Vector2f(MRal.getGlobalBounds().left,MRal.getGlobalBounds().top),Vector2f(MRal.getGlobalBounds().left + MRal.getGlobalBounds().width,MRal.getGlobalBounds().top) },{ Vector2f(MRal.getGlobalBounds().left,MRal.getGlobalBounds().top),Vector2f(MRal.getGlobalBounds().left,MRal.getGlobalBounds().top + MRal.getGlobalBounds().height) },{ Vector2f(MRal.getGlobalBounds().left + MRal.getGlobalBounds().width,MRal.getGlobalBounds().top),Vector2f(MRal.getGlobalBounds().left + MRal.getGlobalBounds().width,MRal.getGlobalBounds().top + MRal.getGlobalBounds().height) },{ Vector2f(MRal.getGlobalBounds().left,MRal.getGlobalBounds().top + MRal.getGlobalBounds().height),Vector2f(MRal.getGlobalBounds().left + MRal.getGlobalBounds().width,MRal.getGlobalBounds().top + MRal.getGlobalBounds().height) } };
	for (int i = 0; i < 4; i++)
	{
		LinieDookolaMRala[i].setColor(Color::White);
	}


	DeltaTimeClock.restart();
	TimeElapsedClock.restart();
	while (true)
	{
		if (bLost)
		{
			EndGameTimer = EndGameClock.getElapsedTime();
			if (EndGameTimer.asSeconds() >= 1)
			{
				cleanup(zamalowanelinie, ozdoby, punktysprawdzajace);
				return 1;
			}
		}
		if (bWon)
		{
			EndGameTimer = EndGameClock.getElapsedTime();
			if (EndGameTimer.asSeconds() >= 1)
			{
				cleanup(zamalowanelinie, ozdoby, punktysprawdzajace);
				return 0;
			}
		}
		txt_wyczyszczonaCzesc.loadFromImage(image);
		WyczyszczonaCzesc.setTexture(txt_wyczyszczonaCzesc);
		DeltaTimeTimer = DeltaTimeClock.restart();
		DeltaTime = DeltaTimeTimer.asMicroseconds() / 1000.0;
		Event zdarzenie;
		if (okno.pollEvent(zdarzenie))
		{
			obslugazdarzen(zdarzenie);
			if (!bLost)
			{
				Mouse mysz;
				if (mysz.isButtonPressed(Mouse::Button::Left))
				{
					PoprzedniaPozycjaMyszy = poz_myszy;
					poz_myszy = Mouse::getPosition();


					double SkalaX = view.getSize().x / (float)okno.getSize().x;
					double SkalaY = view.getSize().y / (float)okno.getSize().y;
					poz_myszy.x *= SkalaX;
					poz_myszy.y *= SkalaY;
					//poz_myszy.x += przesuniecieviewportu;
					if (!LPMTrzymany) PoprzedniaPozycjaMyszy = poz_myszy;
					/*if ((poz_myszy.x * SkalaX - promienrysowania) - (view.getSize().x / 2.0 - 960) > 0 && poz_myszy.x * SkalaX + promienrysowania / 2.0 < 1920 && poz_myszy.y * SkalaY - promienrysowania / 2.0 > 0 && poz_myszy.y * SkalaY + promienrysowania / 2.0 < 1080)
					{
						for (int i = 0; i < promienrysowania; i++)
						{
							for (int j = 0; j < promienrysowania; j++)
							{

								//image.setPixel(poz_myszy.x + (( i - promienrysowania / 2.0 * promienrysowania) + j ), poz_myszy.y + ( (i - promienrysowania / 2.0 * promienrysowania) + j) , Color(255, 255, 255));
								image.setPixel(/*view.getCenter().x - view.getSize().x / 2.0 + (poz_myszy.x * SkalaX + j - promienrysowania / 2.0) - ( view.getSize().x / 2.0 - 960), poz_myszy.y * SkalaY + i - promienrysowania / 2.0, Color(255, 255, 255));
								//cout << "X: " << poz_myszy.x << " Y: " << poz_myszy.y << " View LEFT: " << (poz_myszy.x * SkalaX + j - promienrysowania / 2.0) - (view.getSize().x / 2.0 - 960) << endl;
								//cout << (poz_myszy.x * SkalaX + j - promienrysowania / 2.0) - (view.getSize().x / 2.0 - 960) << endl;
							}
						}
					}*/
					sfLine linia(PoprzedniaPozycjaMyszy, poz_myszy);
					zamalowanelinie.push_back(sfLine(PoprzedniaPozycjaMyszy, poz_myszy));
					zamalowanelinie[zamalowanelinie.size() - 1].setThickness(promienrysowania);
					zamalowanelinie[zamalowanelinie.size() - 1].setColor(KolorTablicy);
					Vector2f poz_myszyf;
					poz_myszyf.x = poz_myszy.x;
					poz_myszyf.y = poz_myszy.y;
					for (int i = 0; i < punktysprawdzajace.size(); i++)
					{
						if (punktysprawdzajace[i].Pkt.getGlobalBounds().contains(poz_myszyf))
						{
							if (!punktysprawdzajace[i].Aktywowany)
							{
								punktysprawdzajace[i].Aktywowany = 1;
								CompletedPts++;
								punktysprawdzajace[i].Pkt.setColor(Color::White);
								cout << "Activated " << endl;
							}
						}
					}
					if (MRal.getGlobalBounds().contains(poz_myszyf) && !bWon)
					{
						bLost = 1;
						MRal.setTexture(textury_ScieranieTablicy.SmutnyMRal);
						cout << "Czuje sie dotkniety" << endl;
						EndGameTimer = EndGameClock.restart();
					}
					LPMTrzymany = 1;
					float kat = 0;
					float prevkat = atan((PoprzedniaPozycjaMyszy.y - poz_myszy.y) / (float)(PoprzedniaPozycjaMyszy.x - poz_myszy.x)) * 180 / M_PIl;
					if (prevkat != NULL)
						kat = prevkat;
					Gabka.setPosition(poz_myszyf);
					Gabka.setRotation(kat);
				}
				else
					LPMTrzymany = 0;
			}
		}
		okno.setView(view);
		if (!bWon)
		{
			if (Completed(CompletedPts, punktysprawdzajace.size()))
			{
				cout << "Completed" << endl;
				bWon = 1;
				EndGameTimer = EndGameClock.restart();
			}
		}
		if(!bWon && !bLost)
			TimeElapsedTimer = TimeElapsedClock.getElapsedTime();
		okno.clear();
		okno.draw(tlo);
		//okno.draw(WyczyszczonaCzesc);
		/*for (int i = 0; i < punktysprawdzajace.size(); i++)
		{
			okno.draw(punktysprawdzajace[i].Pkt);
		}*/
		for (int i = 0; i < ozdoby.size(); i++)
		{
			okno.draw(ozdoby[i]);
		}
		okno.draw(MRal);
		for (int i = 0; i < 4; i++)
		{
			okno.draw(LinieDookolaMRala[i]);
		}
		//okno.draw(linia);
		for (int i = 0; i < zamalowanelinie.size(); i++)
		{
			okno.draw(zamalowanelinie[i]);
		}
		okno.draw(Gabka);
		if (RysujHUD(czasnawykonanie - TimeElapsedTimer.asMilliseconds() / 1000.0, czasnawykonanie))
		{
			
			MRal.setTexture(textury_ScieranieTablicy.SmutnyMRal);
			if (!bLost)
			{
				EndGameClock.restart();
			}
			bLost = 1;
		}
		
		okno.display();
	}
	return 0;
}
