#include "Kontrol.h"

#define SONSUZ 9999

Kontrol::Kontrol()
{
	uyariCalisti = false;
	geriDonusBasla = false;
	raporVerildi = false;

	TMNT = new Degerler();

	MAX_PIZZA = Say("PizzaBilgileri.txt");

	pizzalar = new Pizza *[MAX_PIZZA];

	MAX_DUSMAN = Say("DusmanBilgisi.txt");

	dusmanlar = new Dusman *[MAX_DUSMAN];

	dusmanlar = DusmanOku(dusmanlar, MAX_DUSMAN);

	MAX_DUGUM = Say("KanalBaglantiBilgileri.txt");

	dugumler = new int *[MAX_DUGUM];
	yeraltiDugumler = new Dugum[MAX_DUGUM];
	TMNT->gidilenYollar = new int[MAX_DUGUM];
	TMNT->dugumKarsilasilan = new int[MAX_DUGUM];

	for (int i = 0; i < MAX_DUGUM; i++)
	{
		TMNT->gidilenYollar[i] = -1;
		TMNT->dugumKarsilasilan[i] = -1;
		dugumler[i] = new int[MAX_DUGUM];
		yeraltiDugumler[i].ID = "N" + to_string(i);
	}

	GrafOlustur(dugumler, MAX_DUGUM);

	PizzaOku(pizzalar, MAX_PIZZA);

	MenuGetir(pizzalar, MAX_PIZZA);
}

void Kontrol::MenuGetir(Pizza **pizza, int max_pizza)
{
	PizzaOlustur(pizza, max_pizza);

	KnapSack(2500, yeraltiDugumler, pizzaSayisi);

	int secilenSayi = 0;

	for (int i = 0; i < pizzaSayisi; i++)
	{
		if (itemDizi2[i] != -1)
		{
			secilenSayi++;
		}
	}

	int *optimalYol = new int[MAX_DUGUM];

	for (int i = 0; i < MAX_DUGUM; i++)
	{
		optimalYol[i] = -1;
	}

	optimalYol = HaritaCiz(dugumler, secilenSayi, doluDugumIDList);

	yeraltiDugumler = DusmanEkle(yeraltiDugumler, dusmanlar);

	HareketBaslat(optimalYol, yeraltiDugumler, dugumler);
}

void Kontrol::HareketBaslat(int *optimalYol, Dugum *yeralti, int **kanalBilgileri)
{
	int adim = 0;

	while (!geriDonusBasla)
	{
		if (optimalYol[adim + 1] == -1)
		{
			geriDonusBasla = true;
			break;
		}

		EnerjiMiktariHesapla(kanalBilgileri[optimalYol[adim]][optimalYol[adim + 1]], optimalYol[adim + 1], yeralti, adim);

		adim++;

		if (raporVerildi)
			return;
	}

	int indeksAdim = adim;

	if (geriDonusBasla)
	{
		for (int i = adim; i > 0; i--)
		{
			if (optimalYol[adim - 1] == -1)
			{
				break;
			}

			EnerjiMiktariHesapla(kanalBilgileri[optimalYol[adim]][optimalYol[adim - 1]], optimalYol[adim - 1], yeralti, indeksAdim);

			indeksAdim++;
			adim--;

			if (raporVerildi)
				return;
		}
	}

	if (raporVerildi == false)
	{
		raporVer(0);
	}
}

void Kontrol::EnerjiMiktariHesapla(int yol, int konum, Dugum *yeralti, int yolIndeks)
{
	TMNT->gidilenYolMetre += yol;

	float gecenZaman = yol / TMNT->splinterHiz;

	TMNT->ninjaKalori -= (50.0f / 3600.0f) * gecenZaman;

	if (TMNT->splinterHiz == 2)
	{
		TMNT->splinterKalori -= (gecenZaman * 0.4f);
		TMNT->harcadigiKalori += (gecenZaman * 0.4f);
	}
	else
	{
		TMNT->splinterKalori -= (gecenZaman * 0.2f);
		TMNT->harcadigiKalori += (gecenZaman * 0.2f);
	}

	TMNT->mevcutKonum = konum;
	TMNT->gidilenYollar[yolIndeks] = konum;

	if (yeralti[konum].pizza != NULL)
	{
		for (int i = 0; i < pizzaSayisi; i++)
		{
			if (TMNT->toplananPizalar[i] == -1)
			{
				if (TMNT->splinterCanta + yeralti[konum].pizza->hacim > 2500)
				{
					break;
				}

				TMNT->splinterCantaKalori += yeralti[konum].pizza->kalori;
				TMNT->toplananKalori += yeralti[konum].pizza->kalori;
				TMNT->splinterCanta += yeralti[konum].pizza->hacim;
				TMNT->toplananPizalar[i] = stoi(yeralti[konum].pizza->ID.substr(1));
				TMNT->dugumKarsilasilan[konum] = 1;

				delete yeralti[konum].pizza;

				yeralti[konum].pizza = NULL;

				break;
			}
		}
	}

	if (yeralti[konum].dusman != NULL)
	{
		TMNT->splinterKalori -= yeralti[konum].dusman->kalori;
		TMNT->harcadigiKalori += yeralti[konum].dusman->kalori;

		TMNT->ninjaKalori -= (50.0f / 3600.0f) * (yeralti[konum].dusman->dakika * 60);

		for (int i = 0; i < dusmanSayisi; i++)
		{
			if (TMNT->karsilasilanDusmanlar[i] == -1)
			{
				TMNT->karsilasilanDusmanlar[i] = stoi(yeralti[konum].dusman->ID.substr(1));
				if (TMNT->dugumKarsilasilan[konum] == 1)
				{
					TMNT->dugumKarsilasilan[konum] = 3;
				}
				else
				{
					TMNT->dugumKarsilasilan[konum] = 2;
				}
				break;
			}
		}
		delete yeralti[konum].dusman;

		yeralti[konum].dusman = NULL;
	}

	if (TMNT->splinterKalori < 201 || TMNT->ninjaKalori < 21)
	{
		int sonuc = Uyari();

		if (TMNT->splinterKalori <= 0 || TMNT->ninjaKalori <= 0)
		{
			if (TMNT->splinterKalori <= 0)
			{
				cout << "Splinter Ustanin enerjisi bitti" << endl;
			}

			if (TMNT->ninjaKalori <= 0)
			{
				cout << "Yavru ninjalarin enerjisi bitti" << endl;
			}

			raporVer(-1);
			raporVerildi = true;
			return;
		}

		if (sonuc == -1)
		{
			raporVer(-1);
			raporVerildi = true;
			return;
		}
	}
}

int Kontrol::buyukDondur(int ilkSayi, int ikinciSayi)
{
	if (ilkSayi > ikinciSayi)
	{
		return ilkSayi;
	}
	else
	{
		return ikinciSayi;
	}
}

int Kontrol::Uyari()
{
	if (TMNT->splinterKalori < 201)
	{
		cout << "Splinter Ustanin enerjisi azaldi" << endl;
		float gececekZaman = (TMNT->gidilenYolMetre / TMNT->splinterHiz);

		float ihtiyacEnerji = 0;

		if (TMNT->splinterHiz == 2)
		{
			ihtiyacEnerji = (gececekZaman * 0.4f);
		}
		else
		{
			ihtiyacEnerji = (gececekZaman * 0.2f);
		}

		bool pizzaYenildi = false;

		int pizzaKalori = 0;
		int pizzaIndex = 0;

		for (int i = 0; i < pizzaSayisi; i++)
		{
			if (TMNT->toplananPizalar[i] != -1)
			{
				int kalori = pizzalar[TMNT->toplananPizalar[i] - 1]->kalori;
				if (kalori > pizzaKalori)
				{
					pizzaKalori = kalori;
					pizzaIndex = i;
				}

				if (pizzaKalori >= ihtiyacEnerji)
				{
					cout << "Splinter Ustanin yolda yedigi pizzanin ID'si : " << TMNT->toplananPizalar[pizzaIndex] << endl;
					cout << "Splinter Ustanin yolda yedigi pizzanin kalorisi : " << pizzaKalori << endl;

					pizzaYenildi = true;
					TMNT->toplananPizalar[pizzaIndex] = -1;

					TMNT->splinterKalori += pizzaKalori;

					TMNT->splinterCantaKalori -= pizzaKalori;

					if (TMNT->splinterKalori > 2000)
					{
						TMNT->splinterKalori = 2000;
					}

					break;
				}
			}

			if (pizzaYenildi)
			{
				break;
			}
			else
			{
				cout << "Splinter Ustanin yolda yedigi pizzanin ID'si : " << TMNT->toplananPizalar[pizzaIndex] << endl;
				cout << "Splinter Ustanin yolda yedigi pizzanin kalorisi : " << pizzaKalori << endl;

				TMNT->toplananPizalar[pizzaIndex] = -1;

				TMNT->splinterCantaKalori -= pizzaKalori;

				TMNT->splinterKalori += pizzaKalori;

				if (TMNT->splinterKalori > 2000)
				{
					TMNT->splinterKalori = 2000;
				}

				geriDonusBasla = true;

				if (TMNT->splinterKalori > 0)
					return 0;
				else
					return -1;
			}
		}
	}

	if (TMNT->ninjaKalori < 21)
	{
		cout << "Yavru ninjalarin enerjisi azaldi" << endl;

		bool pizzaVar = false;

		for (int i = 0; i < pizzaSayisi; i++)
		{
			if (TMNT->toplananPizalar[i] != -1)
			{
				pizzaVar = true;
				break;
			}
		}

		if (pizzaVar)
		{
			float gececekZaman = (TMNT->gidilenYolMetre / TMNT->splinterHiz);

			TMNT->splinterHiz = 4;

			geriDonusBasla = true;
		}
		else
		{
			TMNT->splinterHiz = 4;
			geriDonusBasla = true;
		}
	}

	return 0;
}

void Kontrol::PizzaOlustur(Pizza **pizzaDizi, int MAX)
{
	pizzaSayisi = 0;

	cout << "Harita uzerinde yerlestirilecek pizza sayisi :" << endl;
	cin >> pizzaSayisi;
	cin.ignore();
	for (int i = 0; i < pizzaSayisi; i++)
	{
		string noktaID = "";
		cout << "Pizzanin yerlestirilecek nokta bilgisi : (N1 seklinde)" << endl;
		getline(cin, noktaID);

		cout << "Pizza tipleri:" << endl;

		string pizzaID = "";

		for (int j = 0; j < MAX; j++)
		{
			cout << pizzaDizi[j]->ID << ", Boyutu : " << pizzaDizi[j]->boyut << endl;
		}
		getline(cin, pizzaID);

		if (PizzaEkle(noktaID, pizzaID, i, yeraltiDugumler, pizzaDizi) == false)
		{
			i--;
		}
	}

	TMNT->toplananPizalar = new int[pizzaSayisi];

	for (int i = 0; i < pizzaSayisi; i++)
	{
		TMNT->toplananPizalar[i] = -1;
	}
}

bool Kontrol::PizzaEkle(string noktaID, string pizzaID, int sira, Dugum *dugumler, Pizza **pizzaDizi)
{
	bool pizzaBulundu = false;

	for (int i = 0; i < MAX_DUGUM; i++)
	{
		if (dugumler[i].ID.compare(noktaID) == 0)
		{
			for (int j = 0; j < MAX_PIZZA; j++)
			{
				if (pizzaDizi[j]->ID.compare(pizzaID) == 0)
				{
					Pizza *yeniPizza = new Pizza(pizzaDizi[j]->ID, pizzaDizi[j]->boyut, pizzaDizi[j]->kalori, pizzaDizi[j]->hacim);
					dugumler[i].pizza = yeniPizza;
					pizzaBulundu = true;

					return true;
				}
			}
		}
	}

	if (pizzaBulundu == false)
	{
		cout << pizzaID << " bilgisine sahip pizza bulunamadi" << endl;

		return false;
	}

	return false;
}

Dugum *Kontrol::DusmanEkle(Dugum *dugumler, Dusman **dusmanDizi)
{
	for (int i = 0; i < MAX_DUGUM; i++)
	{
		for (int j = 0; j < MAX_DUSMAN; j++)
		{
			if (dugumler[i].ID == dusmanDizi[j]->konum)
			{
				dugumler[i].dusman = new Dusman(dusmanDizi[j]->ID, dusmanDizi[j]->kalori, dusmanDizi[j]->konum, dusmanDizi[j]->dakika);
			}
		}
	}

	return dugumler;
}

void Kontrol::raporVer(int sonuc)
{
	if (sonuc == -1)
	{
		cout << "Gorev Basarisiz" << endl;
		TMNT->splinterCantaKalori = 0.f;
	}
	else
	{
		if (TMNT->splinterKalori < 0 || TMNT->ninjaKalori < 0)
		{
			TMNT->splinterCantaKalori = 0.f;
			cout << "Gorev Basarisiz" << endl;
		}
		else
			cout << "Gorev Basarili" << endl;
	}

	cout << "Splinter Ustanin Ugradigi Optimal Yollar : " << endl;

	cout << "EV->";
	for (int i = 0; i < MAX_DUGUM; i++)
	{
		if (TMNT->gidilenYollar[i] != -1)
		{
			if (TMNT->gidilenYollar[i] == 0)
			{
				cout << "EV->";
				continue;
			}
			cout << "N" << TMNT->gidilenYollar[i];

			if (TMNT->dugumKarsilasilan[TMNT->gidilenYollar[i]] == 1)
			{
				TMNT->dugumKarsilasilan[TMNT->gidilenYollar[i]] = 0;
				cout << "(Pizza)";
			}
			else if (TMNT->dugumKarsilasilan[TMNT->gidilenYollar[i]] == 2)
			{
				TMNT->dugumKarsilasilan[TMNT->gidilenYollar[i]] = 0;
				cout << "(Dusman)";
			}
			else if (TMNT->dugumKarsilasilan[TMNT->gidilenYollar[i]] == 3)
			{
				TMNT->dugumKarsilasilan[TMNT->gidilenYollar[i]] = 0;
				cout << "(Dusman)(Pizza)";
			}
			cout << "->";
		}
		else
			break;
	}

	cout << endl;

	cout << "Karsilasilan dusman bilgileri : " << endl;

	for (int i = 0; i < dusmanSayisi; i++)
	{
		if (TMNT->karsilasilanDusmanlar[i] != -1)
		{
			cout << "D" << TMNT->karsilasilanDusmanlar[i] << ", ";
		}
	}

	cout << endl;

	cout << "Toplamis oldugu pizzalarin kalorisi : " << TMNT->toplananKalori << endl;

	cout << "Splinter Ustanin yolda harcadigi kalori miktari : " << TMNT->harcadigiKalori << endl;

	cout << "Eve getirdigi pizzalarin kalorisi : " << TMNT->splinterCantaKalori << endl;

	cout << "Usta Splinter'in kalan enerji seviyesi : " << TMNT->splinterKalori << endl;

	cout << "Usta Splinter'a dusen pizza kalori miktari : " << TMNT->splinterCantaKalori / 2 << endl;

	cout << "Yavru Ninjalarin kalan enerji seviyesi : " << TMNT->ninjaKalori << endl;

	cout << "Yavru Ninjalara dusen kalori miktari : " << (TMNT->splinterCantaKalori / 2) / 4 << endl;
}

Kontrol::~Kontrol()
{
	for (int i = 0; i < MAX_DUGUM; i++)
	{
		if (yeraltiDugumler[i].ID != "")
		{
			if (yeraltiDugumler[i].pizza != NULL)
			{
				delete yeraltiDugumler[i].pizza;
			}

			if (yeraltiDugumler[i].dusman != NULL)
			{
				delete yeraltiDugumler[i].dusman;
			}
		}
	}

	if (itemDizi2 != NULL)
		delete[] itemDizi2;

	if (yeraltiDugumler != NULL)
		delete[] yeraltiDugumler;

	for (int i = 0; i < MAX_DUGUM; i++)
	{
		delete[] dugumler[i];
	}

	if (dugumler != NULL)
		delete[] dugumler;

	if (pizzalar != NULL)
		delete[] pizzalar;

	if (dusmanlar != NULL)
		delete[] dusmanlar;

	if (doluDugumIDList != NULL)
		delete[] doluDugumIDList;

	if (TMNT != NULL)
	{
		delete[] TMNT->toplananPizalar;
		delete[] TMNT->karsilasilanDusmanlar;
		delete[] TMNT->gidilenYollar;
		delete[] TMNT->dugumKarsilasilan;

		delete TMNT;
	}
}

int Kontrol::Say(string textAdi)
{

	ifstream dosya;
	dosya.open(textAdi);

	string satir = "";
	int counter = 0;

	if (dosya.is_open())
	{
		while (!dosya.eof())
		{
			getline(dosya, satir);
			counter++;
		}
	}

	dosya.close();

	return counter;
}

void Kontrol::PizzaOku(Pizza **pizzalar, int MAX)
{
	ifstream dosya;
	dosya.open("PizzaBilgileri.txt");

	string satir = "", ID = "", boyut = "";
	int kalori, hacim;
	int counter = 0;

	if (dosya.is_open())
	{
		for (int i = 0; i < MAX; i++)
		{
			getline(dosya, satir);

			ID = satir.substr(0, satir.find(','));
			satir = satir.substr(satir.find(',') + 1);

			boyut = satir.substr(0, satir.find(','));
			satir = satir.substr(satir.find(',') + 1);

			kalori = stoi(satir.substr(0, satir.find(',')));
			satir = satir.substr(satir.find(',') + 1);

			hacim = stoi(satir.substr(0, satir.find(',')));
			satir = satir.substr(satir.find(',') + 1);

			pizzalar[i] = new Pizza(ID, boyut, kalori, hacim);
		}
	}
	dosya.close();
}

Dusman **Kontrol::DusmanOku(Dusman *dusmanlar[], int MAX)
{
	ifstream dusman;
	dusman.open("DusmanBilgisi.txt");

	string satir = "", ID = "", konum = "";
	int kalori, dakika;

	dusmanSayisi = 0;

	if (dusman.is_open())
	{
		for (int i = 0; i < MAX_DUSMAN; i++)
		{
			getline(dusman, satir);

			ID = satir.substr(0, satir.find(','));
			satir = satir.substr(satir.find(',') + 1);

			kalori = stoi(satir.substr(0, satir.find(',')));
			satir = satir.substr(satir.find(',') + 1);

			konum = satir.substr(0, satir.find(','));
			satir = satir.substr(satir.find(',') + 1);

			dakika = stoi(satir.substr(0, satir.find(',')));
			satir = satir.substr(satir.find(',') + 1);

			dusmanlar[i] = new Dusman(ID, kalori, konum, dakika);
			dusmanSayisi++;
		}

		dusman.close();
	}

	TMNT->karsilasilanDusmanlar = new int[dusmanSayisi];

	for (int i = 0; i < dusmanSayisi; i++)
	{
		TMNT->karsilasilanDusmanlar[i] = -1;
	}

	return dusmanlar;
}

void Kontrol::GrafOlustur(int **dugum, int MAX)
{
	ifstream graf;
	graf.open("KanalBaglantiBilgileri.txt");
	string satir = "";
	string eleman = "";
	char next;
	int bas = 0;

	if (graf.is_open())
	{
		for (int i = 0; i < MAX; i++)
		{
			getline(graf, satir);

			for (int j = 0; j < MAX; j++)
			{
				if (j == 40)
				{
					eleman = satir.substr(bas);
				}
				else
				{
					eleman = satir.substr(bas, satir.find(','));
					satir = satir.substr(satir.find(',') + 1);
				}

				if (i == j)
				{
					dugum[i][j] = 0;
					eleman = "";
				}
				else
				{
					dugum[i][j] = stoi(eleman);
					eleman = "";
				}
			}
		}
	}

	graf.close();
}

int Kontrol::KnapSack(int kapasite, Dugum *dugumDizi, int sayi)
{
	int *hacimler = new int[sayi];
	int *kaloriler = new int[sayi];

	int index = 0;

	for (int i = 0; i < MAX_DUGUM; i++)
	{
		if (dugumDizi[i].pizza != NULL)
		{
			kaloriler[index] = dugumDizi[i].pizza->kalori;
			hacimler[index] = dugumDizi[i].pizza->hacim;
			index++;
		}
	}

	itemDizi2 = new int[sayi];

	for (int i = 0; i < sayi; i++)
	{
		itemDizi2[i] = -1;
	}

	int sonuc = Hesap(2500, hacimler, kaloriler, sayi, itemDizi2);

	delete[] hacimler;
	delete[] kaloriler;

	return sonuc;
}

void Kontrol::DugumSirala(int secilenPizzalar[], int sayi)
{
	doluDugumIDList = new int[sayi];

	int sayac = 0;

	int idSayac = 0;

	for (int i = 0; i < sayi; i++)
	{
		for (int j = 0; j < MAX_DUGUM; j++)
		{
			if (yeraltiDugumler[j].pizza != NULL)
			{
				if (sayac == secilenPizzalar[i])
				{
					doluDugumIDList[idSayac] = j;
					idSayac++;
					break;
				}
				else
				{
					sayac++;
				}
			}
		}

		sayac = 0;
	}
}

int Kontrol::Hesap(int kapasite, int wt[], int val[], int n, int itemDizi[])
{
	int *itemDiziSira = new int[n];

	for (int i = 0; i < n; i++)
	{
		itemDiziSira[i] = i;
	}

	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < (n - i - 1); j++)
		{

			if ((float)(val[j + 1] / wt[j + 1]) > (float)(val[j] / wt[j]))
			{
				int kaloriTasiyici = val[j + 1];
				val[j + 1] = val[j];
				val[j] = kaloriTasiyici;

				int hacimTasiyici = wt[j + 1];
				wt[j + 1] = wt[j];
				wt[j] = hacimTasiyici;

				int idTasiyici = itemDiziSira[j + 1];
				itemDiziSira[j + 1] = itemDiziSira[j];
				itemDiziSira[j] = idTasiyici;
			}
		}
	}

	int number = 0;
	int sonuc = 0;

	int i = 0;

	while (kapasite > 0 && number < n)
	{
		if (wt[number] <= kapasite)
		{
			sonuc += val[number];
			kapasite -= wt[number];
			itemDizi[i] = itemDiziSira[number];
			i++;
		}

		++number;
	}

	DugumSirala(itemDizi, i);

	return sonuc;
}

int Kontrol::MinUzaklik(int mesafe[], bool visited[])
{
	int min = SONSUZ, min_index;

	for (int v = 0; v < MAX_DUGUM; v++)
		if (visited[v] == false && mesafe[v] <= min)
			min = mesafe[v], min_index = v;

	return min_index;
}

int Kontrol::PrintPath(int parent[], int j, int *sayilarDizi, int indeks)
{
	if (j < 0)
		return indeks;

	if (j == 0)
		return indeks;

	if (parent[j] == -1)
		return indeks;

	indeks = PrintPath(parent, parent[j], sayilarDizi, indeks);

	sayilarDizi[indeks] = j;
	indeks++;

	return indeks;
}

int Kontrol::PrintSolution(int mesafe[], int n, int parent[], int baslangicKonum, int konumlar[], int sayi, int *sayilarDizi, int indeks)
{
	int indeksTutucu = konumlar[0];
	int sonIndex = 0;

	for (int i = 0; i < sayi; i++)
	{
		if (konumlar[i] != -1)
		{
			indeksTutucu = konumlar[i];
			sonIndex = i;
			break;
		}
	}

	for (int i = 0; i < sayi; i++)
	{
		if (konumlar[i] != -1 && mesafe[konumlar[i]] < mesafe[indeksTutucu])
		{
			sonIndex = i;
			indeksTutucu = konumlar[i];
		}
	}

	if (indeks > 0)
		indeks--;

	indeks = PrintPath(parent, indeksTutucu, sayilarDizi, indeks);

	konumlar[sonIndex] = -1;
	return indeks;
}

int Kontrol::OptimalYolBul(int **graph, int konumlar[], int baslangicKonum, int sayi, int *sayilarDizi, int indeks)
{
	int V = MAX_DUGUM;

	int *mesafe = new int[V];
	bool *visited = new bool[V];

	int *parent = new int[V];

	for (int i = 0; i < V; i++)
	{
		parent[0] = -1;
		mesafe[i] = SONSUZ;
		visited[i] = false;
	}

	if (indeks != 0)
		visited[0] = true;

	mesafe[baslangicKonum] = 0;

	for (int count = 0; count < V - 1; count++)
	{
		int enYakinDugum = MinUzaklik(mesafe, visited);

		visited[enYakinDugum] = true;

		for (int v = 0; v < V; v++)
		{
			if (!visited[v] && graph[enYakinDugum][v] &&
				mesafe[enYakinDugum] + graph[enYakinDugum][v] < mesafe[v])
			{
				parent[v] = enYakinDugum;
				mesafe[v] = mesafe[enYakinDugum] + graph[enYakinDugum][v];
			}
		}
	}

	indeks = PrintSolution(mesafe, V, parent, baslangicKonum, konumlar, sayi, sayilarDizi, indeks);

	delete[] visited;
	delete[] parent;

	return indeks;
}

int *Kontrol::HaritaCiz(int **mesafeler, int sayi, int *dizi)
{
	int sayilarDiziIndex = 0;
	int baslangic = 0;
	int *sonuclar = new int[MAX_DUGUM];
	int sayac = 0;

	for (int i = 0; i < sayi; i++)
	{
		sayilarDiziIndex = OptimalYolBul(mesafeler, dizi, baslangic, sayi, sonuclar, sayilarDiziIndex);

		baslangic = sonuclar[sayilarDiziIndex - 1];
		sayac = sayilarDiziIndex;
	}

	for (int i = sayilarDiziIndex; i >= 0; i--)
	{
		sonuclar[i + 1] = sonuclar[i];
	}
	sonuclar[0] = 0;
	for (int i = sayilarDiziIndex + 1; i < MAX_DUGUM; i++)
	{
		sonuclar[i] = -1;
	}
	return sonuclar;
}
