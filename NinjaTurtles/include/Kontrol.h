#pragma once
#include <iostream>
#include "Pizza.h"
#include "Dusman.h"

using namespace std;

struct Dugum
{
    string ID = "";
    Pizza* pizza = NULL;
    Dusman* dusman = NULL;
};

struct Degerler
{
    float ninjaKalori;
    float splinterKalori;
    float harcadigiKalori;
    int splinterCanta;
    float splinterCantaKalori;
    int splinterHiz;
    int mevcutKonum;
    int toplananKalori;
    int gidilenYolMetre;
    int* toplananPizalar;
    int* karsilasilanDusmanlar;
    int* gidilenYollar;
    int* dugumKarsilasilan;

    Degerler()
    {
        this->ninjaKalori = 100;
        this->splinterKalori = 500;
        this->splinterCanta = 0;
        this->splinterCantaKalori = 0;
        this->splinterHiz = 2;
        this->mevcutKonum = 0;
        this->toplananKalori = 0;
        this->gidilenYolMetre = 0;
        this->harcadigiKalori = 0;

    }
};

class Kontrol
{
public:
    Pizza** pizzalar;
    Dusman** dusmanlar;
    Dugum* yeraltiDugumler;
    int** dugumler;
    int MAX_PIZZA;
    int MAX_DUSMAN;
    int MAX_DUGUM;
    int pizzaSayisi;
    int dusmanSayisi;
    int* itemDizi2;
    Degerler* TMNT;
    int itemIndex = 0;
    int* doluDugumIDList;
    bool geriDonusBasla;
    bool raporVerildi;
    bool uyariCalisti;

    Kontrol();
    ~Kontrol();
    void MenuGetir(Pizza**, int);
    int OptimalYolBul(int** graph, int konumlar[], int baslangicKonum, int sayi, int* sayilarDizi, int indeks);
    int Uyari();
    void HareketBaslat(int* optimalYol, Dugum* yeralti, int** dugumler);
    void EnerjiMiktariHesapla(int yol, int konum, Dugum* yeralti, int yolIndeks);
    int buyukDondur(int, int);
    int MinUzaklik(int[], bool[]);
    void PizzaOlustur(Pizza**, int);
    bool PizzaEkle(string, string, int, Dugum*, Pizza**);
    void PizzaOku(Pizza** pizzalar, int MAX);
    Dusman** DusmanOku(Dusman* [], int MAX);
    Dugum* DusmanEkle(Dugum*, Dusman**);
    int* HaritaCiz(int**, int, int*);
    void DugumSirala(int secilenPizzalar[], int pizzaSayi);
    void raporVer(int);
    void Sirala(int hacimler[], int kaloriler[], int boyut);
    void GrafOlustur(int** dugum, int MAX);
    int Say(string text);
    int KnapSack(int, Dugum*, int);
    int Hesap(int, int[], int[], int, int[]);
    int PrintPath(int parent[], int j, int* sayilarDizi, int indeks);
    int PrintSolution(int mesafe[], int n, int parent[], int baslangicKonum, int konumlar[], int sayi, int* sayilarDizi, int indeks);
};