#include "BiegZPrzeszkodami.hpp"

//Textury
Textury_BiegZPrzeszkodami textury_BiegZPrzeszkodami;

//Wartoœci
const int minOdstepPlotkow = 850;
const int maxRozstrzalPlotkow = 200;
const float predkoscMRala = 5;
const int odstepanimacjiMRala = 50; //Odstêp miêdzy zmian¹ klatki animacji w ms
const int iloscKlatekAnimacjiMRala = 2;
const int szerokoscSpriteMRala = 169; //Szerokosc sprite MRala w px
const int wysokoscSpriteMrala = 111;
const int startowailoscplotkow = 10;
const int skalatla = 2;
const int startowailosctel = 2;
const int PozycjaMRala_Wysokosc = 500;
const short ilosctexturprzeszkod = BiegZPrzeszkodami_ilosctexturprzeszkod;
const short ilosctexturozdobtla = BiegZPrzeszkodami_ilosctexturozdobtla;
const float MRalDefaultJumpHeight = 4;
const short MRalMaxJumpForce = 3;
const float JumpingForceIncreaseTimeAmount = 50; //Im wiêkszy, tym d³u¿ej trwa ,,nape³nianie paska skoku"
const float GravityForce = 0.04;
const int DomyslnyKoniecPoziomu = 6000;
const float SpeedFactorDecrease = 1.5;

enum EndAnimStage
{
	Oczekiwanie,
	PierwszaZmianaObrotu,
	ZjazdPoWykresie,
	PowrotDoNormalnegoObrotu
};

//Publiczne zmienne
float iloscplotkow_float = startowailoscplotkow;
int iloscplotkow = iloscplotkow_float;
float ilosctel_float = 10;
int ilosctel = ilosctel_float;
int iloscozdobtla = ilosctel_float;
float predkoscruchu = 0;
float MRalActualJumpingForce = 0;
bool MRalBlockJumping = 0;
float MRalActualRisingForce = 0;
int AktualnyKoniecPoziomu = 0;
bool CameraCenterOnMRal = 1;
EndAnimStage StanKoncowejAnimacji = Oczekiwanie;
float SpeedFactor = 0;
bool dead = 0;
bool won = 0;





//View
//View view;

//Zegar
Clock zegar;
Time czas;

Clock zegar2;
Time czas2;

Clock ZegarAnimacji;
Time CzasAnimacji;

Clock JumpClock;
Time JumpTime;

Clock JumpHeightClock;
Time JumpHeightTime;

Clock EndLevelAnimationClock;
Time EndLevelAnimationTime;

Clock DeltaClock;
Time DeltaTimeTimer;

Clock DeathClock;
Time DeathTimer;

Text przeszkody;

void cleanup(std::vector<GotowySprite> & Przeszkoda, std::vector<GotowySprite> & tlo, std::vector<GotowySprite> & odzdobytla)
{
	Przeszkoda.clear();
	tlo.clear();
	odzdobytla.clear();
	iloscplotkow_float = startowailoscplotkow;
	iloscplotkow = iloscplotkow_float;
	ilosctel_float = 10;
	ilosctel = ilosctel_float;
	iloscozdobtla = ilosctel_float;
	predkoscruchu = 0;
	MRalActualJumpingForce = 0;
	MRalBlockJumping = 0;
	MRalActualRisingForce = 0;
	AktualnyKoniecPoziomu = 0;
	CameraCenterOnMRal = 1;
	StanKoncowejAnimacji = Oczekiwanie;
	SpeedFactor = 0;
	dead = 0;
	won = 0;
}

void MRalJumpMove(GotowySprite & MRal)
{
	JumpHeightTime = JumpHeightClock.getElapsedTime();
	MRalActualRisingForce -= (JumpHeightTime.asMicroseconds()) * GravityForce / 1000;
	MRal.move(0, -MRalActualRisingForce / 10000.f * JumpHeightTime.asMicroseconds() );
	JumpHeightClock.restart();
}

void MRalJumpForce(GotowySprite & MRal)
{
	
	if (MRalBlockJumping == 0)
	{
		if (MRalActualJumpingForce < MRalMaxJumpForce)
		{
			if (MRalActualJumpingForce == 0)
			{
				JumpClock.restart();
				MRalActualJumpingForce = 0.000000001;
				if (debug) cout << "Zresetowano" << endl;
			}
			JumpTime = JumpClock.getElapsedTime();
			MRalActualJumpingForce += ( JumpTime.asMicroseconds() / JumpingForceIncreaseTimeAmount ) / 1000;
			JumpClock.restart();
		}
		else
		{
			MRalActualJumpingForce = MRalMaxJumpForce;
			MRalBlockJumping = true;
		}
		MRalActualRisingForce = MRalActualJumpingForce * MRalDefaultJumpHeight;
		MRalJumpMove(MRal);
	}
	
}

void animacjaMRala(GotowySprite & MRal)
{
	if (CzasAnimacji.asMilliseconds() < odstepanimacjiMRala * (iloscKlatekAnimacjiMRala))
	{
		int klatka = CzasAnimacji.asMilliseconds() / odstepanimacjiMRala;
		MRal.sprite.setTextureRect(IntRect(szerokoscSpriteMRala * klatka, 0, szerokoscSpriteMRala, wysokoscSpriteMrala));
	}
	else
	{
		MRal.sprite.setTextureRect(IntRect(0, 0, szerokoscSpriteMRala, wysokoscSpriteMrala));
		ZegarAnimacji.restart();
	}
}

void obliczpredkoscruchu(float czasnawykonanie)
{
	czas = zegar.getElapsedTime();
	predkoscruchu = (startowailoscczasu / czasnawykonanie) * czas.asMicroseconds() * predkoscMRala / 10000.0 ;
	zegar.restart();
	CzasAnimacji = ZegarAnimacji.getElapsedTime();
}

void wstepnakonfiguracja(GotowySprite & MRal, std::vector<GotowySprite> & tlo, GotowySprite & KoncowyZjazd)
{
	//view.setSize(defResX, defResY);
	view.setCenter(defResX / 2, defResY / 2);
	MRal.sprite.setOrigin(MRal.sprite.getGlobalBounds().width / 2, MRal.sprite.getGlobalBounds().height / 2);
	for (int i = 0; i < ilosctel; i++)
	{
		tlo[i].sprite.setOrigin(0, 1080);
		tlo[i].sprite.setPosition(tlo[0].sprite.getGlobalBounds().width * i, defResY);
		if (i % 2 == 0)
		{
			tlo[i].setScale(skalatla);
		}
		else
		{
			tlo[i].setScale(-skalatla, skalatla);
			tlo[i].sprite.move(Vector2f(tlo[i].sprite.getGlobalBounds().width, 0));
		}
		
	}
	
	MRal.setPosition(defResX / 2, PozycjaMRala_Wysokosc);
	KoncowyZjazd.setPosition(AktualnyKoniecPoziomu, PozycjaMRala_Wysokosc - KoncowyZjazd.sprite.getGlobalBounds().width / 2 - 100);
}

int wygenerujpozycjeprzeszkody(short ktory)
{
	int pozycja = 0;
	bool exit = 0;
	while (!exit)
	{
		if (minOdstepPlotkow * ( ktory + 3 ) * SpeedFactor / SpeedFactorDecrease >= AktualnyKoniecPoziomu - 300)
		{
			return -100;
		}
		exit = 1;
		pozycja = (minOdstepPlotkow * (ktory + 3) * SpeedFactor / SpeedFactorDecrease + (rand() % maxRozstrzalPlotkow));
		if (pozycja >= AktualnyKoniecPoziomu - 300)
		{
			exit = 0;
			continue;
		}
	}
	return pozycja;
}



int MiniGra_BiegZPrzeszkodami(/*RenderWindow * okno,*/ float czasnawykonanie)
{
	float DeltaTime = 0;
	zegar.restart();
	//EndLevelAnimationClock.restart();
	if (debug_write(0)) cout << (startowailoscczasu / czasnawykonanie);

	SpeedFactor = (startowailoscczasu / czasnawykonanie);
	iloscplotkow_float = startowailoscplotkow * SpeedFactorDecrease /* (startowailoscczasu / czasnawykonanie)*/;
	iloscplotkow = iloscplotkow_float + 1;
	ilosctel_float = startowailosctel * SpeedFactor / SpeedFactorDecrease;
	ilosctel = ilosctel_float + 1;
	if (debug) cout << "Tla: " << ilosctel << endl;
	iloscozdobtla = ilosctel;
	AktualnyKoniecPoziomu = DomyslnyKoniecPoziomu * SpeedFactor / SpeedFactorDecrease;


	if (debug_write(0)) cout << "Plotki: " << iloscplotkow << " Tla: " << ilosctel << endl;

	//Sprite'y + konfiguracja
	GotowySprite MRal;
	GotowySprite KoncowyZjazd;

	MRal.sprite.setTexture(textury_BiegZPrzeszkodami.MRal);
	KoncowyZjazd.sprite.setTexture(textury_BiegZPrzeszkodami.KoncowyZjazd);

	//Text'y + konfiguracja
	GotowyText FPS(to_string(0), 100, Text::Regular, Color::White, "font/arial.ttf");
	FPS.text.setPosition(view.getViewport().left, view.getViewport().top);
	
	std::vector<GotowySprite> Przeszkoda(iloscplotkow, GotowySprite());
	std::vector<GotowySprite> tlo(ilosctel, GotowySprite());
	std::vector<GotowySprite> Ozdobatla(iloscozdobtla, GotowySprite());
	Texture txt_tlo;
	for (int i = 0; i < ilosctel; i++) 
	{
		tlo[i].sprite.setTexture(textury_BiegZPrzeszkodami.tlo);
	}
	for (int i = 0; i < iloscplotkow; i++) 
	{
		short randnmbr = rand() % BiegZPrzeszkodami_ilosctexturprzeszkod;
		Przeszkoda[i].sprite.setTexture(textury_BiegZPrzeszkodami.Przeszkoda[randnmbr]);
		Przeszkoda[i].sprite.setOrigin(0, Przeszkoda[i].sprite.getGlobalBounds().height / 2);
		Przeszkoda[i].setPosition(wygenerujpozycjeprzeszkody(i), PozycjaMRala_Wysokosc);
	}
	for (int i = 0; i < iloscozdobtla; i++)
	{
		short randnmbr = rand() % ilosctexturozdobtla;
		Ozdobatla[i].sprite.setTexture(textury_BiegZPrzeszkodami.OzdobaTla[randnmbr]);
	}
	wstepnakonfiguracja(MRal, tlo, KoncowyZjazd);

	//G³ówna pêtla programu
	while (true)
	{
		DeltaTimeTimer = DeltaClock.restart();
		DeltaTime = DeltaTimeTimer.asMicroseconds() / 1000.0;
		obliczpredkoscruchu(czasnawykonanie);

		animacjaMRala(MRal);

		Event zdarzenie;
		if (okno.pollEvent(zdarzenie))
		{
			obslugazdarzen(zdarzenie);
		}

		if (Keyboard::isKeyPressed(bind_skok))
		{
			if (MRalActualJumpingForce == 0)
			{
				JumpHeightClock.restart();
			}
			MRalJumpForce(MRal);
			//if (MRalActualJumpingForce == 0) JumpClock.restart();
		}
		else
		{
			if (MRalActualJumpingForce > 0)
			{
				MRalBlockJumping = true;
				//if (debug) cout << "Blocked jumping" << endl;
			}
		}

		if (MRalBlockJumping == true)
		{
			MRalJumpMove(MRal);
			if (MRal.sprite.getPosition().y >= PozycjaMRala_Wysokosc && !dead)
			{
				MRalBlockJumping = false;
				MRalActualJumpingForce = 0;
				MRal.setPosition(MRal.sprite.getPosition().x, PozycjaMRala_Wysokosc);
			}
		}
		if (MRal.sprite.getPosition().y >= PozycjaMRala_Wysokosc && !dead)
		{
			MRal.setPosition(MRal.sprite.getPosition().x, PozycjaMRala_Wysokosc);
		}

		if (debug_write(50))
		{
			//cout << "JumpingForce: " << MRalActualJumpingForce << " RisingRate: " << MRalActualRisingForce << endl;
			cout << "DeltaTime: " << DeltaTime << endl;
			tick = 0;
		}

		if (ShowFPS)
		{
			FPS.text.setPosition(view.getCenter().x - defResX / 2, view.getCenter().y - defResY / 2);
		}

		if (!won)
		{
			if (MRal.sprite.getPosition().x > AktualnyKoniecPoziomu)
			{
				CameraCenterOnMRal = 0;
				won = 1;
				DeathTimer = DeathClock.restart();
			}
		}

		if (CameraCenterOnMRal)
		{
			//Ustalanie pozycji okna
			view.setCenter(MRal.sprite.getPosition().x, PozycjaMRala_Wysokosc);
			okno.setView(view);
		}
		if (!dead)
		{
			for (int i = 0; i < iloscplotkow; i++)
			{
				if (Przeszkoda[i].sprite.getGlobalBounds().intersects(MRal.sprite.getGlobalBounds()))
				{
					dead = 1;
					DeathTimer = DeathClock.restart();
				}
			}
		}
		else
		{
			CameraCenterOnMRal = 0;
			MRal.move(0, predkoscruchu / 2.0);
			MRal.sprite.setRotation(MRal.sprite.getRotation() + predkoscruchu / 5);
			DeathTimer = DeathClock.getElapsedTime();
			if (DeathTimer.asMilliseconds() > 1500)
			{
				cleanup(Przeszkoda, tlo, Ozdobatla);
				return 1;
			}
		}

		if (won)
		{
			DeathTimer = DeathClock.getElapsedTime();
			if (DeathTimer.asMilliseconds() > 1500)
			{
				cleanup(Przeszkoda, tlo, Ozdobatla);
				return 0;
			}
		}

		MRal.move(predkoscruchu, 0);

		//Czyszczenie i rysowanie obrazu
		okno.clear();
		for (int i = 0; i < ilosctel; i++)
		{
			tlo[i].draw(&okno);
		}
		for (int i = 0; i < iloscplotkow; i++)
		{
			Przeszkoda[i].draw(&okno);
		}
		for (int i = 0; i < iloscozdobtla; i++)
		{
			Ozdobatla[i].draw(&okno);
		}
		KoncowyZjazd.draw(&okno);
		MRal.draw(&okno);
		if (ShowFPS)
		{
			ustalfps(FPS);
			FPS.draw(&okno);
		}
		okno.display();

		tickupdate();
	}
	cleanup(Przeszkoda, tlo, Ozdobatla);
	return 0;
}
