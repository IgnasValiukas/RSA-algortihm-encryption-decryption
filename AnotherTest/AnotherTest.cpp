#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include <windows.h>

using namespace std;
//konsoles teksto spalvos funkcija
void SetColor(int ForgC) {
    WORD wColor;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}

int gcd(int a, int b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}
//funkcija kuri tikrina ar pirminis skaicius
bool pirminis(int n) {
    if (n <= 1) {
        return false;
    }
    for (int i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

int atvirkstine(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1; // atvirkstine neegzistuoja
}

//funkcija kuri sugeneruoja viesaji ir privatu rakta
pair<pair<int, int>, pair<int, int>> generuotiRaktus(int p, int q) {
    if (!(pirminis(p) && pirminis(q))) {
        SetColor(13);
        throw invalid_argument("<KLAIDA> abu skaiciai turi buti pirminiai");
        SetColor(15);
    }
    int n = p * q;
    int phi = (p - 1) * (q - 1);
    int e = 2;
    while (gcd(e, phi) != 1) {
        e++;
    }
    int d = atvirkstine(e, phi);
    return make_pair(make_pair(n, e), make_pair(n, d));
}

//funkcija kuri uzsifruoja teksta
vector<int> sifruoti(pair<int, int> pk, string tekstas) {
    int n = pk.first;
    int e = pk.second;
    vector<int> uzsifruotasTekstas;
    for (char c : tekstas) {
        int m = static_cast<int>(c);
        int uzsifruotas_m = 1;
        for (int i = 0; i < e; i++) {
            uzsifruotas_m = (uzsifruotas_m * m) % n;
        }
        uzsifruotasTekstas.push_back(uzsifruotas_m);
    }
    return uzsifruotasTekstas;
}


//funkcija kuri desifruoja teksta
string desifruoti(pair<int, int> pk, vector<int> uzsifruotasTekstas) {
    int n = pk.first;
    int d = pk.second;
    string tekstas = "";
    for (int c : uzsifruotasTekstas) {
        int uzsifruotas_c = 1;
        for (int i = 0; i < d; i++) {
            uzsifruotas_c = (uzsifruotas_c * c) % n;
        }
        tekstas += static_cast<char>(uzsifruotas_c);
    }
    return tekstas;
}

pair<int, int> findPrimes(int n) {
    int p, q;
    for (int i = 2; i <= sqrt(n); i++) {
        if (n % i == 0 && pirminis(i)) {
            p = i;
            q = n / i;
            if (pirminis(q))
                return make_pair(p, q);
        }
    }
    return make_pair(0, 0);
}

pair<int, int> generatePrivateKey(int phi, int e) {
    int d = 1;
    while ((d * e) % phi != 1) {
        d++;
    }
    return make_pair(phi, d);
}

int rastas_d(int e, int phi) {
    int k = 1;
    while (true) {
        int d = (k * phi + 1) / e;
        if ((k * phi + 1) % e == 0 && d > 0) {
            return d;
        }
        k++;
    }
}

int main() {
    SetColor(14);
    cout << "          <RSA SIFRAVIMO/DESIFRAVIMO ALGORITMAS>" << endl;
    cout << "___________________________________________________________" << endl;
    int pasirinkimas;
    //Pasirinkti, ar ivedame teksta, ar nuskaitome is failo
    SetColor(14);
    cout << "<MENIU>" << endl;
    cout << "1. Teksto ivedimas\n";
    cout << "2. Teksto skaitymas is failo\n";
    SetColor(9);
    cout << "Iveskite pasirinkima ir spauskite ENTER: ";
    SetColor(15);
    cin >> pasirinkimas;

    if (pasirinkimas == 1) {
        SetColor(14);
        cout << "___________________________________________________________" << endl;
        cout << "<PIRMINIU SKAICIU IVEDIMAS>" << endl;
        SetColor(15);
        int p, q;
        string tekstas;
        SetColor(9);
        cout << "Iveskite p(pirminis skaicius) ir spauskite ENTER: ";
        SetColor(15);
        cin >> p;
        SetColor(9);
        cout << "Iveskite q(pirminis skaicius) ir spauskite ENTER: ";
        SetColor(15);
        cin >> q;

        auto raktai = generuotiRaktus(p, q);
        auto viesasRaktas = raktai.first;
        auto privatusRaktas = raktai.second;
        //Teksto ivedimas ir sifravimas
        SetColor(9);
        cout << "Iveskite teksta ir spauskite ENTER: ";
        SetColor(15);
        cin.ignore(); //Ignoruojame '\n' simboli, kuris liko po skaitymo is pirmo cin objekto
        getline(cin, tekstas);
        auto uzsifruotasTekstas = sifruoti(viesasRaktas, tekstas);
        auto desifruotasTekstas = desifruoti(privatusRaktas, uzsifruotasTekstas);

        //Ciphertext ir vieas raktas issaugojimas faile
        ofstream uzsifruotasFailas("test.txt");
        for (int c : uzsifruotasTekstas) {
            uzsifruotasFailas << c << " ";
        }
        uzsifruotasFailas.close();

        //Isvedimas
        SetColor(14);
        cout << "\n___________________________________________________________" << endl;
        cout << "<SUGENERUOTI RAKTAI>" << endl;
        cout << "Viesas raktas: (" << viesasRaktas.first << ", " << viesasRaktas.second << ")" << endl;
        cout << "Privatus raktas: (" << privatusRaktas.first << ", " << privatusRaktas.second << ")" << endl;
        cout << "___________________________________________________________" << endl;
        cout << "<ATSAKYMAS>" << endl;
        SetColor(9);
        cout << "Uzsifruotas tekstas: ";
        SetColor(34);
        for (int c : uzsifruotasTekstas) {
            cout << c << " ";
        }
        cout << endl;
        SetColor(9);
        cout << "Desifruotas tekstas: ";
        SetColor(34);
        cout << desifruotasTekstas << endl;
        SetColor(15);
        cout << endl;
        ofstream out_file("viesas_raktas.txt");
        out_file << viesasRaktas.first << " " << viesasRaktas.second;
        out_file.close();
        //ofstream out_file1("privatus_raktas.txt");
        //out_file1 << privatusRaktas.first << " " << privatusRaktas.second;
        //out_file1.close();
    }
    else if (pasirinkimas == 2) {

        ifstream inputFile("viesas_raktas.txt");
        int n1, e;
        inputFile >> n1;
        inputFile >> e;
        inputFile.close();

        pair<int, int> pq = findPrimes(n1);
        int p = pq.first;
        int q = pq.second;
        cout << "p = " << p << ", q = " << q << ", e = " << e << endl;

        int phi = (p - 1) * (q - 1);
        //int d = generatePrivateKey(phi, e);
        int d = rastas_d(e, phi);
        pair<int, int> privatus_raktas = make_pair(n1, d);

        //auto raktai = generuotiRaktus(p, q);
        //auto viesasRaktas = raktai.first;
        //auto privatusRaktas = raktai.second;

        //Nuskaityti uzsifruota teksta is failo
        ifstream uzsifruotasFailas("test.txt");
        vector<int> uzsifruotasTekstas;
        int c;
        while (uzsifruotasFailas >> c) {
            uzsifruotasTekstas.push_back(c);
        }
        uzsifruotasFailas.close();

        auto desifruotasTekstas = desifruoti(privatus_raktas, uzsifruotasTekstas);

        cout << "Uzsifruotas tekstas: ";
        SetColor(34);
        for (int c : uzsifruotasTekstas) {
            cout << c << " ";
        }
        cout << endl;
        SetColor(9);
        cout << "Desifruotas tekstas: ";
        SetColor(34);
        cout << desifruotasTekstas << endl;
        SetColor(15);
    }
    else {
        SetColor(13);
        cout << "Neteisingas pasirinkimas." << endl;
        SetColor(15);
    }

    return 0;
}

