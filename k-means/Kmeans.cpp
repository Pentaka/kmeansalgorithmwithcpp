#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;

// Veriseti sınıfı
class VeriSeti {
public:
    vector<vector<double>> veriler;
    int n, m;

    // CSV dosyasını okuma fonksiyonu
    void csvOku(string dosyaAdi) {
        ifstream dosya(dosyaAdi);

        if (!dosya.is_open()) {
            cout << "Dosya acilamadi!\n";
            return;
        }

        string satir;
        while (getline(dosya, satir)) {
            vector<double> veri;
            size_t pos = 0;
            string token;
            while ((pos = satir.find(",")) != string::npos) {
                token = satir.substr(0, pos);
                veri.push_back(atof(token.c_str()));
                satir.erase(0, pos + 1);
            }
            veri.push_back(atof(satir.c_str()));
            veriler.push_back(veri);
        }

        n = veriler.size();
        m = veriler[0].size();

        dosya.close();
    }

};

// K-means sınıfı
class KMeans {
public:
    int k;
    VeriSeti veriSeti;
    vector<vector<double>> merkezler;
    vector<int> tahminler;

    // Kurucu fonksiyon
    KMeans(int k, VeriSeti veriSeti) {
        this->k = k;
        this->veriSeti = veriSeti;
    }

    // Rastgele merkezler seçme fonksiyonu
    void rastgeleMerkezSec() {
    vector<double> min_degerler = {14.9, 11.3, 0.9, 10.6, 0.8, 1.42, 1.43, 8, 4.5, 44.8};
    vector<double> max_degerler = {82.2, 416.2, 325.3, 324, 254, 16.41, 9.67, 1079.1, 650.9, 90};
    srand(time(NULL));
    // her bir küme için bir merkez seç
    for (int i = 0; i < k; i++) {
        vector<double> merkez;
        for (int j = 0; j < veriSeti.m; j++) {
            // j. sütunun rastgele değerini belirle
            double rastgele = (double)rand() / RAND_MAX;
            double min_deger = min_degerler[j];
            double max_deger = max_degerler[j];
            double aralik = max_deger - min_deger;
            double deger = min_deger + rastgele * aralik;
            merkez.push_back(deger);
        }
        merkezler.push_back(merkez);
    }
     cout << "Ilk Kume merkezleri: " << endl;
    for (int i = 0; i < k; i++) {
        cout << "Merkez " << i+1 << ": ";
        for (int j = 0; j < veriSeti.m; j++) {
            cout << merkezler[i][j] << " ";
        }
        cout << endl;
    }
}
   // Mesafe hesaplama fonksiyonu
   double mesafe(vector<double> a, vector<double> b) {
    double toplam = 0.0;
    if (a.size() != b.size()) {

        return -1;
    }
    for (int i = 0; i < a.size(); i++) {
        double fark = a[i] - b[i];
        toplam += fark * fark;
    }
    return sqrt(toplam);
}


// kume merkezlerini yeniden hesaplama fonksiyonu
void tahminleriYenile() {
    tahminler.clear();
    for (int i = 0; i < veriSeti.n; i++) {
        double minMesafe = INFINITY;
        int minIndex = 0;
        for (int j = 0; j < k; j++) {
            double d = mesafe(veriSeti.veriler[i], merkezler[j]);
            if (d < minMesafe) {
                minMesafe = d;
                minIndex = j;
            }
        }
        tahminler.push_back(minIndex);
    }
}

// Merkezleri yeniden hesaplama fonksiyonu
bool merkezleriYenile() {
vector<vector<double>> yeniMerkezler(k, vector<double>(veriSeti.m, 0.0));
vector<int> kumeSayilari(k, 0);
for (int i = 0; i < veriSeti.n; i++) {
    int kumeIndex = tahminler[i];
    for (int j = 0; j < veriSeti.m; j++) {
        yeniMerkezler[kumeIndex][j] += veriSeti.veriler[i][j];
    }
    kumeSayilari[kumeIndex]++;
   for (int i = 0; i < kumeSayilari.size(); i++) {
        cout << "Kume " << i << " eleman sayisi: " << kumeSayilari[i] << endl;
    }

}

// Kümelerdeki veri sayısı sıfır olan merkezlerin yerini değiştirme
for (int i = 0; i < k; i++) {
    if (kumeSayilari[i] == 0) {
        int rastgeleIndex = rand() % veriSeti.n;
        yeniMerkezler[i] = veriSeti.veriler[rastgeleIndex];
    }
}

// Yeni merkezleri hesapla ve eski merkezlerle karşılaştır
bool degisiklikVar = false;
for (int i = 0; i < k; i++) {
    for (int j = 0; j < veriSeti.m; j++) {
        if (kumeSayilari[i] != 0) {
            double yeniMerkez = yeniMerkezler[i][j] / kumeSayilari[i];
            if (fabs(merkezler[i][j] - yeniMerkez) > 0.00001) {//iterasyon sayısı
                degisiklikVar = true;
            }
            merkezler[i][j] = yeniMerkez;
        }
    }
}


return degisiklikVar;
}

// K-means algoritması fonksiyonu
void kMeans() {
    int sayac = 0;
    rastgeleMerkezSec();
    tahminleriYenile();
    bool farkli = true;
    while (farkli) {
        farkli = merkezleriYenile();
        tahminleriYenile();
        sayac++;
    }
    cout << "iterasyon Sayisi: " << sayac <<endl;
    cout << "Son Kume merkezleri: " << endl;
for (int i = 0; i < k; i++) {
    cout << "Merkez " << i+1 << ": ";
    for (int j = 0; j < veriSeti.m; j++) {
        cout << merkezler[i][j] << " ";
    }
    cout << endl;
}
}
};

int main() {
// Veriseti okuma
VeriSeti veriSeti;
veriSeti.csvOku("veriler.csv");

// K-means algoritması
int k;
cout << "Kume sayisi giriniz: ";
cin >> k;
KMeans kmeans(k, veriSeti);
kmeans.kMeans();
return 0;
}
