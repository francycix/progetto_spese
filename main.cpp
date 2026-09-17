#include <iostream>
#include <libpq-fe.h>
#include <string>
#include <limits>
#include <cctype>

using namespace std;

void gestisciCategorie(PGconn* conn) {
    string nome;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Inserisci il nome della categoria: ";

    getline(cin, nome);

    if (nome.empty()) {
    cout << "Errore: il nome della categoria non puo essere vuoto." << endl;
    return;
    }

    const char* parametri[1] = {nome.c_str()};

    PGresult* risultato = PQexecParams(
    conn,
    "SELECT id_categoria FROM categorie WHERE nome = $1",
    1,
    nullptr,
    parametri,
    nullptr,
    nullptr,
    0
);

if (PQresultStatus(risultato) != PGRES_TUPLES_OK) {
    cout << "Errore durante la ricerca della categoria." << endl;
    PQclear(risultato);
    return;
}

if (PQntuples(risultato) > 0) {
    cout << "La categoria esiste gia." << endl;
    PQclear(risultato);
    return;
}

PQclear(risultato);

risultato = PQexecParams(
    conn,
    "INSERT INTO categorie (nome) VALUES ($1)",
    1,
    nullptr,
    parametri,
    nullptr,
    nullptr,
    0
);

if (PQresultStatus(risultato) == PGRES_COMMAND_OK) {
    cout << "Categoria inserita correttamente." << endl;
} else {
    cout << "Errore durante l'inserimento della categoria." << endl;
}

PQclear(risultato);
}

bool dataValida(const string& data) {
    if (data.length() != 10) {
        return false;
    }

    if (data[4] != '-' || data[7] != '-') {
        return false;
    }

    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) {
            continue;
        }

        if (!isdigit(data[i])) {
            return false;
        }
    }

    int anno = stoi(data.substr(0, 4));
    int mese = stoi(data.substr(5, 2));
    int giorno = stoi(data.substr(8, 2));

    if (anno < 1 || mese < 1 || mese > 12) {
        return false;
    }

    int giorniMese[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    bool bisestile =
        (anno % 400 == 0) ||
        (anno % 4 == 0 && anno % 100 != 0);

    if (bisestile) {
        giorniMese[1] = 29;
    }

    if (giorno < 1 || giorno > giorniMese[mese - 1]) {
        return false;
    }

    return true;
}

void inserisciSpesa(PGconn* conn) {
string data;
double importo;
string categoria;
string descrizione;

cin.ignore(numeric_limits<streamsize>::max(), '\n');

cout << "Inserisci la data (YYYY-MM-DD): ";
getline(cin, data);

if (!dataValida(data)) {
    cout << "Errore: la data deve essere valida e nel formato YYYY-MM-DD." << endl;
    return;
}

cout << "Inserisci l'importo: ";

if (!(cin >> importo)) {
    cout << "Errore: inserire un numero valido." << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return;
}

if (importo <= 0) {
    cout << "Errore: l'importo deve essere maggiore di zero." << endl;
    return;
}

cin.ignore(numeric_limits<streamsize>::max(), '\n');

cout << "Inserisci il nome della categoria: ";
getline(cin, categoria);

cout << "Inserisci una descrizione (facoltativa): ";
getline(cin, descrizione);

const char* parametriCategoria[1] = {categoria.c_str()};

PGresult* risultatoCategoria = PQexecParams(
    conn,
    "SELECT id_categoria FROM categorie WHERE nome = $1",
    1,
    nullptr,
    parametriCategoria,
    nullptr,
    nullptr,
    0
);

if (PQresultStatus(risultatoCategoria) != PGRES_TUPLES_OK) {
    cout << "Errore durante la ricerca della categoria." << endl;
    PQclear(risultatoCategoria);
    return;
}
if (PQntuples(risultatoCategoria) == 0) {
    cout << "Errore: la categoria non esiste." << endl;
    PQclear(risultatoCategoria);
    return;
}
string idCategoria = PQgetvalue(risultatoCategoria, 0, 0);

PQclear(risultatoCategoria);

string importoTesto = to_string(importo);

const char* parametriSpesa[4] = {
    data.c_str(),
    importoTesto.c_str(),
    idCategoria.c_str(),
    descrizione.c_str()
};
PGresult* risultatoSpesa = PQexecParams(
    conn,
    "INSERT INTO spese (data_spesa, importo, id_categoria, descrizione) "
    "VALUES ($1, $2, $3, $4)",
    4,
    nullptr,
    parametriSpesa,
    nullptr,
    nullptr,
    0
);
if (PQresultStatus(risultatoSpesa) == PGRES_COMMAND_OK) {
    cout << "Spesa inserita correttamente." << endl;
} else {
    cout << "Errore durante l'inserimento della spesa." << endl;
}
PQclear(risultatoSpesa);
}

bool meseValido(const string& mese) {
    if (mese.length() != 7) {
        return false;
    }

    if (mese[4] != '-') {
        return false;
    }

    if (!isdigit(mese[0]) ||
        !isdigit(mese[1]) ||
        !isdigit(mese[2]) ||
        !isdigit(mese[3]) ||
        !isdigit(mese[5]) ||
        !isdigit(mese[6])) {
        return false;
    }

    int numeroMese = stoi(mese.substr(5, 2));

    if (numeroMese < 1 || numeroMese > 12) {
        return false;
    }

    return true;
}

void definisciBudget(PGconn* conn) {
string mese;
string categoria;
double importoBudget;

cin.ignore(numeric_limits<streamsize>::max(), '\n');

cout << "Inserisci il mese (YYYY-MM): ";
getline(cin, mese);

if (!meseValido(mese)) {
    cout << "Errore: il mese deve essere nel formato YYYY-MM." << endl;
    return;
}

cout << "Inserisci il nome della categoria: ";
getline(cin, categoria);

cout << "Inserisci l'importo del budget: ";

if (!(cin >> importoBudget)) {
    cout << "Errore: inserire un numero valido." << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return;
}

if (importoBudget <= 0) {
    cout << "Errore: il budget deve essere maggiore di zero." << endl;
    return;
}
const char* parametriCategoria[1] = {categoria.c_str()};
PGresult* risultatoCategoria = PQexecParams(
    conn,
    "SELECT id_categoria FROM categorie WHERE nome = $1",
    1,
    nullptr,
    parametriCategoria,
    nullptr,
    nullptr,
    0
);
if (PQresultStatus(risultatoCategoria) != PGRES_TUPLES_OK) {
    cout << "Errore durante la ricerca della categoria." << endl;
    PQclear(risultatoCategoria);
    return;
}
if (PQntuples(risultatoCategoria) == 0) {
    cout << "Errore: la categoria non esiste." << endl;
    PQclear(risultatoCategoria);
    return;
}
string idCategoria = PQgetvalue(risultatoCategoria, 0, 0);
PQclear(risultatoCategoria);
string importoBudgetTesto = to_string(importoBudget);
const char* parametriBudget[3] = {
    mese.c_str(),
    idCategoria.c_str(),
    importoBudgetTesto.c_str()
};
PGresult* risultatoBudget = PQexecParams(
    conn,
    "INSERT INTO budget (mese, id_categoria, importo_budget) "
    "VALUES ($1, $2, $3) "
    "ON CONFLICT (id_categoria, mese) "
    "DO UPDATE SET importo_budget = EXCLUDED.importo_budget",
    3,
    nullptr,
    parametriBudget,
    nullptr,
    nullptr,
    0
);
if (PQresultStatus(risultatoBudget) == PGRES_COMMAND_OK) {
    cout << "Budget mensile salvato correttamente." << endl;
} else {
    cout << "Errore durante il salvataggio del budget." << endl;
}
PQclear(risultatoBudget);
}

void visualizzaReport(PGconn* conn) {
int sceltaReport;
do {
cout << "\n-------------------------" << endl;
cout << "MENU REPORT" << endl;
cout << "-------------------------" << endl;
cout << "1. Totale spese per categoria" << endl;
cout << "2. Spese mensili vs budget" << endl;
cout << "3. Elenco completo delle spese" << endl;
cout << "4. Ritorna al menu principale" << endl;
cout << "-------------------------" << endl;
cout << "Inserisci la tua scelta: ";
if (!(cin >> sceltaReport)) {
    cout << "Scelta non valida." << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    continue;
}
switch (sceltaReport) {

case 1: {
    PGresult* risultato = PQexec(
        conn,
        "SELECT categorie.nome AS categoria, "
        "SUM(spese.importo) AS totale_speso "
        "FROM spese "
        "JOIN categorie "
        "ON spese.id_categoria = categorie.id_categoria "
        "GROUP BY categorie.nome "
        "ORDER BY categorie.nome"
    );

    if (PQresultStatus(risultato) != PGRES_TUPLES_OK) {
        cout << "Errore durante la generazione del report." << endl;
        PQclear(risultato);
        break;
    }

    cout << "\nCategoria - Totale speso" << endl;
    cout << "-------------------------" << endl;

    for (int i = 0; i < PQntuples(risultato); i++) {
        cout << PQgetvalue(risultato, i, 0)
             << " - "
             << PQgetvalue(risultato, i, 1)
             << " euro" << endl;
    }

    PQclear(risultato);
    break;
}

case 2: {
    PGresult* risultato = PQexec(
        conn,
        "SELECT TO_CHAR(spese.data_spesa, 'YYYY-MM') AS mese, "
        "categorie.nome AS categoria, "
        "budget.importo_budget AS budget, "
        "SUM(spese.importo) AS speso "
        "FROM spese "
        "JOIN categorie "
        "ON spese.id_categoria = categorie.id_categoria "
        "JOIN budget "
        "ON spese.id_categoria = budget.id_categoria "
        "AND TO_CHAR(spese.data_spesa, 'YYYY-MM') = budget.mese "
        "GROUP BY TO_CHAR(spese.data_spesa, 'YYYY-MM'), "
        "categorie.nome, budget.importo_budget "
        "ORDER BY mese, categoria"
    );
if (PQresultStatus(risultato) != PGRES_TUPLES_OK) {
cout << "Errore durante la generazione del report." << endl;
PQclear(risultato);
break;
}
for (int i = 0; i < PQntuples(risultato); i++) {
    string mese = PQgetvalue(risultato, i, 0);
    string categoria = PQgetvalue(risultato, i, 1);

    double budget = stod(PQgetvalue(risultato, i, 2));
    double speso = stod(PQgetvalue(risultato, i, 3));
cout << "\nMese: " << mese << endl;
cout << "Categoria: " << categoria << endl;
cout << "Budget: " << budget << " euro" << endl;
cout << "Speso: " << speso << " euro" << endl;
if (speso > budget) {
    cout << "Stato: SUPERAMENTO BUDGET" << endl;
} else {
    cout << "Stato: BUDGET RISPETTATO" << endl;
}
cout << "-------------------------" << endl;
}
PQclear(risultato);
break;
}

case 3: {
    PGresult* risultato = PQexec(
        conn,
        "SELECT spese.data_spesa, "
        "categorie.nome AS categoria, "
        "spese.importo, "
        "spese.descrizione "
        "FROM spese "
        "JOIN categorie "
        "ON spese.id_categoria = categorie.id_categoria "
        "ORDER BY spese.data_spesa"
    );
cout << "\nData - Categoria - Importo - Descrizione" << endl;
cout << "-----------------------------------------" << endl;

for (int i = 0; i < PQntuples(risultato); i++) {
    cout << PQgetvalue(risultato, i, 0)
         << " - "
         << PQgetvalue(risultato, i, 1)
         << " - "
         << PQgetvalue(risultato, i, 2)
         << " euro - "
         << PQgetvalue(risultato, i, 3)
         << endl;
}
PQclear(risultato);
break;
}

case 4:
    cout << "Ritorno al menu principale..." << endl;
    break;
default:
    cout << "Scelta non valida. Riprovare." << endl;
            }

    } while (sceltaReport != 4);
}

int main() {
    PGconn* conn = PQconnectdb("");

    if (PQstatus(conn) != CONNECTION_OK) {
        cout << "Errore di connessione al database." << endl;
        PQfinish(conn);
        return 1;
    }

    cout << "Connessione al database riuscita!" << endl;

int scelta;

do {
    cout << "\n-------------------------" << endl;
    cout << "SISTEMA SPESE PERSONALI" << endl;
    cout << "-------------------------" << endl;
    cout << "1. Gestione Categorie" << endl;
    cout << "2. Inserisci Spesa" << endl;
    cout << "3. Definisci Budget Mensile" << endl;
    cout << "4. Visualizza Report" << endl;
    cout << "5. Esci" << endl;
    cout << "-------------------------" << endl;
    cout << "Inserisci la tua scelta: ";

    if (!(cin >> scelta)) {
    cout << "Scelta non valida. Inserire un numero da 1 a 5." << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    continue;
}

    switch (scelta) {

        case 1:
         gestisciCategorie(conn);
        break;

        case 2:
         inserisciSpesa(conn);
        break;

        case 3:
        definisciBudget(conn);
        break;

        case 4:
        visualizzaReport(conn);
        break;

        case 5:
            cout << "Uscita dal programma..." << endl;
            break;

        default:
            cout << "Scelta non valida. Riprovare." << endl;
    }

} while (scelta != 5);

    PQfinish(conn);
    return 0;
}