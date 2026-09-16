CREATE TABLE categorie (
    id_categoria INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    nome VARCHAR(100) NOT NULL UNIQUE
);

CREATE TABLE spese (
    id_spesa INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    id_categoria INTEGER NOT NULL,
    data_spesa DATE NOT NULL,
    importo NUMERIC(10, 2) NOT NULL CHECK (importo > 0),
    descrizione TEXT,
FOREIGN KEY (id_categoria) REFERENCES categorie(id_categoria)
);

CREATE TABLE budget (
    id_budget INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    id_categoria INTEGER NOT NULL,
    mese CHAR(7) NOT NULL,
    importo_budget NUMERIC(10,2) NOT NULL CHECK (importo_budget > 0),
FOREIGN KEY (id_categoria) REFERENCES categorie(id_categoria),
UNIQUE (id_categoria, mese)
);

INSERT INTO categorie (nome)
VALUES 
    ('Alimentari'),
    ('Trasporti'),
    ('Svago');

INSERT INTO spese (id_categoria, data_spesa, importo, descrizione)
VALUES
    (1, '2026-09-16', 25.00, 'Spesa supermercato'),
    (2, '2026-09-16', 15.00, 'Biglietto aereo'),
    (3, '2026-09-16', 12.00, 'Biglietto cinema');

INSERT INTO budget (id_categoria, mese, importo_budget)
VALUES
    (1, '2026-09', 300.00),
    (2, '2026-09', 150.00),
    (3, '2026-09', 100.00);

SELECT categorie.nome AS categoria,
       SUM(spese.importo) AS totale_speso
FROM spese
JOIN categorie
    ON spese.id_categoria = categorie.id_categoria
GROUP BY categorie.nome;

SELECT TO_CHAR(spese.data_spesa, 'YYYY-MM') AS mese,
       categorie.nome AS categoria,
       budget.importo_budget AS budget,
       SUM(spese.importo) AS speso
FROM spese
JOIN categorie
    ON spese.id_categoria = categorie.id_categoria
JOIN budget
    ON spese.id_categoria = budget.id_categoria
    AND TO_CHAR(spese.data_spesa, 'YYYY-MM') = budget.mese
GROUP BY TO_CHAR(spese.data_spesa, 'YYYY-MM'),
         categorie.nome,
         budget.importo_budget
ORDER BY mese, categoria;

SELECT spese.data_spesa,
       categorie.nome AS categoria,
       spese.importo,
       spese.descrizione
FROM spese
JOIN categorie
    ON spese.id_categoria = categorie.id_categoria
ORDER BY spese.data_spesa;
