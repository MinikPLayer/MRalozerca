#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <fstream>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#define USE_JNI

#if defined(USE_JNI)
// These headers are only needed for direct NDK/JDK interaction
#include <jni.h>
#include <android/native_activity.h>

// Since we want to get the native activity from SFML, we'll have to use an
// extra header here:
#include <SFML/System/NativeActivity.hpp>

// NDK/JNI sub example - call Java code from native code
int vibrate(float duration)
{
	// First we'll need the native activity handle
	ANativeActivity *activity = sf::getNativeActivity();

	// Retrieve the JVM and JNI environment
	JavaVM* vm = activity->vm;
	JNIEnv* env = activity->env;

	// First, attach this thread to the main thread
	JavaVMAttachArgs attachargs;
	attachargs.version = JNI_VERSION_1_6;
	attachargs.name = "NativeThread";
	attachargs.group = NULL;
	jint res = vm->AttachCurrentThread(&env, &attachargs);

	if (res == JNI_ERR)
		return EXIT_FAILURE;

	// Retrieve class information
	jclass natact = env->FindClass("android/app/NativeActivity");
	jclass context = env->FindClass("android/content/Context");

	// Get the value of a constant
	jfieldID fid = env->GetStaticFieldID(context, "VIBRATOR_SERVICE", "Ljava/lang/String;");
	jobject svcstr = env->GetStaticObjectField(context, fid);

	// Get the method 'getSystemService' and call it
	jmethodID getss = env->GetMethodID(natact, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
	jobject vib_obj = env->CallObjectMethod(activity->clazz, getss, svcstr);

	// Get the object's class and retrieve the member name
	jclass vib_cls = env->GetObjectClass(vib_obj);
	jmethodID vibrate = env->GetMethodID(vib_cls, "vibrate", "(J)V");

	// Determine the timeframe
	jlong length = duration;

	// Bzzz!
	env->CallVoidMethod(vib_obj, vibrate, length);

	// Free references
	env->DeleteLocalRef(vib_obj);
	env->DeleteLocalRef(vib_cls);
	env->DeleteLocalRef(svcstr);
	env->DeleteLocalRef(context);
	env->DeleteLocalRef(natact);

	// Detach thread again
	vm->DetachCurrentThread();
}

#endif

using namespace sf;
using namespace std;

const short ilosckolorwgrunia = 3;

class Grunio
{
public:
    Sprite sprite;
    Texture textura[ilosckolorwgrunia];
    float x;
    float y;
    float xt;
    long int punkty = 0;
    long int zycia = 3;
    short kolor = 0;
};

const int powerUpsSize = 4;

class PowerUps
{

public:

    enum powerUpsNames
    {
        slowMo,
        doublePoints,
        turbo,
        colorsOff
    };
    bool powerUp[powerUpsSize];
    float czasTrwaniaPowerUpa[powerUpsSize];
    PowerUps();

    void managerPowerUpsTimers();


private:
    Time timer[powerUpsSize];
    Clock clock;
    const float domyslnyCzasTrwania = 5.0;
};

PowerUps::PowerUps()
{
    clock.restart();
    for (int i = 0; i < powerUpsSize; i++)
    {
        powerUp[i] = false;
        timer[i] = clock.getElapsedTime();
        czasTrwaniaPowerUpa[i] = domyslnyCzasTrwania;
    }
}

void PowerUps::managerPowerUpsTimers()
{
    Time cache_time = clock.restart();
    for (int i = 0; i < powerUpsSize; i++)
    {
        timer[i] = cache_time;
        if (powerUp[i])
        {
            if (timer[i].asSeconds() >= czasTrwaniaPowerUpa[i])
            {
                powerUp[i] = false;
            }
        }
    }

}

string actualSaveFileDirectory = "/data/data/com.minikplayer.mralozerca/files/savedata.sav";

bool SaveActualState(float MrX, long int punkty, long int zycia, float marchX, float marchY, short MRkolor, short mchkolor, bool stworzona)
{
    ofstream plik;
    plik.open(actualSaveFileDirectory);
    if (!plik.good()) return 0;
    plik << "HOME_EXIT" << "\n";
    plik << MrX << "\n";
    plik << punkty << "\n";
    plik << zycia << "\n";
    plik << marchX << "\n";
    plik << marchY << "\n";
    plik << MRkolor << "\n";
    plik << mchkolor << "\n";
    plik << stworzona << "\n";
    plik.close();
    return 1;
}

short checkExitSaveState()
{
    ifstream plik;
    plik.open(actualSaveFileDirectory);
    if (!plik.good()) return 0;
    string firstLine;
    plik >> firstLine;
    if (firstLine == "HOME_EXIT")
    {
        return 2;
    }
    else
    {
        return 1;
    }
}

bool loadActualSaveState(float & MrX, long int & punkty, long int & zycia, float & marchX, float & marchY, short & MRkolor, short & mchkolor, bool & stworzona)
{
    string firstLine;
    ifstream plik;
    plik.open(actualSaveFileDirectory);
    if (!plik.good()) return 0;
    plik >> firstLine;
    plik >> MrX;
    plik >> punkty;
    plik >> zycia;
    plik >> marchX;
    plik >> marchY;
    plik >> MRkolor;
    plik >> mchkolor;
    plik >> stworzona;
    plik.close();
    return 1;
}

void clearExitSaveFile()
{
    ofstream plik;
    plik.open(actualSaveFileDirectory);
    plik << "\n";
    plik.close();
}

void wyswietlIntro(Sprite & minigameSpr, float percent, RenderWindow * okno, Sprite & tloTablicy, View view)
{
	const Color insideKolorTablicy = Color(31, 64, 53);

	if (percent <= 1 / 3.0)
	{
		minigameSpr.setPosition(Vector2f(1920 * 1.5 - ((1920) * percent * 3.05), view.getCenter().y));
	}

	float fadeStarter = 0.6;

	if (percent >= fadeStarter)
	{
		//cout << "Przyciemnienie: " << 255 - ((percent - 1 / 3.0) * 3 / 2.0) * 255 << endl;
		//tloTablicy.setColor(Color(tloTablicy.getColor().r, tloTablicy.getColor().g, tloTablicy.getColor().b, 255 - (((percent - 1/2.0) * 2.0) * 255)));
		tloTablicy.setColor(Color(tloTablicy.getColor().r, tloTablicy.getColor().g, tloTablicy.getColor().b, 255 - ((percent - fadeStarter) / (1 - fadeStarter)) * 255));
		//tloTablicy.setColor(Color(tloTablicy.getColor().r, tloTablicy.getColor().g, tloTablicy.getColor().b, 255 - 0));
		//minigameSpr.setColor(Color(255, 255, 255, 255 - ((percent - 1 / 3.0) * 3 / 2.0) * 255));
		minigameSpr.setColor(Color(255, 255, 255, 255 - ((percent - fadeStarter) / (1 - fadeStarter)) * 255));
		//tloTablicy.setColor(Color(tloTablicy.getColor().r, tloTablicy.getColor().g, tloTablicy.getColor().b, 255 - 128));
		cout << "Przyciemnienie: " << (int)tloTablicy.getColor().a << endl;

	}

	okno->draw(tloTablicy);
	okno->draw(minigameSpr);
}

void obslugazdarzen(Event zdarzenie, RenderWindow * okno)
{
	if (zdarzenie.type == zdarzenie.Closed)
	{
		okno->close();
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

void plikmaxscore(unsigned long long int & maxscore)
{
    ifstream plik;
    plik.open("images/wynik.png");
    if (!plik.good())
    {
        ofstream plikz;
        plikz.open("images/wynik.png");
        plikz << "0";
        plikz.close();
        plik.open("images/wynik.png");
    }
    plik >> maxscore;
    plik.close();
}

int plik(int & resX, int & resY, bool & fullscreen, short & maxfps, bool & vsync, bool wczytaj, bool zapisz, float & predkosc, float & przyspieszenie, float & maxpredkosc, bool & tworzyc, float & glosnosc, float & glosnoscmuzyki, float & glosnoscdzwiekow)
{
    if (zapisz == 1)
        if (tworzyc == 1)
        {
            {
                ofstream plik;
                plik.open("config.ini");
                plik << resX << "\n";
                plik << resY << "\n";
                plik << fullscreen << "\n";
                plik << maxfps << "\n";
                plik << vsync << "\n";
                plik << predkosc << "\n";
                plik << przyspieszenie << "\n";
                plik << maxpredkosc << "\n";
                plik << glosnosc << "\n";
                plik << glosnoscmuzyki << "\n";
                plik << glosnoscdzwiekow << "\n";
                plik << "\n";
                plik << "/* OPIS */";
                plik << "\n // 1: Rozdzielczosc X (px) - Nie zalecana jest ni�sza ni� 1280x720\n // 2: Rozdzielczosc Y (px) - Nie zalecana jest ni�sza ni� 1280x720\n // 3: Pelny ekran ( 0 - okno, 1 - pelny ekran )\n // 4: MAXFPS ( Domyslnie - 60. UWAGA! Zmiana tej wartosci na wieksza niz twoj komputer jest w stanie wygenerowac spowoduje spowolnienia! )\n // 5: VSync ( 0 - wylaczony, 1 - wlaczony )\n // 6: Predkosc startowa marchewek ( domyslnie 3 )\n // 7: Przyspieszenie marchewek ( domyslnie 0.25 )\n // 8: Max Predkosc marchewek ( domyslnie 9 )\n // 9: Glosnosc ogolna\n // 10: Glosnosc muzyki\n // 11: Glosnosc dzwiekow";
            }
        }
    ifstream config;		//Zmienna do odczytywania configu
    config.open("config.ini");
    if (wczytaj == 1)
    {
        if (!config.good())		//Jesli brakuje / nie da si� odczyta� pliku configu stworz go
        {
            int wybor = 2; // MessageBox(NULL, "B��d odczytu pliku konfiguracyjnego! \nPlik jest uszkodzony lub nie istnieje.\nStworzyc nowy?", "Blad odczytu pliku!", MB_YESNOCANCEL | MB_ICONERROR)
            switch (wybor)
            {
            case 0:
            {
                tworzyc = 1;
                break;
            }
            case 1:
            {
                tworzyc = 0;
                break;
            }
            case 2:
            {
                return 1;
            }
            }
            if (tworzyc == 1)
            {
                string str_resX;
                string str_resY;
                str_resX = to_string(VideoMode::getDesktopMode().width);
                str_resY = to_string(VideoMode::getDesktopMode().height);
                ofstream stworzcfg;		//Stw�rz plik configu
                stworzcfg.open("config.ini");
                if (!stworzcfg.good())		//Brak uprawnie� - wy�wietl komunikat
                {
                    //MessageBox(NULL, "Brak uprawnien do stworzenia pliku!", "Blad tworzenia pliku!", MB_OK | MB_ICONERROR);
                    return 2;
                }
                stworzcfg << str_resX + "\n";
                stworzcfg << str_resY + "\n";
                stworzcfg << "1\n";
                stworzcfg << "60\n";
                stworzcfg << "1\n";
                stworzcfg << "3\n";
                stworzcfg << "0.25\n";
                stworzcfg << "9\n";
                stworzcfg << "100\n";
                stworzcfg << "100\n";
                stworzcfg << "100\n";
                stworzcfg << "\n";
                stworzcfg << "/* OPIS */";
                stworzcfg << "\n // 1: Rozdzielczosc X (px) - Nie zalecana jest ni�sza ni� 1280x720\n // 2: Rozdzielczosc Y (px) - Nie zalecana jest ni�sza ni� 1280x720\n // 3: Pelny ekran ( 0 - okno, 1 - pelny ekran )\n // 4: MAXFPS ( Domyslnie - 60. UWAGA! Zmiana tej wartosci na wieksza niz twoj komputer jest w stanie wygenerowac spowoduje spowolnienia! )\n // 5: VSync ( 0 - wylaczony, 1 - wlaczony )\n // 6: Predkosc startowa marchewek ( domyslnie 3 )\n // 7: Przyspieszenie marchewek ( domyslnie 0.25 )\n // 8: Max Predkosc marchewek ( domyslnie 9 )\n // 9: Glosnosc ogolna\n // 10: Glosnosc muzyki\n // 11: Glosnosc dzwiekow";
            }
            resX = VideoMode::getDesktopMode().width;
            resY = VideoMode::getDesktopMode().height;
            fullscreen = 1;
            maxfps = 60;
            vsync = 1;
            predkosc = 3;
            przyspieszenie = 0.25;
            maxpredkosc = 9;
        }
    }
    if (wczytaj == 1)
    {
        config >> resX;
        config >> resY;
        config >> fullscreen;
        config >> maxfps;
        config >> vsync;
        config >> predkosc;
        config >> przyspieszenie;
        config >> maxpredkosc;
        config >> glosnosc;
        config >> glosnoscmuzyki;
        config >> glosnoscdzwiekow;
    }
}

void wycentrujtext(Text & text, int resX, int resY, double skalaY, int wysokosc)
{
    FloatRect textrect = text.getGlobalBounds();
    text.setOrigin(textrect.left + textrect.width / 2.0, textrect.top + textrect.height / 2.0);
    text.setPosition(Vector2f(resX / 2.0, resY / 2.0 + (wysokosc * skalaY)));
}

void wycentrujsprite(Sprite & sprite, int resX, int resY, double skalaY, double skalaX, int wysokosc)
{
    FloatRect spriterect = sprite.getGlobalBounds();
    sprite.setOrigin(spriterect.left + spriterect.width / 2.0, spriterect.top + spriterect.height / 2.0);
    sprite.setPosition(Vector2f(resX / 2.0, resY / 2.0 + (wysokosc * skalaY)));
}

void wycentrujkwadrat(RectangleShape & rectangle, int resX, int resY, double skalaY, double skalaX, int wysokosc)
{
    FloatRect spriterect = rectangle.getGlobalBounds();
    rectangle.setOrigin(spriterect.left + spriterect.width / 2.0, spriterect.top + spriterect.height / 2.0);
    rectangle.setPosition(Vector2f(resX / 2.0, resY / 2.0 + (wysokosc * skalaY)));
}

void ustawrozdzielczosc(short & wybor_rozdzielczosc, short & iloscrozdzielczosci, int resX, int resY, int & resXt, int & resYt)
{
    if (resX == 1024)
        wybor_rozdzielczosc = 1;
    else
        if (resX == 1280)
            wybor_rozdzielczosc = 2;
        else
            if (resX == 1440)
                wybor_rozdzielczosc = 3;
            else
                if (resX == 1600)
                    wybor_rozdzielczosc = 4;
                else
                    if (resX == 1920)
                        wybor_rozdzielczosc = 5;
                    else
                        if (resX == 2560)
                            wybor_rozdzielczosc = 6;
                        else
                            if (resX == 3840)
                                wybor_rozdzielczosc = 7;
                            else
                                if (resX == 5120)
                                    wybor_rozdzielczosc = 8;
                                else
                                    if (resX == 7680)
                                        wybor_rozdzielczosc = 9;
                                    else
                                    {
                                        wybor_rozdzielczosc = 10;
                                        resXt = resX;
                                        resYt = resY;
                                        iloscrozdzielczosci = 10;
                                    }
}

bool ustawmaxfps(short & wybor_maxfps, short & iloscmaxfps, short & maxfps, int & maxfpst)
{
    if (maxfps == 30)
        wybor_maxfps = 0;
    else
        if (maxfps == 60)
            wybor_maxfps = 1;
        else
            if (maxfps == 75)
                wybor_maxfps = 2;
            else
                if (maxfps == 100)
                    wybor_maxfps = 3;
                else
                    if (maxfps == 120)
                        wybor_maxfps = 4;
                    else
                        if (maxfps == 144)
                            wybor_maxfps = 5;
                        else
                            if (maxfps == 240)
                                wybor_maxfps = 6;
                            else
                            {
                                wybor_maxfps = 8;
                                maxfpst = maxfps;
                                iloscmaxfps = iloscmaxfps + 1;
                                if (maxfps > 240)
                                {
                                    //MessageBox(NULL, "Zbyt wysoka czestotliwosc odswiezania ekranu!\nZmniejsz w pliku config.ini", "Zbyt wysoka czestotliwosc odswiezania!", MB_OK | MB_ICONEXCLAMATION);
                                    wybor_maxfps = 2;
                                    maxfps = 60;
                                    maxfpst = 60;
                                    iloscmaxfps = iloscmaxfps - 1;
                                    return 1;
                                }
                            }
    return 0;
}

short menu(int & resX, int & resY, bool & fullscreen, double skalaY, double skalaX, bool fullscreeno, bool vsync, short maxfps, float & predkosc, float & przyspieszenie, float & maxpredkosc, bool & tworzyc, IpAddress & ipservera, unsigned long long int maxscore, float & glosnosc, float & glosnoscmuzyki, float & gloscnoscdzwiekow, Sprite tlo)
{
    int resXt = 0;
    int resYt = 0;
    int resXo = resX;
    int resYo = resY;
    int maxfpst;
    bool fullscreenoriginal = fullscreen;
    bool vsyncoriginal = vsync;
    short maxfpsoriginal = maxfps;
    short iloscrozdzielczosci = 9;
    short iloscmaxfps = 6;
    short wybor_maxfps = 0;
    short wybor_rozdzielczosc = 0;
    ustawrozdzielczosc(wybor_rozdzielczosc, iloscrozdzielczosci, resX, resY, resXt, resYt);
    ustawmaxfps(wybor_maxfps, iloscmaxfps, maxfps, maxfpst);
    resY = (resX / 16) * 9;
    bool zmieniono = 0;
    Sprite logo;
    Texture txt_logo;
    txt_logo.loadFromFile("images/MRalozerca_logo.png");
    logo.setTexture(txt_logo);
    wycentrujsprite(logo, resX, resY, skalaY, skalaX, -350);
    logo.setScale(skalaX / 4, skalaX / 4);

    RenderWindow okno(VideoMode(resX, resY, 32), "Gruniozerca - Menu Glowne");
    if (fullscreen == 1)
    {
        okno.create(VideoMode(resX, resY, 32), "Gruniozerca - Menu", Style::Fullscreen);
    }
    while (okno.isOpen())
    {
        Mouse mouse;
        Event zdarzenie;
        if (okno.pollEvent(zdarzenie))
        {
            if (zdarzenie.type == zdarzenie.Closed) return 1;
            if (zdarzenie.type == zdarzenie.KeyPressed && zdarzenie.key.code == Keyboard::Escape) return 1;
        }
        Text startbutton;
        Text multiplayerbutton;
        Font font;
        Text optionsbutton;
        Text exitbutton;
        Text opcjen;
        Text bestscore;

        if (!font.loadFromFile("gabriola.ttf"))
        {
            return 2;
        }

        if (maxscore > 0)
        {
            bestscore.setFont(font);
            bestscore.setCharacterSize(50 * skalaY);
            bestscore.setStyle(Text::Bold);
            bestscore.setString("Najlepszy wynik: " + to_string(maxscore));
            bestscore.setFillColor(Color::Green);
            if (maxscore > 10000)
            {
                bestscore.setFillColor(Color::Yellow);
            }
            if (maxscore > 100000)
            {
                bestscore.setFillColor(Color(255, 160, 0));
            }
            if (maxscore > 1000000)
            {
                bestscore.setFillColor(Color::Red);
            }
            if (maxscore == 29)
            {
                bestscore.setString("Autor portu: MinikPLayer. Projekt oryginalny: EmuNES");
            }
        }

        opcjen.setFont(font);
        opcjen.setString("OPCJE");
        opcjen.setCharacterSize(100 * skalaY);
        opcjen.setFillColor(Color::Green);
        wycentrujtext(opcjen, resX, resY, skalaY, -400);




        startbutton.setFont(font);
        startbutton.setCharacterSize(100 * skalaY);
        startbutton.setStyle(Text::Bold);
        startbutton.setString("JEDEN GRACZ");
        wycentrujtext(startbutton, resX, resY, skalaY, -20);

        multiplayerbutton.setFont(font);
        multiplayerbutton.setCharacterSize(100 * skalaY);
        multiplayerbutton.setStyle(Text::Bold);
        multiplayerbutton.setString("WIELU GRACZY");
        multiplayerbutton.setFillColor(Color(180, 180, 180));
        wycentrujtext(multiplayerbutton, resX, resY, skalaY, 80);

        optionsbutton.setFont(font);
        optionsbutton.setCharacterSize(100 * skalaY);
        optionsbutton.setStyle(Text::Bold);
        optionsbutton.setString("OPCJE");
        wycentrujtext(optionsbutton, resX, resY, skalaY, 210);

        exitbutton.setFont(font);
        exitbutton.setCharacterSize(100 * skalaY);
        exitbutton.setStyle(Text::Bold);
        exitbutton.setString("WYJSCIE");
        wycentrujtext(exitbutton, resX, resY, skalaY, 320);

        Vector2f mysz(Mouse::getPosition(okno));

        Vector2i touchpos = Touch::getPosition(0);

        mysz.x = touchpos.x;
        mysz.y = touchpos.y;

        if (startbutton.getGlobalBounds().contains(mysz))
        {
            startbutton.setFillColor(Color::Blue);
            if (Touch::isDown(0) || Touch::isDown(1) || Touch::isDown(2))
            {
                return 3;
            }
        }
        else
            startbutton.setFillColor(Color::White);

        if (multiplayerbutton.getGlobalBounds().contains(mysz))
        {
            //multiplayerbutton.setFillColor(Color::Blue);
            /*if (mouse.isButtonPressed(Mouse::Left))
            {
                /*RectangleShape polewpisywania;
                polewpisywania.setSize(Vector2f(1200 * skalaX, 100 * skalaY));
                wycentrujkwadrat(polewpisywania, resX, resY, skalaY, skalaX, 65);
                polewpisywania.setFillColor(Color::Black);
                polewpisywania.move(200 * skalaX, 0);

                Text naglowekmultiplayer;
                naglowekmultiplayer.setFont(font);
                naglowekmultiplayer.setCharacterSize(100 * skalaY);
                naglowekmultiplayer.setStyle(Text::Bold);
                naglowekmultiplayer.setFillColor(Color::Yellow);
                naglowekmultiplayer.setString("WIELU GRACZY");
                wycentrujtext(naglowekmultiplayer, resX, resY, skalaY, -450);

                String wpiszip;
                Text text_ip;
                text_ip.setFont(font);
                text_ip.setCharacterSize(100 * skalaY);
                text_ip.setStyle(Text::Bold);
                text_ip.setPosition(570 * skalaX, resY / 2 - text_ip.getGlobalBounds().height / 2);

                Text text_wpisz_ip;
                text_wpisz_ip.setFont(font);
                text_wpisz_ip.setCharacterSize(100 * skalaY);
                text_wpisz_ip.setStyle(Text::Bold);
                text_wpisz_ip.setString("IP Servera:");
                text_wpisz_ip.setPosition(20, resY / 2 - text_ip.getGlobalBounds().height / 2);
                while (true)
                {
                Event zdarzenie;
                if (okno.pollEvent(zdarzenie))
                {
                if (zdarzenie.type == zdarzenie.Closed) exit(1);
                if (zdarzenie.type == zdarzenie.KeyPressed && zdarzenie.key.code == Keyboard::Escape) break;
                if (zdarzenie.type == zdarzenie.KeyPressed && zdarzenie.key.code == Keyboard::Return)
                {
                IpAddress adres(text_ip.getString());
                //IpAddress adres("127.0.0.1");
                ipservera = adres;
                cout << "Ipservera: " << ipservera << endl;
                if (( ipservera == IpAddress::None ) || ( ipservera == "" ) || ( ipservera == "0.0.0.0" ))
                {
                text_ip.setString("BLEDNY ADRES IP");
                text_ip.setFillColor(Color::Red);
                //wycentrujtext(text_ip, resX, resY, skalaY, 0);
                Clock zegar;
                Time czas;
                czas = zegar.getElapsedTime();
                while (czas.asSeconds() < 2)
                {
                okno.clear();
                okno.draw(text_ip);
                okno.display();
                czas = zegar.getElapsedTime();
                }
                text_ip.setFillColor(Color::White);
                wpiszip.clear();
                continue;
                }
                cout << "Ipservera: " << ipservera << endl;
                return 4;
                }
                if (text_ip.getGlobalBounds().width < 1125 * skalaX)
                {
                if (zdarzenie.type == zdarzenie.KeyPressed && zdarzenie.key.code == Keyboard::Num0) wpiszip += "0";
                if (zdarzenie.type == zdarzenie.KeyPressed && zdarzenie.key.code == Keyboard::Num1) wpiszip += "1";
                if (zdarzenie.type == zdarzenie.KeyPressed && zdarzenie.key.code == Keyboard::Num2) wpiszip += "2";
                if (zdarzenie.type == zdarzenie.KeyPressed && zdarzenie.key.code == Keyboard::Num3) wpiszip += "3";
                if (zdarzenie.type == zdarzenie.KeyPressed && zdarzenie.key.code == Keyboard::Num4) wpiszip += "4";
                if (zdarzenie.type == zdarzenie.KeyPressed && zdarzenie.key.code == Keyboard::Num5) wpiszip += "5";
                if (zdarzenie.type == zdarzenie.KeyPressed && zdarzenie.key.code == Keyboard::Num6) wpiszip += "6";
                if (zdarzenie.type == zdarzenie.KeyPressed && zdarzenie.key.code == Keyboard::Num7) wpiszip += "7";
                if (zdarzenie.type == zdarzenie.KeyPressed && zdarzenie.key.code == Keyboard::Num8) wpiszip += "8";
                if (zdarzenie.type == zdarzenie.KeyPressed && zdarzenie.key.code == Keyboard::Num9) wpiszip += "9";
                if (zdarzenie.type == zdarzenie.KeyPressed && zdarzenie.key.code == Keyboard::Period) wpiszip += ".";
                }
                if (zdarzenie.type == zdarzenie.KeyPressed && zdarzenie.key.code == Keyboard::BackSpace && wpiszip.getSize() > 0) wpiszip.erase(wpiszip.getSize() - 1, 1);
                }
                text_ip.setString(wpiszip);
                okno.clear(Color(77,77,77));
                okno.draw(polewpisywania);
                okno.draw(text_ip);
                okno.draw(text_wpisz_ip);
                okno.draw(naglowekmultiplayer);
                okno.display();
                }
                return 4;

            }*/

        }
        else
            multiplayerbutton.setFillColor(Color(180,180,180));

        /*if (optionsbutton.getGlobalBounds().contains(mysz))
        {
            optionsbutton.setFillColor(Color::Blue);
            if (Touch::isDown(0) || Touch::isDown(1) || Touch::isDown(2))
            {
                while (1)
                {

                    Text rozdzielczosc;
                    rozdzielczosc.setFont(font);
                    rozdzielczosc.setString("Rozdzielczosc:");
                    rozdzielczosc.setCharacterSize(60 * skalaY);
                    rozdzielczosc.setPosition(200 * skalaX, 350 * skalaY);
                    rozdzielczosc.setFillColor(Color::Yellow);

                    Text fullscreen;
                    fullscreen.setFont(font);
                    fullscreen.setString("Pelny ekran:");
                    fullscreen.setCharacterSize(60 * skalaY);
                    fullscreen.setPosition(200 * skalaX, 450 * skalaY);
                    fullscreen.setFillColor(Color::Yellow);

                    Text VSync;
                    VSync.setFont(font);
                    VSync.setString("Synchronizacja pionowa:");
                    VSync.setCharacterSize(60 * skalaY);
                    VSync.setPosition(200 * skalaX, 550 * skalaY);
                    VSync.setFillColor(Color::Yellow);

                    Text MaxFPS;
                    MaxFPS.setFont(font);
                    MaxFPS.setString("Max FPS:");
                    MaxFPS.setCharacterSize(60 * skalaY);
                    MaxFPS.setPosition(200 * skalaX, 650 * skalaY);
                    MaxFPS.setFillColor(Color::Yellow);

                    string resx = to_string(resX);
                    string resy = to_string(resY);
                    Text rozdzielczoscw;
                    rozdzielczoscw.setFont(font);
                    rozdzielczoscw.setString(resx + "x" + resy);
                    if (wybor_rozdzielczosc == 10)
                    {
                        rozdzielczoscw.setString("Niestandardowa: " + resx + "x" + resy);
                    }
                    rozdzielczoscw.setCharacterSize(60 * skalaY);
                    rozdzielczoscw.setPosition(600 * skalaX, 350 * skalaY);
                    rozdzielczoscw.setFillColor(Color(192, 192, 192, 255));

                    string fullscr = "tak";
                    if (fullscreeno == 0)
                        fullscr = "nie";
                    Text fullscreenw;
                    fullscreenw.setFont(font);
                    fullscreenw.setString(fullscr);
                    fullscreenw.setCharacterSize(60 * skalaY);
                    fullscreenw.setPosition(500 * skalaX, 450 * skalaY);
                    fullscreenw.setFillColor(Color(192, 192, 192, 255));

                    string Vsync = "tak";
                    if (vsync == 0)
                        Vsync = "nie";
                    Text VSyncw;
                    VSyncw.setFont(font);
                    VSyncw.setString(Vsync);
                    VSyncw.setCharacterSize(60 * skalaY);
                    VSyncw.setPosition(700 * skalaX, 550 * skalaY);
                    VSyncw.setFillColor((Color(192, 192, 192, 255)));

                    String MaxFPSwS = to_string(maxfps);
                    Text MaxFPSw;
                    MaxFPSw.setFont(font);
                    MaxFPSw.setString(MaxFPSwS);
                    if (wybor_maxfps == 7)
                    {
                        MaxFPSw.setString("Niestandardowa: " + maxfpst);
                    }
                    MaxFPSw.setCharacterSize(60 * skalaY);
                    MaxFPSw.setPosition(470 * skalaX, 650 * skalaY);
                    MaxFPSw.setFillColor((Color(192, 192, 192, 255)));

                    Vector2f mysz(Mouse::getPosition(okno));

                    mysz.x = touchpos.x;
                    mysz.y = touchpos.y;

                    if (okno.pollEvent(zdarzenie))
                    {
                        if (zdarzenie.type == zdarzenie.Closed) return 1;
                        if (zdarzenie.type == zdarzenie.KeyPressed && zdarzenie.key.code == Keyboard::Escape)
                        {
                            if (resXo != resX || fullscreeno != fullscreenoriginal || vsync != vsyncoriginal || maxfps != maxfpsoriginal)
                            {
                                tworzyc = 1;
                                //plik(resX, resY, fullscreeno, maxfps, vsync, 0, 1, predkosc, przyspieszenie, maxpredkosc, tworzyc, glosnosc, glosnoscmuzyki, gloscnoscdzwiekow);
                                //MessageBox(NULL, "Uruchom ponownie program aby zobaczyc zmiany", "Uruchom ponownie", MB_OK | MB_ICONEXCLAMATION);
                                return 10;
                                tworzyc = 0;
                                break;
                            }
                            break;
                        }
                        if (rozdzielczoscw.getGlobalBounds().contains(mysz))
                        {
                            if (Touch::isDown(0) || Touch::isDown(1) || Touch::isDown(2))
                            {
                                if (wybor_rozdzielczosc < iloscrozdzielczosci)
                                    wybor_rozdzielczosc++;
                                else
                                    wybor_rozdzielczosc = 0;
                            }
                           /* if (Touch::isDown(0) || Touch::isDown(1) || Touch::isDown(2))
                            {
                                if (wybor_rozdzielczosc > 0)
                                {
                                    wybor_rozdzielczosc--;
                                }
                                else
                                    wybor_rozdzielczosc = iloscrozdzielczosci;
                            }*/
/*
                            if (wybor_rozdzielczosc == 0)
                                resX = 960;
                            if (wybor_rozdzielczosc == 1)
                                resX = 1024;
                            if (wybor_rozdzielczosc == 2)
                                resX = 1280;
                            if (wybor_rozdzielczosc == 3)
                                resX = 1440;
                            if (wybor_rozdzielczosc == 4)
                                resX = 1600;
                            if (wybor_rozdzielczosc == 5)
                                resX = 1920;
                            if (wybor_rozdzielczosc == 6)
                                resX = 2560;
                            if (wybor_rozdzielczosc == 7)
                                resX = 3840;
                            if (wybor_rozdzielczosc == 8)
                                resX = 5120;
                            if (wybor_rozdzielczosc == 9)
                                resX = 7680;
                            if (wybor_rozdzielczosc == 10)
                                resX = resXt;
                            resY = (resX / 16) * 9;
                        }

                        if (fullscreenw.getGlobalBounds().contains(mysz) && zdarzenie.type != Event::MouseMoved)
                        {
                            if (Touch::isDown(0) || Touch::isDown(1) || Touch::isDown(2) || mouse.isButtonPressed(Mouse::Right))
                            {
                                if (fullscreeno == 0)
                                    fullscreeno = 1;
                                else fullscreeno = 0;
                                zmieniono = 1;
                            }
                        }

                        if (MaxFPSw.getGlobalBounds().contains(mysz) && zdarzenie.type != Event::MouseMoved)
                        {
                            if (Touch::isDown(0) || Touch::isDown(1) || Touch::isDown(2))
                            {
                                if (wybor_maxfps < iloscmaxfps)
                                    wybor_maxfps++;
                                else
                                    wybor_maxfps = 0;
                            }
                            if (mouse.isButtonPressed(Mouse::Right))
                            {
                                if (wybor_maxfps > 0)
                                    wybor_maxfps--;
                                else
                                    wybor_maxfps = iloscmaxfps;
                            }
                            if (wybor_maxfps == 0)
                                maxfps = 30;
                            if (wybor_maxfps == 1)
                                maxfps = 60;
                            if (wybor_maxfps == 2)
                                maxfps = 75;
                            if (wybor_maxfps == 3)
                                maxfps = 100;
                            if (wybor_maxfps == 4)
                                maxfps = 120;
                            if (wybor_maxfps == 5)
                                maxfps = 144;
                            if (wybor_maxfps == 6)
                                maxfps = 240;
                            if (wybor_maxfps == 7)
                                maxfps = maxfpst;
                        }

                        if (VSyncw.getGlobalBounds().contains(mysz) && zdarzenie.type != Event::MouseMoved)
                        {
                            if (Touch::isDown(0) || Touch::isDown(1) || Touch::isDown(2))
                            {
                                if (vsync == 0)
                                {
                                    vsync = 1;
                                }
                                else
                                    vsync = 0;
                            }
                        }
                    }

                    if (rozdzielczoscw.getGlobalBounds().contains(mysz))
                        rozdzielczoscw.setFillColor(Color::Cyan);
                    else
                    {
                        rozdzielczoscw.setFillColor((Color(192, 192, 192, 255)));
                        if (resXo != resX)
                            rozdzielczoscw.setFillColor(Color(255, 50, 50, 255));
                    }

                    if (fullscreenw.getGlobalBounds().contains(mysz))
                        fullscreenw.setFillColor(Color::Cyan);
                    else
                    {
                        fullscreenw.setFillColor((Color(192, 192, 192, 255)));
                        if (fullscreeno != fullscreenoriginal)
                            fullscreenw.setFillColor(Color(255, 50, 50, 255));
                    }

                    if (MaxFPSw.getGlobalBounds().contains(mysz))
                        MaxFPSw.setFillColor(Color::Cyan);
                    else
                    {
                        MaxFPSw.setFillColor((Color(192, 192, 192, 255)));
                        if (maxfps != maxfpsoriginal)
                            MaxFPSw.setFillColor(Color(255, 50, 50, 255));
                    }

                    if (VSyncw.getGlobalBounds().contains(mysz))
                        VSyncw.setFillColor(Color::Cyan);
                    else
                    {
                        VSyncw.setFillColor((Color(192, 192, 192, 255)));
                        if (vsync != vsyncoriginal)
                            VSyncw.setFillColor(Color(255, 50, 50, 255));
                    }
                    okno.clear();
                    okno.draw(tlo);
                    okno.draw(rozdzielczosc);
                    okno.draw(fullscreen);
                    okno.draw(VSync);
                    okno.draw(MaxFPS);
                    okno.draw(rozdzielczoscw);
                    okno.draw(fullscreenw);
                    okno.draw(VSyncw);
                    okno.draw(MaxFPSw);
                    okno.draw(opcjen);
                    okno.display();
                }
            }
        }
        else
            optionsbutton.setFillColor(Color::White);
            */
        if (exitbutton.getGlobalBounds().contains(mysz))
        {
            exitbutton.setFillColor(Color::Blue);
            if (Touch::isDown(0) || Touch::isDown(1) || Touch::isDown(2))
            {
                return 1;
            }
        }
        else
            exitbutton.setFillColor(Color::White);

        okno.clear();
        okno.draw(tlo);
        okno.draw(logo);
        okno.draw(startbutton);
        okno.draw(multiplayerbutton);
        okno.draw(optionsbutton);
        okno.draw(exitbutton);
        okno.draw(bestscore);
        okno.display();
    }
}

void wyswietlwynik(long int punkty, Text & wynik, string string_wynik)
{
    string_wynik = to_string(punkty);
    string_wynik = "Punkty: " + string_wynik;
    wynik.setString(string_wynik);
}

int uziemmarchewke(Texture txt_marchewkawziemi[3], Sprite & marchewkawziemi, Sprite marchewka, short mchkolor, int ktoram)
{
    marchewkawziemi.setPosition(marchewka.getPosition().x - 26, marchewka.getPosition().y);
    marchewkawziemi.setTexture(txt_marchewkawziemi[mchkolor]);
    ktoram++;
    return ktoram;
}

void zmienkolor(Sprite & grunio, Texture txt_grunio[], short & grkolor, short & mchkolor, bool wgore)
{
    if (wgore == 1)
    {
        if (grkolor < 2)
            grkolor++;
        else
            grkolor = 0;
    }
    if (wgore == 0)
    {
        if (grkolor > 0)
            grkolor--;
        else
            grkolor = 2;
    }
    grunio.setTexture(txt_grunio[grkolor]);
}

void animacjapunktownadgr(int & klatkaanimacji_punktynadgr, Text & text_punktynadgr, double maxfps, short ciag, bool & zjedzona, float grX, float grY, float skalaX, float skalaY)
{
    FloatRect spriterect = text_punktynadgr.getGlobalBounds();
    text_punktynadgr.setPosition(grX - (spriterect.width / 2), grY - (spriterect.height * 2));
    int dlugosc = 2;
    if (klatkaanimacji_punktynadgr <= 5.0 * dlugosc * (maxfps / 60))
        text_punktynadgr.setFillColor(Color(0, 200, 255, 255));
    if (klatkaanimacji_punktynadgr >= 5.0 * dlugosc * (maxfps / 60))
        text_punktynadgr.setFillColor(Color(0, 200, 255, 200));
    if (klatkaanimacji_punktynadgr >= 6.0 * dlugosc * (maxfps / 60))
        text_punktynadgr.setFillColor(Color(0, 200, 255, 175));
    if (klatkaanimacji_punktynadgr >= 7.0 * dlugosc * (maxfps / 60))
        text_punktynadgr.setFillColor(Color(0, 200, 255, 150));
    if (klatkaanimacji_punktynadgr >= 8.0 * dlugosc * (maxfps / 60))
        text_punktynadgr.setFillColor(Color(0, 200, 255, 125));
    if (klatkaanimacji_punktynadgr >= 9.0 * dlugosc * (maxfps / 60))
        text_punktynadgr.setFillColor(Color(0, 200, 255, 100));
    if (klatkaanimacji_punktynadgr >= 10.0 * dlugosc * (maxfps / 60))
        text_punktynadgr.setFillColor(Color(0, 200, 255, 75));
    if (klatkaanimacji_punktynadgr >= 11.0 * dlugosc * (maxfps / 60))
        text_punktynadgr.setFillColor(Color(0, 200, 255, 50));
    if (klatkaanimacji_punktynadgr >= 12.0 * dlugosc * (maxfps / 60))
        text_punktynadgr.setFillColor(Color(0, 200, 255, 25));
    if (klatkaanimacji_punktynadgr >= 13.0 * dlugosc * (maxfps / 60))
        text_punktynadgr.setFillColor(Color(0, 200, 255, 0));
    if (klatkaanimacji_punktynadgr >= 14.0 * dlugosc * (maxfps / 60))
    {
        klatkaanimacji_punktynadgr = 0;
        zjedzona = 0;
        cout << spriterect.width << endl;
    }
    text_punktynadgr.move(0, klatkaanimacji_punktynadgr * -2.0 / (maxfps / 60));
    klatkaanimacji_punktynadgr++;
}

void wyswietlzdobytepunkty(short & ciag, Text & text_punktynadgr, int & klatkaanimacji_punktynadgr, int maxfps, bool & zjedzona, float grX, float grY, float skalaX, float skalaY)
{
    int punkty = 0;
    if (ciag == 1)
        punkty = 10;
    if (ciag == 2)
        punkty = 50;
    if (ciag == 3)
        punkty = 100;
    if (ciag == 4)
        punkty = 200;
    if (ciag == 5)
        punkty = 500;
    if (ciag == 6)
        punkty = 1000;
    if (ciag == 7)
        punkty = 2000;
    if (ciag >= 8)
        punkty = 5000;
    string pkt = to_string(punkty);
    text_punktynadgr.setString(pkt);
    text_punktynadgr.setPosition(grX, grY);
    animacjapunktownadgr(klatkaanimacji_punktynadgr, text_punktynadgr, maxfps, ciag, zjedzona, grX, grY, skalaX, skalaY);
}

void dodajpunkty(long int & punkty, short & ciag)
{
    if (ciag == 0)
        punkty += 10;
    if (ciag == 1)
        punkty += 50;
    if (ciag == 2)
        punkty += 100;
    if (ciag == 3)
        punkty += 200;
    if (ciag == 4)
        punkty += 500;
    if (ciag == 5)
        punkty += 1000;
    if (ciag == 6)
        punkty += 2000;
    if (ciag >= 7)
        punkty += 5000;
    ciag++;
}

bool sprawdzczyzjedzona(Sprite & marchewka, Sprite grunio, bool & stworzona, float & marchY, float & predkoscmarchewki, float przyspieszenie, int maxprmarchewki, short grkolor, short mchkolor, long int & punkty, short & ciag)
{
    FloatRect grunioBOX = grunio.getGlobalBounds();
    FloatRect MarchewkaBOX = marchewka.getGlobalBounds();
    if (grunioBOX.intersects(MarchewkaBOX))
    {
        if (grkolor == mchkolor)
        {
            marchY = -1000;
            stworzona = 0;
            if (predkoscmarchewki < maxprmarchewki)
                predkoscmarchewki = predkoscmarchewki + przyspieszenie;
            dodajpunkty(punkty, ciag);
            vibrate(50);
            return 1;
        }
    }
    return 0;
}

void pokazzycia(Sprite zycie[], int zycia, Texture & txt_zycie, Texture & txt_pusty, Text & text_zycia)
{
    text_zycia.setString("");
    for (int i = 0; i < 5; i++)
    {
        zycie[i].setTexture(txt_pusty);
    }
    if (zycia > 5)
    {
        string ilosczyc = to_string(zycia);
        ilosczyc = ilosczyc + "x";
        text_zycia.setString(ilosczyc);
        zycie[0].setTexture(txt_zycie);
    }
    else
    {
        if (zycia == 1)
        {
            zycie[0].setTexture(txt_zycie);
        }
        if (zycia == 2)
        {
            for (int i = 0; i < 2; i++)
            {
                zycie[i].setTexture(txt_zycie);
            }
        }
        if (zycia == 3)
        {
            for (int i = 0; i < 3; i++)
            {
                zycie[i].setTexture(txt_zycie);
            }
        }
        if (zycia == 4)
        {
            for (int i = 0; i < 4; i++)
            {
                zycie[i].setTexture(txt_zycie);
            }
        }
        if (zycia == 5)
        {
            for (int i = 0; i < 5; i++)
            {
                zycie[i].setTexture(txt_zycie);
            }
        }
    }
}

bool sprawdzmarchewke(Sprite & marchewka, double skalaY, bool & stworzona, float & marchY, int resY, int marchsizeY, float predkoscmarchewki, float przyspieszenie, int maxprmarchewki, short mchkolor, short grkolor, short & ciag, int & ktoram, Sprite & marchewkawziemi, Texture txt_marchewkawziemi[3], long int & zycia)
{
    if (marchY > resY - (225 * skalaY) + marchsizeY)
    {
        ktoram = uziemmarchewke(txt_marchewkawziemi, marchewkawziemi, marchewka, mchkolor, ktoram);
        marchY = -1000;
        stworzona = 0;
        if (predkoscmarchewki < maxprmarchewki)
            predkoscmarchewki = predkoscmarchewki + przyspieszenie;
        ciag = 0;
        zycia--;
		vibrate(250);
        return 1;
    }
    return 0;
}

void sprawdzciag(short & ciag, long int & zycia)
{
    int wartosczanowezycie = 20;
    if (ciag >= wartosczanowezycie)
    {
        zycia++;
        ciag = 8;
    }
}

void ruszmarchewke(Sprite & marchewka, double skalaX, double skalaY, bool stworzona, float marchX, float & marchY, float predkoscmarchewki, Clock zegar, Time czas)
{
    if (stworzona == 1)
    {
        marchY = marchY + ((predkoscmarchewki * skalaY * czas.asMilliseconds()) / 10);
        marchewka.setPosition(marchX, marchY);
    }
}
void stworzmarchewke(Sprite & marchewka, double & skalaX, double & skalaY, Texture txt_marchewka[], float & grX, bool & stworzona, int & resX, float & marchX, float & marchY, float & predkoscmarchewki, short & mchkolor)
{
    short maxodstep = 1200;
    if (stworzona == 0)
    {
        while (1)
        {
            int pozX = (rand() * 1246) % resX;
            int pozY = (rand() * 4212) % 200;
            if (( grX - pozX < maxodstep - (predkoscmarchewki * 50) && grX - pozX > -maxodstep + (predkoscmarchewki * 50) || grX == pozX) && pozX > 0 && pozX < resX && pozY > 0)
            {
                marchX = pozX * skalaX;
                marchY = -pozY * skalaY - marchewka.getGlobalBounds().height;
                marchewka.setPosition(marchX, marchY);
                stworzona = 1;
                break;
            }
        }

        mchkolor = rand() % 3;
        marchewka.setTexture(txt_marchewka[mchkolor]);
    }
}

bool gameover(Text & text_gameover, int & klatkaanimacji_gameover, int maxfps)
{
    if (klatkaanimacji_gameover > 0 && klatkaanimacji_gameover * 80 / maxfps < 255)
        text_gameover.setFillColor(Color(255, 0, 0, klatkaanimacji_gameover * 80 / maxfps));
    if (klatkaanimacji_gameover * 80 / maxfps > 300 && klatkaanimacji_gameover * 80 / maxfps < 555)
        text_gameover.setFillColor(Color(255, 0, 0, 555 - (klatkaanimacji_gameover * 80 / maxfps)));
    if (klatkaanimacji_gameover * 80 / maxfps < 610)
        klatkaanimacji_gameover++;
    if (klatkaanimacji_gameover * 80 / maxfps > 600)
        return 1;
    else return 0;
}

bool sprawdzczyzginales(long int zycia, bool & stworzona, Sprite & grunio, Music & muzyka, int & klatkaanimacji_smierc, int maxfps, float & grY, float & grX, float & grXt, Text & text_gameover, int & klatkaanimacji_gameover, int resY, Clock & zegaranimacjismierci, Time & czasanimacjismierci, float & czas, Clock & deathtimer, Time deathtime, Sprite & gabeczka, int resX)
{
    //czasanimacjismierci = zegaranimacjismierci.getElapsedTime();
	if (zycia < 0)
	{
		//czas = czasanimacjismierci.asMilliseconds();
		cout << "klatka: " << klatkaanimacji_smierc << " czas: " << czas << endl;
		if (klatkaanimacji_smierc == 0)
		{
			deathtimer.restart();
			grXt = grX;
			klatkaanimacji_smierc++;
			gabeczka.setPosition(resX + 250, grY - gabeczka.getGlobalBounds().height / 1.5);
		}
		deathtime = deathtimer.getElapsedTime();
		muzyka.stop();
		stworzona = 0;
		if (deathtime.asMilliseconds() <= 4000)
		{
			grX = grXt;
			gabeczka.move(-10, 0);
		}
		if (gabeczka.getGlobalBounds().left < grunio.getGlobalBounds().left)
		{
			grXt = 10000000;
		}
		if (deathtime.asSeconds() > 4)
		{
			klatkaanimacji_smierc = 0;
			return 1;
		}
		else
			return 0;
	}
	else
		return 0;
	
}

short przyciskipauzy(Text & przycisk_pauza_wznow, Text & przycisk_pauza_restart, Text & przycisk_pauza_wyjscie, Vector2f mysz, bool sieciowy)
{
    Vector2i touch = Touch::getPosition(0);

    mysz.x = touch.x;
    mysz.y = touch.y;
    if (przycisk_pauza_wznow.getGlobalBounds().contains(mysz))
    {
        przycisk_pauza_wznow.setFillColor(Color::Cyan);
        if (Touch::isDown(0) || Touch::isDown(1) || Touch::isDown(2))
            return 1;
    }
    else
        przycisk_pauza_wznow.setFillColor(Color(192, 192, 192, 255));

    if (przycisk_pauza_restart.getGlobalBounds().contains(mysz) && sieciowy == 0)
    {
        przycisk_pauza_restart.setFillColor(Color::Cyan);
        if (Touch::isDown(0) || Touch::isDown(1) || Touch::isDown(2))
            return 2;
    }
    else
        przycisk_pauza_restart.setFillColor(Color(192, 192, 192, 255));

    if (przycisk_pauza_wyjscie.getGlobalBounds().contains(mysz))
    {
        przycisk_pauza_wyjscie.setFillColor(Color::Cyan);
        if (Touch::isDown(0) || Touch::isDown(1) || Touch::isDown(2))
            return 3;
    }
    else
        przycisk_pauza_wyjscie.setFillColor(Color(192, 192, 192, 255));

    return 0;
}


int main()
{

	VideoMode screenmode(VideoMode::getDesktopMode());

    Clock deathtimer;
    Time deathtime;

    Clock animrate;
    Time animtime;

    Clock FishAnimClock;
    Time FishAnimTime;

    Clock tickrate;
    Time ticktime;

    // Parametr okre�la, czy gra si� toczy czy nie
    bool czyjestwgrze = 0;

    // Parametr okre�la, kt�rym u�ytkownikiem si� jest
    short userid = 0;

    //Parametr okre�la, czy przeciwnik nadal jest po��czony czy nie
    bool przeciwnikjestwgrze = 0;

    // Parametr okre�la, czy po��czono z serverem
    bool polaczono = 0;

    // Parametr okre�la, czy przeciwnik wog�le pojawi� si� w grze
    bool przeciwnikbylwgrze = 0;

    int klatka = 0;
    bool multiplayer = 0;	// Parametr okre�la, czy jest to gra wieloosobowa
    bool sieciowy = 0;	// Parametr okre�la, czy jest to gra przez sie�
    IpAddress ipservera;	// IP Servera
    srand(time(NULL));
    unsigned long long int maxscore = 0;	// Najwy�szy wynik
    int klatkaanimacji_gameover = 0;
    int klatkaanimacji_smierc = 0;
    short odpauzowac = 0;	// Parametr okre�la, czy u�ytkownik zapauzowa� gr�
                            //long int zycia = 3;	// Ilo�� �y�
    int klatkaanimacji_punktynadgr = 0;	// Okre�la, kt�ra ,,klatka animacji" ma by� wy�wietlona podczas wy�wietlania zdobytych punkt�w
    bool zjedzona[2] = { 0,0 };	// Czy marchewka jest zjedzona
    bool tworzyc = 0;
    int ktoram = 0;
    short ciag[2] = { 0,0 };	// Kt�ra z rz�du zjedzona marchewka
                                //long int punkty[2] = { 0,0 };	// Ilo�� punkt�w
   // int resX = VideoMode::getDesktopMode().width;		//Rozdzielczo�� ekranu X - domy�lna warto��
   // int resY = VideoMode::getDesktopMode().height;		//Rozdzielczo�� ekranu Y - domy�lna warto��

	//int resX = screenmode.width;
	//int resY = screenmode.height;

	int resX = 1920;
	int resY = 1080;

    //double skalaX = resX / 1920.0;	// Skala ekranu w odniesieniu do FullHD w osi X
    //double skalaY = resY / 1080.0;	// Skala ekranu w odniesieniu do FullHD w osi Y
	double skalaX = 1;
	double skalaY = 1;
	
	bool fullscreen = 0;		//Pe�ny ekran - domy�lna warto��
    short maxfps = 60;		//Max FPS - domy�lna warto��
    bool vsync = 0;		//Synchronizacja pionowa - domy�lna warto��
    float maxprmarchewki = 9;	// Okre�la max pr�dko�� jak� marchewka mo�e osi�gn��
    float przyspieszanie = 0.25;	// Okre�la jak szybko marchewka b�dzie zwi�ksza� swoj� pr�dko��
    float startpredkoscmarchewki = 3;
    float predkoscmarchewki[2] = { 3, 3 };	// Okre�la pocz�tkow� pr�dko�� marchewki
    float glosnosc = 100;	// G�o�no�� g��wna gry
    float glosnoscmuzyki = 100 * (glosnosc / 100);	// G�o�no�� muzyki
    float glosnoscdzwiekow = 100 * (glosnosc / 100);	// G�o�no�� d�wi�k�w

	bool WcisnietyPrzyciskZmianyKoloru = 0;

    float animframetime = 50; // Parametr okre�la aktualn� pr�dko�� zmiany klatki animacji ( w milisekundach )
    float animframetimemove = 50; // Parametr okre�la domy�lny co jaki czas zmieniana b�dzie klatka animacji w ruchu ( w milisekundach )
    float animframetimestay = 250; // Parametr okre�la domy�lny co jaki czas zmieniana b�dzie klatka animacji w spoczynku ( w milisekundach )

    float FishAnimationTime = 600; // Parametr okre�la jak d�uga ma by� obr�t rybki ( w milisekundach )

    bool wRuchu = 0; // Okre�la czy Grunio si� porusza

    float czasanimacjismiercisuma = 0;

    Clock zegaranimacjismierci[2];
    Time czasanimacjismierci[2];

    //plik(resX, resY, fullscreen, maxfps, vsync, 1, 0, predkoscmarchewki[0], przyspieszanie, maxprmarchewki, tworzyc, glosnosc, glosnoscmuzyki, glosnoscdzwiekow);
    startpredkoscmarchewki = predkoscmarchewki[0];
    predkoscmarchewki[1] = predkoscmarchewki[0];
    //skalaX = resX / 1920.0;
    //skalaY = resY / 1080.0;
    sf::SoundBuffer plik_dzwiek_dobrze;
    plik_dzwiek_dobrze.loadFromFile("sounds/dobrze.wav");
    sf::SoundBuffer plik_dzwiek_zle;
    plik_dzwiek_zle.loadFromFile("sounds/zle.wav");
    sf::Sound dzwiek_zebraniemarchewki;
    sf::Sound dzwiek_marchewkauciekla;
    dzwiek_zebraniemarchewki.setBuffer(plik_dzwiek_dobrze);
    dzwiek_marchewkauciekla.setBuffer(plik_dzwiek_zle);
    dzwiek_zebraniemarchewki.setVolume(glosnoscdzwiekow);
    dzwiek_marchewkauciekla.setVolume(glosnoscdzwiekow);
    plikmaxscore(maxscore);
    sf::Music muzyka;
    if (!muzyka.openFromFile("sounds/muzyka.ogg"))
        //MessageBox(NULL, "Nie uda�o si� wczyta� muzyki!\nUpewnij si�, �e plik muzyka.ogg jest w folderze sounds.", "Blad wczytywania muzyki!", MB_OK | MB_ICONERROR);
        muzyka.play();
    muzyka.setLoop(true);
    muzyka.setVolume(glosnoscmuzyki);
    unsigned short porttcp = 12345;
    unsigned short portudpupload = 12346;
    unsigned short portudpdownload = 12347;
    UdpSocket socket;

    glosnoscmuzyki = glosnoscmuzyki * (glosnosc / 100);
    glosnoscdzwiekow = glosnoscdzwiekow * (glosnosc / 100);

    dzwiek_marchewkauciekla.setVolume(glosnoscdzwiekow);
    dzwiek_zebraniemarchewki.setVolume(glosnoscdzwiekow);
    muzyka.setVolume(glosnoscmuzyki);

    Sprite marchewka[2];
    Sprite tlo;
    Sprite pauza;
    Sprite zycie[2][5];
    Sprite marchewkawziemi[1000];
    Sprite gabeczka;
    Sprite PrzyciskWPrawo;
    Sprite PrzyciskWLewo;
	Sprite PrzyciskZmianyKoloru;

	Texture txt_przycisk_zmiany_koloru;
    Texture txt_przycisk_w_prawo;
    Texture txt_przycisk_w_lewo;
    Texture txt_pusty;
    Texture txt_marchewkawziemi[3];
    Texture txt_tlo;
    Texture txt_marchewka[3];
    Texture txt_marchewka2[3];
    Texture txt_grunio2[3];
    Texture txt_pauza;
    Texture txt_zycie;
    Texture txt_gabeczka;

    Grunio grunio[2];
    grunio[0].x = resX / 2;
    grunio[0].y = resY - 220;
    grunio[1].x = resX / 2;
    grunio[1].y = resY - 220;

    txt_tlo.loadFromFile("images/tablica.png");
    txt_marchewka[0].loadFromFile("images/rybka_cz.png");
    txt_marchewka[1].loadFromFile("images/rybka_zi.png");
    txt_marchewka[2].loadFromFile("images/rybka_ni.png");
    grunio[0].textura[0].loadFromFile("images/MRal_cz.png");
    grunio[0].textura[1].loadFromFile("images/MRal_zi.png");
    grunio[0].textura[2].loadFromFile("images/MRal_ni.png");
    grunio[1].textura[0].loadFromFile("images/grunio2_cz.png");
    grunio[1].textura[1].loadFromFile("images/grunio2_zi.png");
    grunio[1].textura[2].loadFromFile("images/grunio2_ni.png");
    txt_pauza.loadFromFile("images/pauza.png");
    txt_marchewkawziemi[0].loadFromFile("images/rybka_spadla.png");
    txt_marchewkawziemi[1].loadFromFile("images/rybka_spadla.png");
    txt_marchewkawziemi[2].loadFromFile("images/rybka_spadla.png");
    txt_marchewka2[0].loadFromFile("images/marchewka2_cz.png");
    txt_marchewka2[1].loadFromFile("images/marchewka2_zi.png");
    txt_marchewka2[2].loadFromFile("images/marchewka2_ni.png");
    txt_zycie.loadFromFile("images/zycie.png");
    txt_pusty.loadFromFile("images/pusty.png");
    txt_gabeczka.loadFromFile("images/gabeczka.png");
    txt_przycisk_w_prawo.loadFromFile("images/przycisk_w_prawo.png");
    txt_przycisk_w_lewo.loadFromFile("images/przycisk_w_lewo.png");
	txt_przycisk_zmiany_koloru.loadFromFile("images/przycisk_zmiany_kolorow.png");

    PrzyciskWPrawo.setTexture(txt_przycisk_w_prawo);
    PrzyciskWLewo.setTexture(txt_przycisk_w_lewo);
	PrzyciskZmianyKoloru.setTexture(txt_przycisk_zmiany_koloru);

	//resX = VideoMode::getDesktopMode().width;
	//resY = VideoMode::getDesktopMode().height;
	skalaX = resX / 1920;
	skalaY = resY / 1080;

	PrzyciskWPrawo.setScale(skalaX, skalaY);
	PrzyciskWLewo.setScale(skalaX, skalaY);
	PrzyciskZmianyKoloru.setScale(skalaX, skalaY);

    PrzyciskWLewo.setPosition(0, resY - PrzyciskWPrawo.getGlobalBounds().height);
    PrzyciskWPrawo.setPosition(PrzyciskWLewo.getGlobalBounds().width, resY - PrzyciskWLewo.getGlobalBounds().height);
	PrzyciskZmianyKoloru.setPosition(resX - PrzyciskZmianyKoloru.getGlobalBounds().width, resY - PrzyciskZmianyKoloru.getGlobalBounds().height);

    pauza.setTexture(txt_pauza);
    tlo.setTexture(txt_tlo);
    gabeczka.setTexture(txt_gabeczka);

    gabeczka.setPosition(1000000, 500000);
    gabeczka.setScale(skalaX, skalaY);

    marchewka[1].setTexture(txt_marchewka2[0]);

    grunio[0].sprite.setTexture(grunio[0].textura[grunio[0].kolor]);
    grunio[0].sprite.setPosition(grunio[0].x, grunio[0].y);

    grunio[1].sprite.setTexture(grunio[1].textura[grunio[1].kolor]);
    grunio[1].sprite.setPosition(grunio[1].x, grunio[1].y);

    //RenderWindow oknoh(VideoMode(1, 1, 1), "1", Style::None);
    /*switch (menu(resX, resY, fullscreen, skalaY, skalaX, fullscreen, vsync, maxfps, predkoscmarchewki[0], przyspieszanie, maxprmarchewki, tworzyc, ipservera, maxscore, glosnosc, glosnoscmuzyki, glosnoscdzwiekow, tlo))
    {
    case 1:
    {
        exit(0);
        break;
    }
    case 2:
    {
        //MessageBox(NULL, "Nie uda�o si� wczyta� czcionki!\nUpewnij si�, �e plik gabriola.ttf jest w g��wnym folderze gry.", "Blad wczytywania czcionki!", MB_OK | MB_ICONERROR);
        return 3;
        break;
    }
    case 3:
    {
        //oknoh.close();
        break;
    }
    case 4:
    {
        oknoh.close();
        multiplayer = 1;
        //sieciowy = 1;
        //cout << "IP: " << ipservera << endl;
        break;
    }
    case 10:
    {
        oknoh.close();
        return 2;
    }
    }*/










    RenderWindow okno(VideoMode(resX, resY, 32), "");

    int resXt = 0;
    int resYt = 0;
    int resXo = resX;
    int resYo = resY;
    int maxfpst;
    bool fullscreenoriginal = fullscreen;
    bool vsyncoriginal = vsync;
    short maxfpsoriginal = maxfps;
    short iloscrozdzielczosci = 9;
    short iloscmaxfps = 6;
    short wybor_maxfps = 0;
    short wybor_rozdzielczosc = 0;
    ustawrozdzielczosc(wybor_rozdzielczosc, iloscrozdzielczosci, resX, resY, resXt, resYt);
    //ustawmaxfps(wybor_maxfps, iloscmaxfps, maxfps, maxfpst);
	okno.setVerticalSyncEnabled(true);
	okno.setFramerateLimit(60);
    resY = (resX / 16) * 9;
    bool zmieniono = 0;
    /*Sprite logo;
    Texture txt_logo;
    txt_logo.loadFromFile("images/MRalozerca_logo.png");
    logo.setTexture(txt_logo);
    wycentrujsprite(logo, resX, resY, skalaY, skalaX, -350);
    logo.setScale(skalaX / 4, skalaX / 4);*/

    //RenderWindow okno(VideoMode(resX, resY, 32), "Gruniozerca - Menu Glowne");


    Vector2f touchinput[5];

    Vector2i touch[5] = {
        Touch::getPosition(0),
        Touch::getPosition(1),
        Touch::getPosition(2),
        Touch::getPosition(3),
        Touch::getPosition(4)
    };

	Text startbutton;
	Text multiplayerbutton;
	Font font;
	Text optionsbutton;
	Text exitbutton;
	Text opcjen;
	Text bestscore;
	Text logo;

	if (!font.loadFromFile("gabriola.ttf"))
	{
		exit(2);
	}

	if (maxscore > 0)
	{
		bestscore.setFont(font);
		bestscore.setCharacterSize(50 * skalaY);
		bestscore.setStyle(Text::Bold);
		bestscore.setString("Najlepszy wynik: " + to_string(maxscore));
		bestscore.setFillColor(Color::Green);
		if (maxscore > 10000)
		{
			bestscore.setFillColor(Color::Yellow);
		}
		if (maxscore > 100000)
		{
			bestscore.setFillColor(Color(255, 160, 0));
		}
		if (maxscore > 1000000)
		{
			bestscore.setFillColor(Color::Red);
		}
		if (maxscore == 29)
		{
			bestscore.setString("Autor portu: MinikPLayer. Projekt oryginalny: EmuNES");
		}
	}

	opcjen.setFont(font);
	opcjen.setString("OPCJE");
	opcjen.setCharacterSize(100 * skalaY);
	opcjen.setFillColor(Color::Green);
	wycentrujtext(opcjen, resX, resY, skalaY, -400);

	logo.setFont(font);
	logo.setString("MRalozerca");
	logo.setCharacterSize(340 * skalaY);
	logo.setFillColor(Color::Blue);
	wycentrujtext(logo, resX, resY, skalaY, -350);


	startbutton.setFont(font);
	startbutton.setCharacterSize(200 * skalaY);
	startbutton.setStyle(Text::Bold);
	startbutton.setString("JEDEN GRACZ");
	wycentrujtext(startbutton, resX, resY, skalaY, -20);

	exitbutton.setFont(font);
	exitbutton.setCharacterSize(150 * skalaY);
	exitbutton.setStyle(Text::Bold);
	exitbutton.setString("WYJSCIE");
	wycentrujtext(exitbutton, resX, resY, skalaY, 200);

	muzyka.play();

    short lastExitMode = checkExitSaveState();

    while (okno.isOpen())
    {
        if (lastExitMode == 2) break;
        Mouse mouse;
        Event zdarzenie;
        if (okno.pollEvent(zdarzenie))
        {
            if (zdarzenie.type == zdarzenie.Closed) return 1;
            if (zdarzenie.type == zdarzenie.KeyPressed && zdarzenie.key.code == Keyboard::Escape) return 1;
        }
       

        Vector2f mysz(Mouse::getPosition(okno));

        Vector2i touchpos = Touch::getPosition(0);

        mysz.x = touchpos.x;
        mysz.y = touchpos.y;

        if (startbutton.getGlobalBounds().contains(mysz))
        {
            startbutton.setFillColor(Color::Blue);
            if (Touch::isDown(0) || Touch::isDown(1) || Touch::isDown(2))
            {
                break;
            }
        }
        else
            startbutton.setFillColor(Color::White);

		/*for (int i = 0; i < 5; i++)
		{
			if (Touch::isDown(i))
			{
				break;
			}
		}*/
		/*if (Touch::isDown(0)) break;
		if (Touch::isDown(1)) break;
		if (Touch::isDown(2)) break;
		if (Touch::isDown(3)) break;
		if (Touch::isDown(4)) break;*/

        
        if (exitbutton.getGlobalBounds().contains(mysz))
        {
            exitbutton.setFillColor(Color::Blue);
            if (Touch::isDown(0) || Touch::isDown(1) || Touch::isDown(2))
            {
				okno.close();
                exit(1);
				return 1;
            }
        }
        else
            exitbutton.setFillColor(Color::White);

        okno.clear();
        okno.draw(tlo);
        okno.draw(logo);
        okno.draw(startbutton);
        okno.draw(multiplayerbutton);
        okno.draw(optionsbutton);
        okno.draw(exitbutton);
        okno.draw(bestscore);
        okno.display();
    }











    skalaX = resX / 1920.0;
    skalaY = resY / 1080.0;
    bool pause = 0;	// Okre�la czy gra ma by� zatrzymana czy ma dzia�a�
    bool zapauzowano = 0;	// Okre�la czy zapauzowano gr�
    short mchkolor[2] = { 0,0 };	// Okre�la kolor marchewki	// Okre�la kolor grunia
    bool stworzona[2] = { 0,0 };		//czy marchewka jest ju� stworzona?
    short grsizeX = 169;		//Szeroko�� tekstury grunia w px
    short grsizeY = 111;		//Wysoko�� tekstury grunia w px
    short marchsizeX = 74;		//Szeroko�� tekstury marchewki w px
    short marchsizeY = 54;		//Wysoko�� tekstury marchewki w px
    float predkoscgr = 1;		//Pr�dko�� grunia ( domyslnie 1 )
    Clock zegar;	// Zegar g��wny aplikacji
    Time czas;








    View widok;
    widok.setSize(resX, resY);
    widok.setCenter(resX / 2, resY / 2);

    if (!font.loadFromFile("gabriola.ttf"))
    {
        //MessageBox(NULL, "Nie uda�o si� wczyta� czcionki!\nUpewnij si�, �e plik gabriola.ttf jest w g��wnym folderze gry.", "Blad wczytywania czcionki!", MB_OK | MB_ICONERROR);
        return 3;
    }





	/*
	//DEBUG
	Text startbutton;
	startbutton.setFont(font);
	startbutton.setCharacterSize(100 * skalaY);
	startbutton.setStyle(Text::Bold);
	startbutton.setString("JEDEN GRACZ");
	wycentrujtext(startbutton, resX, resY, skalaY, -20);
	//END DEBUG
	*/






    //RenderWindow okno(VideoMode(resX, resY, 32), "Gruniozerca");
    if (fullscreen == 1)
    {
        okno.create(VideoMode(resX, resY, 32), "Gruniozerca", Style::Fullscreen);
    }

    Text oczekiwanienadrugiegogracza;
    oczekiwanienadrugiegogracza.setString("Oczekiwanie na drugiego gracza...");
    oczekiwanienadrugiegogracza.setFont(font);
    oczekiwanienadrugiegogracza.setCharacterSize(60 * skalaY);
    oczekiwanienadrugiegogracza.setStyle(Text::Bold);
    oczekiwanienadrugiegogracza.setFillColor(Color::Red);

    Text text_punktynadgr;
    text_punktynadgr.setFont(font);
    text_punktynadgr.setCharacterSize(50 * skalaY);
    text_punktynadgr.setStyle(Text::Bold);

    Text text_gameover;
    text_gameover.setFont(font);
    text_gameover.setCharacterSize(150 * skalaY);
    text_gameover.setStyle(Text::Bold);
    text_gameover.setString("GAME OVER!");
    text_gameover.setFillColor(Color(255, 0, 0, 0));
    wycentrujtext(text_gameover, resX, resY, skalaY, 0);

    Text text_zycia[2];
    FloatRect rect_textzycia[2] = { text_zycia[0].getGlobalBounds() };
    text_zycia[0].setFont(font);
    text_zycia[0].setCharacterSize(50 * skalaY);
    text_zycia[0].setStyle(Text::Bold);
    text_zycia[0].setOrigin(rect_textzycia[0].left + rect_textzycia[0].width, rect_textzycia[0].top + rect_textzycia[0].height);
    string string_text_zycia[2] = { to_string(grunio[0].zycia) };
	int znaki[1];
	znaki[0] = string_text_zycia[0].size();
    text_zycia[0].setPosition(resX - 220 - (znaki[0] * (50 * skalaY)), 0);

    if (multiplayer == 1)
    {
        rect_textzycia[1] = text_zycia[1].getGlobalBounds();
        text_zycia[1].setFont(font);
        text_zycia[1].setCharacterSize(40 * skalaY);
        text_zycia[1].setStyle(Text::Bold);
        text_zycia[1].setOrigin(rect_textzycia[1].left + rect_textzycia[1].width, rect_textzycia[1].top + rect_textzycia[1].height);
        string_text_zycia[1] = to_string(grunio[1].zycia);
        znaki[1] = string_text_zycia[1].size();
        //text_zycia[0].setPosition(resX - 220 - (znaki[0] * (50 * skalaY)), 0);
    }

    Text przycisk_pauza_wyjscie;
    Text przycisk_pauza_restart;
    Text przycisk_pauza_wznow;
    Text wynik[2];

    wynik[0].setCharacterSize(50 * skalaY);
    wynik[0].setFont(font);
    wynik[0].setPosition(0, 0);
    wynik[0].setStyle(Text::Italic);
    wynik[0].setStyle(Text::Bold);

    wynik[1].setCharacterSize(40 * skalaY);
    wynik[1].setFont(font);
    wynik[1].setPosition(0, wynik[0].getGlobalBounds().height + resX / 30);
    wynik[1].setStyle(Text::Italic);
    wynik[1].setStyle(Text::Bold);

    string string_wynik[2];

    grunio[0].sprite.setOrigin(grsizeX / 2, grsizeY / 2);
    grunio[1].sprite.setOrigin(grsizeX / 2, grsizeY / 2);
    float marchX[2] = { 0,0 };
    float marchY[2] = { 0,0 };

    for (int i = 0; i < 5; i++)
    {
        zycie[0][i].setTexture(txt_zycie);
        zycie[0][i].setPosition(resX - ((105 + 5)*(i + 1)*skalaX), 0);
        zycie[0][i].setScale(skalaX / 1.5, skalaY / 1.5);
        zycie[1][i].setTexture(txt_zycie);
        zycie[1][i].setPosition(resX - ((105 + 5)*(i + 1)*skalaX), zycie[0][i].getGlobalBounds().height + resX / 30 - zycie[1][i].getGlobalBounds().height / 2);
        zycie[1][i].setScale(skalaX / 1.7, skalaY / 1.7);
    }


    string string_pauza_wznow = "Wznow";
    string string_pauza_restart = "Restart";
    string string_pauza_wyjscie = "Wyjscie";

    przycisk_pauza_wyjscie.setString(string_pauza_wyjscie);
    przycisk_pauza_restart.setString(string_pauza_restart);
    przycisk_pauza_wznow.setString(string_pauza_wznow);

    przycisk_pauza_wyjscie.setFont(font);
    przycisk_pauza_restart.setFont(font);
    przycisk_pauza_wznow.setFont(font);

    przycisk_pauza_wyjscie.setCharacterSize(150 * skalaY); // Ustalanie
    przycisk_pauza_restart.setCharacterSize(150 * skalaY); // rozmiaru czcionki
    przycisk_pauza_wznow.setCharacterSize(150 * skalaY); // napis�w w menu opcji

    wycentrujtext(przycisk_pauza_wyjscie, resX, resY, skalaY, 300);
    wycentrujtext(przycisk_pauza_restart, resX, resY, skalaY, 0);
    wycentrujtext(przycisk_pauza_wznow, resX, resY, skalaY, -300);

    przycisk_pauza_wyjscie.setScale(skalaX, skalaY);
    przycisk_pauza_restart.setScale(skalaX, skalaY);
    przycisk_pauza_wznow.setScale(skalaX, skalaY);

    przycisk_pauza_wyjscie.setFillColor(Color::Yellow);
    przycisk_pauza_restart.setFillColor(Color::Yellow);
    przycisk_pauza_wznow.setFillColor(Color::Yellow);

    tlo.setOrigin(0, 1080);
    tlo.setPosition(0, resY);
    marchewka[0].setOrigin(marchsizeX / 2, marchsizeY / 2);
    marchewka[1].setOrigin(marchsizeX / 2, marchsizeY / 2);
    //plik(resX, resY, fullscreen, maxfps, vsync, 0, 1, predkoscmarchewki[0], przyspieszanie, maxprmarchewki, tworzyc, glosnosc, glosnoscmuzyki, glosnoscdzwiekow);
    grunio[0].sprite.setScale(skalaX, skalaX);
    grunio[1].sprite.setScale(skalaX, skalaY);
    marchewka[0].setScale(skalaX, skalaX);
    marchewka[1].setScale(skalaX, skalaX);
    wycentrujsprite(pauza, resX, resY, skalaY, skalaX, 0);
    pauza.setScale(skalaX, skalaX);
    tlo.setScale(skalaX, skalaX);
    okno.setVerticalSyncEnabled(vsync);
    okno.setFramerateLimit(maxfps);
    przyspieszanie = przyspieszanie * skalaY;
    grunio[0].y = grunio[0].y + (220 - (220 * skalaY));
    grunio[1].y = grunio[1].y + (220 - (220 * skalaY));

    marchewka[0].setTexture(txt_marchewka[0]);
    grunio[0].sprite.setTexture(grunio[0].textura[0]);

    short tickratetime = 50;

    if (sieciowy == 1)
    {
        TcpSocket tcpsocket;
        if (tcpsocket.connect(ipservera, porttcp, sf::Time(seconds(5))) != Socket::Done)
        {
            cout << "Nie mozna polaczyc sie z serverem!" << endl;
            exit(10);
        }
        else
        {
            polaczono = 1;
            Packet packet;
            tcpsocket.receive(packet);
            packet >> portudpupload >> portudpdownload >> userid >> tickratetime;
            cout << "PortUDPUpload: " << portudpupload << " PortuUDPDownl: " << portudpdownload << endl;
        }
    }
    //socket.setBlocking(false);
    socket.bind(portudpdownload, ipservera);


	okno.setVerticalSyncEnabled(true);
	okno.setFramerateLimit(60);

	//resX = 800;
	//resY = 480;
	//skalaX = resX / 1920;
	//skalaY = resY / 1080;

	const float czasTrwaniaIntra = 2;

	Texture introTexture;

	Clock introClock;
	Time introTimer;

	introTexture.loadFromFile("images/introImage.png");

	Sprite introSprite;
	const Color insideKolorTablicy = Color(31, 64, 53);
	Image img_tablicy;
	img_tablicy.create(1920, 1080);
	for (int i = 0; i < 1920; i++)
	{
		for (int j = 0; j < 1080; j++)
		{
			img_tablicy.setPixel(i, j, insideKolorTablicy);
		}
	}
	Texture txt_tablicy;
	txt_tablicy.loadFromImage(img_tablicy);

	Sprite tablicaTlo;

	tablicaTlo.setTexture(txt_tablicy);


	introClock.restart();
	introTimer = introClock.getElapsedTime();

	introSprite.setTexture(introTexture);
	introSprite.setOrigin(introSprite.getGlobalBounds().width / 2.0, introSprite.getGlobalBounds().height / 2.0);

	introClock.restart();

	float czasnawykonanie = 1.5;

    if (lastExitMode != 2)
    {
        while (introTimer.asSeconds() <= (czasTrwaniaIntra * czasnawykonanie) / 2.0)
        {
            okno.clear();
            Event zdarzenie;
            if (okno.pollEvent(zdarzenie))
            {
                obslugazdarzen(zdarzenie, &okno);
            }
            wyswietlIntro(introSprite, introTimer.asSeconds() / (czasTrwaniaIntra * czasnawykonanie), &okno, tablicaTlo, widok);
            okno.display();
            introTimer = introClock.getElapsedTime();
        }
    }
    else
    {
        while (introTimer.asSeconds() <= 0.001)
        {
            okno.clear();
            Event zdarzenie;
            if (okno.pollEvent(zdarzenie))
            {
                obslugazdarzen(zdarzenie, &okno);
            }
            wyswietlIntro(introSprite, introTimer.asSeconds() / (czasTrwaniaIntra * czasnawykonanie), &okno, tablicaTlo, widok);
            okno.display();
            introTimer = introClock.getElapsedTime();
        }
        introTimer = sf::seconds((czasTrwaniaIntra * czasnawykonanie) / 2.0);
        zapauzowano = 1;
    }


    

	zegar.restart();

	grunio[0].sprite.setTextureRect(IntRect(grsizeX, 0, grsizeX, grsizeY));
	grunio[0].zycia = 3;


    if (lastExitMode == 2) loadActualSaveState(grunio[0].x, grunio[0].punkty, grunio[0].zycia, marchX[0], marchY[0], grunio[0].kolor, mchkolor[0], stworzona[0]);

    grunio[0].sprite.setTexture(grunio[0].textura[grunio[0].kolor]);
    marchewka[0].setTexture(txt_marchewka[mchkolor[0]]);
	
	bool active = true;

    while (okno.isOpen())
    {
        //widok = okno.getView();
        for (int i = 0; i < 5; i++)
        {
            touch[i] = Touch::getPosition(i);
            touchinput[i].x = touch[i].x;
            touchinput[i].y = touch[i].y;
        }
		if (introTimer.asSeconds() >= czasTrwaniaIntra * czasnawykonanie || lastExitMode == 2)
		{
			czas = zegar.getElapsedTime();
			if (zapauzowano == 0)
			{
				animtime = animrate.getElapsedTime();
				if (animtime.asMilliseconds() >= animframetime)
				{
					grunio[0].sprite.setTextureRect(IntRect(grsizeX, 0, grsizeX, grsizeY));
					if (multiplayer == 1)
					{
						grunio[1].sprite.setTextureRect(IntRect(grsizeX, 0, grsizeX * 2, grsizeY));
					}
				}
				if (animtime.asMilliseconds() >= animframetime * 2)
				{
					grunio[0].sprite.setTextureRect(IntRect(0, 0, grsizeX, grsizeY));
					if (multiplayer == 1)
					{
						grunio[1].sprite.setTextureRect(IntRect(0, 0, grsizeX, grsizeY));
					}
					animrate.restart();
				}

				FishAnimTime = FishAnimClock.getElapsedTime();
				if (FishAnimTime.asMilliseconds() >= FishAnimationTime)
				{
					FishAnimClock.restart();
				}
				marchewka[0].setRotation(-FishAnimTime.asMilliseconds() / (FishAnimationTime / 360));
			}
		}
		else
		{
			introTimer = introClock.getElapsedTime();
			zegar.restart();
		}
        Vector2f mysz(Mouse::getPosition(okno));
        Event zdarzenie;
        if (okno.pollEvent(zdarzenie) && active)
        {
			//okno.waitEvent(zdarzenie);
			if (zdarzenie.type == sf::Event::LostFocus)
			{
				zapauzowano = 1;
				pause = 1;
				muzyka.pause();
				//okno.setActive(false);
				//active = false;
                SaveActualState(grunio[0].x, grunio[0].punkty, grunio[0].zycia, marchX[0], marchY[0], grunio[0].kolor, mchkolor[0], stworzona[0]);
                okno.close();
                exit(1);
                return 1;
				break;
			}
			if (zdarzenie.type == sf::Event::GainedFocus)
			{
				okno.setActive(true);
				active = true;
				break;
			}
            if (zdarzenie.type == zdarzenie.Closed) okno.close();
            if (zdarzenie.type == zdarzenie.KeyPressed && zdarzenie.key.code == Keyboard::Escape)
            {
                if (zapauzowano == 0)
                {
                    zapauzowano = 1;
                    muzyka.pause();
                }
                else
                {
                    zapauzowano = 0;
                    muzyka.play();
                }

            }
            if (pause == 0)
            {
				short iloscwcisnieczmianykoloru = 5;
                if (multiplayer == 0)
                {
					for (int i = 0; i < 5; i++)
					{
							if (PrzyciskZmianyKoloru.getGlobalBounds().contains(touchinput[i]))
							{
								if (WcisnietyPrzyciskZmianyKoloru == 0)
								{
									WcisnietyPrzyciskZmianyKoloru = 1;
									zmienkolor(grunio[0].sprite, grunio[0].textura, grunio[0].kolor, mchkolor[0], 1);
									break;
								}
							}
							else
								iloscwcisnieczmianykoloru--;
					}
					if (iloscwcisnieczmianykoloru == 0) WcisnietyPrzyciskZmianyKoloru = 0;
					
                    if (zdarzenie.type == zdarzenie.KeyPressed && zdarzenie.key.code == Keyboard::E)
                    {
                        zmienkolor(grunio[0].sprite, grunio[0].textura, grunio[0].kolor, mchkolor[0], 1);
                    }
                    if (zdarzenie.type == zdarzenie.KeyPressed && zdarzenie.key.code == Keyboard::R)
                    {
                        zmienkolor(grunio[0].sprite, grunio[0].textura, grunio[0].kolor, mchkolor[0], 0);
                    }
                    if (zdarzenie.type == zdarzenie.KeyPressed && zdarzenie.key.code == Keyboard::P)
                    {
                        klatka = 50;
                    }
                }
                else
                {
                    if (zdarzenie.type == zdarzenie.KeyPressed && zdarzenie.key.code == Keyboard::Period)
                    {
                        zmienkolor(grunio[0].sprite, grunio[0].textura, grunio[0].kolor, mchkolor[0], 1);
                    }
                    if (zdarzenie.type == zdarzenie.KeyPressed && zdarzenie.key.code == Keyboard::Slash)
                    {
                        zmienkolor(grunio[0].sprite, grunio[0].textura, grunio[0].kolor, mchkolor[0], 0);
                    }
                    if (zdarzenie.type == zdarzenie.KeyPressed && zdarzenie.key.code == Keyboard::R)
                    {
                        zmienkolor(grunio[1].sprite, grunio[1].textura, grunio[1].kolor, mchkolor[1], 1);
                    }
                    if (zdarzenie.type == zdarzenie.KeyPressed && zdarzenie.key.code == Keyboard::T)
                    {
                        zmienkolor(grunio[1].sprite, grunio[1].textura, grunio[1].kolor, mchkolor[1], 0);
                    }
                }

            }
        }
		if (introTimer.asSeconds() >= czasTrwaniaIntra * czasnawykonanie || lastExitMode == 2)
		{
			string_text_zycia[0] = to_string(grunio[0].zycia);
			string_text_zycia[1] = to_string(grunio[1].zycia);
			znaki[0] = string_text_zycia[0].size();
			znaki[1] = string_text_zycia[1].size();
			text_zycia[0].setPosition(resX - 110 - ((znaki[0] + 1)*(29 * skalaY)), 0);
			text_zycia[1].setPosition(resX - 110 - ((znaki[1] + 1)*(29 * skalaY)), text_zycia[0].getGlobalBounds().height + resX / 30 - text_zycia[1].getGlobalBounds().height / 2);
			wRuchu = 0;
			if (pause == 0)
			{
				for (int i = 0; i < 5; i++)
				{
					if (PrzyciskWPrawo.getGlobalBounds().contains(touchinput[i]))
					{
						grunio[0].sprite.setScale(skalaX, skalaY);
						grunio[0].x = grunio[0].x + (predkoscgr*czas.asMilliseconds());
						wRuchu = 1;
					}
					if (PrzyciskWLewo.getGlobalBounds().contains(touchinput[i]))
					{
						grunio[0].sprite.setScale(-skalaX, skalaY);
						grunio[0].x = grunio[0].x - (predkoscgr*czas.asMilliseconds());
						wRuchu = 1;
					}
				}
				if (multiplayer == 1)
				{
					if (Keyboard::isKeyPressed(Keyboard::A))
					{
						grunio[1].sprite.setScale(-skalaX, skalaY);
						grunio[1].x -= predkoscgr * czas.asMilliseconds();
					}
					if (Keyboard::isKeyPressed(Keyboard::D))
					{
						grunio[1].sprite.setScale(skalaX, skalaY);
						grunio[1].x += predkoscgr * czas.asMilliseconds();
					}
				}
			}
			if (wRuchu)
			{
				animframetime = animframetimemove;
			}
			else
			{
				animframetime = animframetimestay;
			}
			zegar.restart().asSeconds();
			if (grunio[0].x < 0 + (grsizeX / 2)*skalaX) grunio[0].x = 0 + (grsizeX / 2)*skalaX;
			if (grunio[0].x > resX - (grsizeX / 2 * skalaX)) grunio[0].x = resX - (grsizeX / 2 * skalaX);
			if (grunio[1].x < 0 + (grsizeX / 2)*skalaX) grunio[1].x = 0 + (grsizeX / 2)*skalaX;
			if (grunio[1].x > resX - (grsizeX / 2 * skalaX)) grunio[1].x = resX - (grsizeX / 2 * skalaX);
			if (pause == 0)
			{
				stworzmarchewke(marchewka[0], skalaX, skalaY, txt_marchewka, grunio[0].x, stworzona[0], resX, marchX[0], marchY[0], predkoscmarchewki[0], mchkolor[0]);
				if (sprawdzczyzjedzona(marchewka[0], grunio[0].sprite, stworzona[0], marchY[0], predkoscmarchewki[0], przyspieszanie, maxprmarchewki, grunio[0].kolor, mchkolor[0], grunio[0].punkty, ciag[0]))
				{
					zjedzona[0] = 1;
					dzwiek_zebraniemarchewki.play();
					grunio[0].xt = grunio[0].x;
				}
				if (sprawdzmarchewke(marchewka[0], skalaY, stworzona[0], marchY[0], resY, marchsizeY, predkoscmarchewki[0], przyspieszanie, maxprmarchewki, mchkolor[0], grunio[0].kolor, ciag[0], ktoram, marchewkawziemi[ktoram], txt_marchewkawziemi, grunio[0].zycia))
				{
					dzwiek_marchewkauciekla.play();
					klatka = 60;
				}
				ruszmarchewke(marchewka[0], skalaX, skalaY, stworzona, marchX[0], marchY[0], predkoscmarchewki[0], zegar, czas);
				if (zjedzona[0] == 1)
				{
					wyswietlzdobytepunkty(ciag[0], text_punktynadgr, klatkaanimacji_punktynadgr, maxfps, zjedzona[0], grunio[0].xt, grunio[0].y, skalaX, skalaY);
				}
				sprawdzciag(ciag[0], grunio[0].zycia);
				if (sprawdzczyzginales(grunio[0].zycia, stworzona[0], grunio[0].sprite, muzyka, klatkaanimacji_smierc, maxfps, grunio[0].y, grunio[0].x, grunio[0].xt, text_gameover, klatkaanimacji_gameover, resY, zegaranimacjismierci[0], czasanimacjismierci[0], czasanimacjismiercisuma, deathtimer, deathtime, gabeczka, resX) == 1)
				{
					if (grunio[0].punkty > maxscore)
					{
						ofstream plikmscore;
						plikmscore.open("images/wynik.png");
						plikmscore << grunio[0].punkty;
						plikmscore.close();
					}
					grunio[0].x = resX / 2;
					grunio[0].sprite.setRotation(0);
					grunio[0].y = resY - 220;
					grunio[0].y = grunio[0].y + (220 - (220 * skalaY));
					grunio[0].punkty = 0;
					ciag[0] = 0;
					grunio[0].zycia = 3;
					stworzona[0] = 0;
					pause = 0;
					muzyka.play();
					klatkaanimacji_gameover = 0;
					klatkaanimacji_smierc = 0;
					klatkaanimacji_punktynadgr = 0;
					predkoscmarchewki[0] = startpredkoscmarchewki;
				}
				if (multiplayer == 1)
				{
					stworzmarchewke(marchewka[1], skalaX, skalaY, txt_marchewka2, grunio[1].x, stworzona[1], resX, marchX[1], marchY[1], predkoscmarchewki[1], mchkolor[1]);
					if (sprawdzczyzjedzona(marchewka[1], grunio[1].sprite, stworzona[1], marchY[1], predkoscmarchewki[1], przyspieszanie, maxprmarchewki, grunio[1].kolor, mchkolor[1], grunio[1].punkty, ciag[1]))
					{
						zjedzona[1] = 1;
						dzwiek_zebraniemarchewki.play();
						grunio[1].xt = grunio[1].x;
					}
					if (sprawdzmarchewke(marchewka[1], skalaY, stworzona[1], marchY[1], resY, marchsizeY, predkoscmarchewki[1], przyspieszanie, maxprmarchewki, mchkolor[1], grunio[1].kolor, ciag[1], ktoram, marchewkawziemi[ktoram], txt_marchewkawziemi, grunio[1].zycia))
					{
						dzwiek_marchewkauciekla.play();
						klatka = 60;
					}
					ruszmarchewke(marchewka[1], skalaX, skalaY, stworzona, marchX[1], marchY[1], predkoscmarchewki[1], zegar, czas);
					if (zjedzona[1] == 1)
					{
						wyswietlzdobytepunkty(ciag[1], text_punktynadgr, klatkaanimacji_punktynadgr, maxfps, zjedzona[1], grunio[1].xt, grunio[1].y, skalaX, skalaY);
					}
					sprawdzciag(ciag[1], grunio[1].zycia);
					if (sprawdzczyzginales(grunio[1].zycia, stworzona[1], grunio[1].sprite, muzyka, klatkaanimacji_smierc, maxfps, grunio[1].y, grunio[1].x, grunio[1].xt, text_gameover, klatkaanimacji_gameover, resY, zegaranimacjismierci[0], czasanimacjismierci[0], czasanimacjismiercisuma, deathtimer, deathtime, gabeczka, resX) == 1)
					{
						grunio[1].x = resX / 2;
						grunio[1].sprite.setRotation(0);
						grunio[1].y = resY - 220;
						grunio[1].y = grunio[1].y + (220 - (220 * skalaY));
						grunio[1].punkty = 0;
						ciag[1] = 0;
						grunio[1].zycia = 3;
						stworzona[1] = 0;
						pause = 0;
						muzyka.play();
						klatkaanimacji_gameover = 0;
						klatkaanimacji_smierc = 0;
						klatkaanimacji_punktynadgr = 0;
						predkoscmarchewki[1] = startpredkoscmarchewki;
					}
				}
				pokazzycia(zycie[0], grunio[0].zycia, txt_zycie, txt_pusty, text_zycia[0]);
				if (multiplayer == 1)
				{
					pokazzycia(zycie[1], grunio[1].zycia, txt_zycie, txt_pusty, text_zycia[1]);
				}
			}
		}
		else
		{
			pokazzycia(zycie[0], grunio[0].zycia, txt_zycie, txt_pusty, text_zycia[0]);
		}


        //grunio[0].sprite.setPosition(grunio[0].x, grunio[0].y);
        //grunio[1].sprite.setPosition(grunio[1].x, grunio[1].y);
        wyswietlwynik(grunio[0].punkty, wynik[0], string_wynik[0]);
        wyswietlwynik(grunio[1].punkty, wynik[1], string_wynik[1]);
        if (zapauzowano == 1)
        {
            odpauzowac = przyciskipauzy(przycisk_pauza_wznow, przycisk_pauza_restart, przycisk_pauza_wyjscie, mysz, sieciowy);
            if (odpauzowac == 1)
            {
                zapauzowano = 0;
                muzyka.play();
                odpauzowac = 0;
            }
            if (odpauzowac == 2)
            {
                grunio[0].x = resX / 2;
                grunio[0].punkty = 0;
                ciag[0] = 0;
                grunio[0].zycia = 3;
                stworzona[0] = 0;
                muzyka.play();
                odpauzowac = 0;
                zapauzowano = 0;

                grunio[1].x = resX / 2;
                grunio[1].punkty = 0;
                ciag[1] = 0;
                grunio[1].zycia = 3;
                stworzona[1] = 0;
                predkoscmarchewki[0] = startpredkoscmarchewki;
                predkoscmarchewki[1] = startpredkoscmarchewki;
            }
            if (odpauzowac == 3)
            {
				okno.close();
                clearExitSaveFile();
				exit(1);
				return 1;
            }

        }
        if (sieciowy == 0)
        {
            if (zapauzowano == 1)
            {
                pause = 1;
            }
            else
                pause = 0;
        }
        if (klatka > 0)
        {
            if (klatka % 5 == 1)
            {
                if (klatka % 2 == 0)
                {
                    widok.move(0, 10);
                }
                else
                    widok.move(0, -10);
            }
            klatka--;
        }
        else
            widok.setCenter(resX / 2, resY / 2);

        //ticktime = tickrate.getElapsedTime();

        /*if ((sieciowy == 1) && (ticktime.asMilliseconds() > tickratetime ))
        {
        Packet packet;
        packet << grunio[0].x;
        socket.send(packet, ipservera, portudpupload);
        packet.clear();
        socket.receive(packet, ipservera, portudpdownload);
        if (userid == 0)
        {
        packet >> przeciwnikjestwgrze >> czyjestwgrze >> grunio[0].x >> grunio[1].x >> pause >> marchX[0] >> marchX[1] >> marchY[0] >> marchY[1] >> mchkolor[0] >> mchkolor[1] >> grunio[0].y >> punkty[0] >> punkty[1];
        }
        else
        packet >> przeciwnikjestwgrze >> czyjestwgrze >> grunio[1].x >> grunio[0].x >> pause >> marchX[1] >> marchX[0] >> marchY[1] >> marchY[0] >> mchkolor[1] >> mchkolor[0] >> grunio[0].y >> punkty[1] >> punkty[0];



        tickrate.restart();
        if (przeciwnikjestwgrze == 1)
        {
        przeciwnikbylwgrze = 1;
        }
        }*/






		//DEBUG --------------------------------------------------------------------------------------------------------------------------------
		//startbutton.setString("RX: " + to_string(marchX[0]) + " RY: " + to_string(marchY[0]));
		//END DEBUG ----------------------------------------------------------------------------------------------------------------------------







        grunio[0].sprite.setPosition(grunio[0].x, grunio[0].y);
		grunio[1].sprite.setPosition(10000, 10000);
        marchewka[0].setPosition(marchX[0], marchY[0]);
        marchewka[1].setPosition(marchX[1], marchY[1]);
        //cout << "Gr0: " << grunio[0].x << "," << grunio[0].y << " Gr1: " << grunio[1].x << "," << grunio[1].y << endl;
		if (active)
		{
			okno.clear(Color(0, 0, 0));
			okno.draw(tlo);
			if (multiplayer == 1)
			{
				okno.draw(grunio[1].sprite);
				okno.draw(marchewka[1]);
			}
			okno.draw(grunio[0].sprite);
			okno.draw(marchewka[0]);
			okno.draw(gabeczka);
			okno.draw(text_punktynadgr);
			okno.draw(text_zycia[0]);
			if (multiplayer == 1) okno.draw(text_zycia[1]);
			okno.draw(text_gameover);
			for (int i = 0; i < 5; i++)
			{
				okno.draw(zycie[0][i]);
				if (multiplayer == 1) okno.draw(zycie[1][i]);
			}
			okno.draw(wynik[0]);
			if (multiplayer == 1) okno.draw(wynik[1]);
			for (int i = 0; i <= ktoram; i++)
			{
				okno.draw(marchewkawziemi[i]);
			}
			okno.draw(PrzyciskWPrawo);
			okno.draw(PrzyciskWLewo);
			okno.draw(PrzyciskZmianyKoloru);
			if (zapauzowano == 1)
			{
				okno.draw(pauza);
				okno.draw(przycisk_pauza_wznow);
				if (sieciowy == 0)
				{
					okno.draw(przycisk_pauza_restart);
				}
				okno.draw(przycisk_pauza_wyjscie);
			}
			if (introTimer.asSeconds() <= czasTrwaniaIntra * czasnawykonanie && lastExitMode != 2) wyswietlIntro(introSprite, introTimer.asSeconds() / (czasTrwaniaIntra * czasnawykonanie), &okno, tablicaTlo, widok);
			okno.setView(widok);


			//DEBUG --------------------------------------------------------------------------------------------------------------------------------
			//okno.draw(startbutton);
			//END DEBUG ----------------------------------------------------------------------------------------------------------------------------

			okno.display();
		}
		else {
			sf::sleep(sf::milliseconds(100));
		}
    }
    return 0;

}
