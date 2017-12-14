Similar cu exercitiul 1, pentru a suporta orice fel de fisier, de orice dimensiune, s-au folosit primitive ce nu tin cont de formatul inputului (utilizeaza bytes raw) si fisierele au fost procesta pe blocuri, de dimensiunea BUFFER_SIZE. Similar cu ex1, EVP API ofera functionalitatea de a face DigestUpdate dupa fiecare bloc citit, iar apoi de e finaliza si, in cazul nostru, a obtine rezumatul hash al fisierului dupa apelul DigestFinal.
Folosind tot primitive de citire de bytes si comparare directa a memoriei, se numara cati bytes identici sunt in fisierele cerute.

Testarea a fost facuta folosind atat fisiere identice (de asteptat, digestul era la fel pt ambele), cat si fisiere care difera (prin unul sau mai multe caractere - rezultatul fiind de obicei, de maxim 1 byte identic)

Exemplu de rulare:
make
./out a b