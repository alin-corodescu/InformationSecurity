Exercitiul 1:

Pentru a genera fisierul criptat am folosit functia din exemplul de pe wikipage:
https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption

Din exemplul de acolo am inlocuit algoritmul cu EVP_aes_128_*. Acest lucru a fost facut si pentru exercitiul propriu zis.

Ca fisiere de input, am folosit ct_ecb si ct_cbc, fiecare criptat in modul in care este si numit. Programul primeste ca parametri: fisier_plain_text fisier_criptat mod_criptare

De asemenea, trebuie sa aiba in folderul curent si dictionarul cuvintelor din limba engleza pe baza carora sa incerce diferite chei de criptare.

Descriere functionare program:

Programul a fost gandit sa poate sa proceseze orice tip de fisiere, nu numai cele text, de orice dimensiuni (nu incarca tot fisierul in memorie).
Acesta va citi cate o parte de BUFFER_SIZE din fisierul criptat, pe care o va decripta folosind API EVP (dar fara sa apeleze functia de finalizare a decriptarii - decat daca chiar este cazul - daca am ajuns la finalul fisierului). Partea de mesaj decriptata este apoi comparata cu corespondenta sa din plain text, utilizand memcmp(), pentru a functiona pe orice tip de date. Daca, la un moment dat, textul decriptat nu se potriveste cu ce ar trebui defapt sa avem sau, daca la finalul decriptarii (cand se apeleaza functia de finalizare), ni se intoarce un cod de eroare (pentru ca nu am folosit cheia potrivita, deci paddingul nu va fi gasit asa cum se astepta algoritmul), atunci trecem la pasul urmator. Altfel, inseamna ca am gasit ceea ce cautam si facem afisarile corespunzatoare.


Programul a fost testat pe fiserele de intrare incluse in folder + pe siruri de caractere scrise direct in cod (in timpul dezvoltarii).


Example to run the program
make
./out pt ct_cbc CBC