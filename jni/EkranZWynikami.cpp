#include "EkranZWynikami.hpp"
#include "Funkcje.hpp"

Textury_EkranZWynikami textury_EkranZWynikami;

int EkranZWynikami(/*RenderWindow * okno,*/ bool bWon)
{
	int starepunkty = nowepunkty;
	float ptscache = 0;
	//View view;
	//view.setSize(Vector2f(1920, 1080));
	view.setCenter(Vector2f(960, 540));

	Clock DeltaTimeClock2;
	Time DeltaTimeTimer2;

	Clock EndScreenClock;
	Time EndScreenTimer;

	Sprite MRal;
	RectangleShape tlo;
	Sprite sprite_zycia[3];
	Text txt_punkty;


	

	const int MRalMaxPositionX = 800;

	float DeltaTime = 0;

	const int EkranZWynikami_CzasTrwania = 3;

	if (!bStraconeZycie)
	{
		short randnmbr = rand() % (EkranZWynikami_ilosctexturMRalow);
		MRal.setTexture(textury_EkranZWynikami.MRal[randnmbr]);
	}
	else
	{
		MRal.setTexture(textury_EkranZWynikami.SmutnyMRal);
		bStraconeZycie = 0;
	}
	MRal.setOrigin(MRal.getGlobalBounds().width / 2.0, MRal.getGlobalBounds().height / 2.0);
	MRal.setPosition(MRalMaxPositionX / 2.0, 1080 / 2.0);

	tlo.setSize(Vector2f(5000, 5000));
	tlo.setPosition(-1500, 0);
	tlo.setFillColor(Color(31, 64, 53));

	if (bWon)
	{
		punkty += ptsperwin;
	}

	for (int i = 0; i < 3; i++)
	{
		sprite_zycia[i].setTexture(textury_EkranZWynikami.zycie);
		sprite_zycia[i].setPosition(MRalMaxPositionX + 500 + (i * ( 50 + sprite_zycia[i].getGlobalBounds().width )), 600);
	}

	txt_punkty.setFont(font_arial);
	txt_punkty.setCharacterSize(200);
	
	

	DeltaTimeClock2.restart();

	while (true)
	{
		txt_punkty.setString(to_string(nowepunkty));
		txt_punkty.setOrigin(txt_punkty.getGlobalBounds().width / 2.0, txt_punkty.getGlobalBounds().height / 2.0);
		txt_punkty.setPosition(sprite_zycia[1].getGlobalBounds().width / 2.0 + sprite_zycia[1].getPosition().x, 400);
		EndScreenTimer = EndScreenClock.getElapsedTime();

		DeltaTimeTimer2 = DeltaTimeClock2.restart();
		DeltaTime = DeltaTimeTimer2.asMicroseconds() / 1000.0;

		Event zdarzenie;
		if (okno.pollEvent(zdarzenie))
		{
			obslugazdarzen(zdarzenie);
		}

		if (EndScreenTimer.asSeconds() >= EkranZWynikami_CzasTrwania)
		{
			return 0;
		}
		if (EndScreenTimer.asMilliseconds() >= 500)
		{
			if (nowepunkty < punkty)
			{
				ptscache += DeltaTime / 5;
				nowepunkty = ptscache + starepunkty;
				if (nowepunkty > punkty) nowepunkty = punkty;
			}
		}

		okno.setView(view);
		okno.clear();
		okno.draw(tlo);
		okno.draw(MRal);
		okno.draw(txt_punkty);
		for (int i = 0; i < zycia; i++)
		{
			okno.draw(sprite_zycia[i]);
		}
		okno.display();
		//cout << "EKRANZWYNIKAMI" << endl;
	}
}
