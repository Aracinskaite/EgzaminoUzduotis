
#include "lib.h"
using namespace std;


void ZodzioPrastinimas(string &Zodis) {
    Zodis.erase(remove_if(Zodis.begin(), Zodis.end(), ::ispunct), Zodis.end());
    transform(Zodis.begin(), Zodis.end(), Zodis.begin(), ::tolower);
}

void URLIeskojimas(const string &Tekstas, ofstream &URLOutput) {
    std::regex url(R"((https?://\S+|www\.\S+))");
    std::smatch radimas;

    URLOutput << left << setw(35) << "Rasti URL:" << endl;
    URLOutput << string(40, '-') << endl;

    auto pradzia = std::sregex_iterator(Tekstas.begin(), Tekstas.end(), url);
    auto pabaiga = std::sregex_iterator();

    for (std::sregex_iterator i = pradzia; i != pabaiga; ++i) {
        std::smatch radimas = *i;
        URLOutput << left << setw(35) << radimas.str() << endl;
    }
}


int main() {
    string failopavadinimas;
    cout << "Iveskite savo failo pavadinima: ";
    cin >> failopavadinimas;
    ifstream Failas(failopavadinimas);

    if (!Failas.is_open()) {
        cerr << "Failo atidaryti nepavyko" << endl;
        return 0;
    }

    unordered_map<string, int> ZodioSk;
    unordered_map<string, vector<int>> Vieta;

    string Zodis;
    int eilute = 0;
    stringstream Tekstas;

    while (getline(Failas, Zodis)) {
        eilute++;
        Tekstas << Zodis << '\n';

        istringstream iss(Zodis);

        while (iss >> Zodis) {
            ZodzioPrastinimas(Zodis);

            if (!Zodis.empty() && Zodis != "–") {
                ZodioSk[Zodis]++;
                Vieta[Zodis].push_back(eilute);
            }
        }
    }

    ofstream OutputFailas("OutputFailas.txt", std::ios::out | std::ios::binary);
    if (!OutputFailas.is_open()) {
        cerr << "Klaida: failas OutputFailas nesukurtas." << endl;
        return 1;
    }

    OutputFailas << left << setw(35) << "Zodis" << setw(25) << "Pasikartojimu skaicius" << setw(25) << "Eilutes" << endl;
    OutputFailas << string(85, '-') << endl;

    for (const auto &zod : ZodioSk) {
        const string &Zodis = zod.first;
        int count = zod.second;

        if (count > 1) {
            OutputFailas << left << setw(35) << Zodis << setw(25) << count;

            const auto &vie = Vieta[Zodis];
            for (size_t i = 0; i < vie.size(); ++i) {
                OutputFailas << vie[i];
                if (i < vie.size() - 1) {
                    OutputFailas << ", ";
                }
            }

            OutputFailas << endl;
        }
    }
    OutputFailas << string(85, '-') << endl;

    OutputFailas.close();


    ofstream URLOutput("URLOutput.txt", std::ios::out | std::ios::binary);
    if (!URLOutput.is_open()) {
        cerr << "Klaida: failas URLOutput nesukurtas." << endl;
        return 1;
    }

    URLIeskojimas(Tekstas.str(), URLOutput);

    URLOutput.close();

    cout << "Buvo sukurti 'OutputFailas.txt' ir 'URLOutput.txt'" << endl;

    return 0;
}
