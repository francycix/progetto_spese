# Progetto Gestione Spese Personali

Applicazione console per la gestione delle spese personali, sviluppata in C++ con database relazionale PostgreSQL.

## Funzionalità principali

- Gestione delle categorie di spesa.
- Inserimento delle spese.
- Definizione del budget mensile.
- Visualizzazione dei report sulle spese e sul budget.

## Tecnologie utilizzate

- C++.
- PostgreSQL.
- libpq.
- Visual Studio Code.
- WSL con Ubuntu.
- Git e GitHub.

## Struttura del progetto

- `main.cpp` — contiene il programma C++.
- `database.sql` — contiene la creazione delle tabelle, i dati di esempio e le query dei report.

## Compilazione

Per compilare il programma:

```bash
g++ main.cpp -o main $(pkg-config --cflags --libs libpq)
```

## Esecuzione

Per avviare il programma:

```bash
./main
```

## Connessione al database

Il programma si collega a PostgreSQL utilizzando variabili d'ambiente, evitando di salvare username e password direttamente nel codice sorgente.

Esempio di configurazione del terminale:

```bash
export PGHOST=$(ip route | awk '/default/ {print $3}')
export PGPORT=5432
export PGDATABASE=gestione_spese
export PGUSER=postgres
read -s -p "Password PostgreSQL: " PGPASSWORD; 
export PGPASSWORD; echo
```

## Uso del programma

1. Gestione Categorie — permette di inserire una nuova categoria e verificare se esiste già.
2. Inserisci Spesa — permette di registrare una spesa indicando data, importo, categoria e descrizione facoltativa.
3. Definisci Budget Mensile — permette di impostare o aggiornare il budget mensile associato a una categoria.
4. Visualizza Report — permette di consultare:
   - totale delle spese per categoria;
   - confronto tra spese mensili e budget;
   - elenco completo delle spese ordinato per data.
5. Esci — chiude il programma e termina la connessione al database.

## Preparazione del database

Prima di avviare il programma è necessario creare il database PostgreSQL e poi eseguire il file `database.sql`, che contiene la creazione delle tabelle, i vincoli, i dati di esempio e le query dei report.

Per eseguire lo script SQL:

```bash
psql -d gestione_spese -f database.sql
```
