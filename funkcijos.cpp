#include "funkcijos.h"

void nuskaitytiFailus(stringstream& pagr_tekstas, stringstream& papild_nuorodos, vector<string>& galunes) {
    ifstream f1("Lietuva.txt");
    if (!f1) {
        cerr << "KLAIDA: Nepavyko atidaryti Lietuva.txt!" << endl;}
    else {
        pagr_tekstas << f1.rdbuf();
        f1.close();}

    ifstream f2("nuorodos.txt");
    if (!f2) {
        cerr << "ISPEJIMAS: Nepavyko atidaryti nuorodos.txt (praleidžiama)." << endl;}
    else {
        papild_nuorodos << f2.rdbuf();
        f2.close();}

    ifstream f3("nuorodu_listas.txt");
    if (!f3) {
        cerr << "KLAIDA: Nepavyko atidaryti nuorodu_listas.txt! Regex neveiks." << endl;}
    else {
        string g;
        while (f3 >> g) galunes.push_back(g);
        f3.close();}}

void ieskotiNuorodu(stringstream& saltinis, vector<string>& rezultatas, const vector<string>& galunes, bool valytiTeksta, stringstream* isvalytasTekstas) {
    saltinis.clear();
    saltinis.seekg(0);
    if (galunes.empty()) {
        string eilute;
        while (getline(saltinis, eilute)) {
            if (valytiTeksta && isvalytasTekstas) *isvalytasTekstas << eilute << "\n";}
        return;}

    string galu_str = "(";
    for (const auto& g : galunes) {
        if (!g.empty()) galu_str += g + "|";}
    
    if (galu_str.back() == '|') galu_str.pop_back();
    galu_str += ")";

    try {
        string sablonas_str = R"(((http(s)?://)?(www\.)?([\w-]+\.)+)" + galu_str + R"(\b[^ \[\)\(\s\r\n\t\f]*))";
        regex url_regex(sablonas_str, regex::icase);

        string eilute;
        while (getline(saltinis, eilute)) {
            auto pradzia = sregex_iterator(eilute.begin(), eilute.end(), url_regex);
            auto pabaiga = sregex_iterator();

            for (auto i = pradzia; i != pabaiga; ++i) {
                rezultatas.push_back(i->str());}

            if (valytiTeksta && isvalytasTekstas) {
                *isvalytasTekstas << regex_replace(eilute, url_regex, " ") << "\n";}}}
    catch (const regex_error& e) {
        cerr << "Regex klaida: " << e.what() << " (Kodas: " << e.code() << ")" << endl;}}

void analizuotiZodzius(stringstream& tekstas, unordered_map<string, ZodzioInfo>& statistika) {
    tekstas.clear();
    tekstas.seekg(0);
    regex zodzio_regex(R"([a-zA-ZąĄčČęĘėĖįĮšŠųŲūŪžŽ]+)");
    string eilute;
    int eilutes_nr = 1;

    while (getline(tekstas, eilute)) {
        auto pradzia = sregex_iterator(eilute.begin(), eilute.end(), zodzio_regex);
        auto pabaiga = sregex_iterator();

        for (auto i = pradzia; i != pabaiga; ++i) {
            string zodis = i->str();
            statistika[zodis].kiekis++;
            statistika[zodis].eilutes.insert(eilutes_nr);}
        eilutes_nr++;}}

void spausdintiRezultatus(const unordered_map<string, ZodzioInfo>& statistika, const vector<string>& nuorodos, int riba) {
    vector<pair<string, ZodzioInfo>> surikiuoti(statistika.begin(), statistika.end());

    sort(surikiuoti.begin(), surikiuoti.end(), [](const auto& a, const auto& b) {
        if (a.second.kiekis != b.second.kiekis) return a.second.kiekis > b.second.kiekis;
        return a.first < b.first;});

    ofstream fZ("RezultataiZodziai.txt");
    if (fZ.is_open()) {
        fZ << left << setw(25) << "ŽODIS" << setw(15) << "KIEKIS" << "EILUTĖS" << endl;
        fZ << string(60, '-') << endl;

        for (const auto& p : surikiuoti) {
            if (p.second.kiekis > riba) {
                int lt_raidziu_skaicius = 0;
                for (unsigned char c : p.first) {
                    if (c >= 128) lt_raidziu_skaicius++;}
                int koreguotas_plotis = 25 + (lt_raidziu_skaicius / 2);

                fZ << left << setw(koreguotas_plotis) << p.first
                   << setw(15) << p.second.kiekis;
                
                bool pirmas = true;
                for (int nr : p.second.eilutes) {
                    if (!pirmas) fZ << ", ";
                    fZ << nr;
                    pirmas = false;}
                fZ << endl;}}
        fZ.close();}

    set<string> unikaliu_nuorodu_set(nuorodos.begin(), nuorodos.end());
    ofstream fN("RezultataiNuorodos.txt");
    for (const auto& n : unikaliu_nuorodu_set) fN << n << endl;
    fN.close();}
