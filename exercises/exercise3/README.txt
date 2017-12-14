Pentru exercitiul 3, am folosit limbajul C/C++, si mediul de dezvoltare CLion. Libraria tot de OpenSSL, ca la exercitiile anterioare (doar ca am utilizat direct primitivele AES in loc de EVP API).

Observatie initiala:
Apelurile primitevelor read() si write() sunt "invelite" in bucle while deoarece, conform manualui, acestea pot returna si mai putini bytes decat au fost ceruti. Pentru un byte acest lucru nu se poate intampla, dar este o practica buna pentru uzul general.

Mod de functionare a programului: 
In fisierul ex3.h avem functii ajutatoare (implementate direct in ex3.h din comoditate - nu este deloc o practica buna).
Aceste functii ne ajuta sa ne folosim de primitevel AES pentru a cripta si decripta texte. Pe langa functiile de criptare, avem si o functie care ne face xor intre bytes a doua blocuri, si o functie care sa afiseze in format readable o cheie.

A va trebui rulat primul, intrucat el genereaza toate fisierele necesare comunicarii intre procese (sub forma de named pipes - acestea au avantajul ca apelul open() asteapta pana cand alt proces va deschide acel pipe cu flagul read (daca noi am deschis cu write) si vice-versa). Dupa generare si deschiderea file descriptorilor, incepe comunicarea prorpiu zisa, exact sub forma in care este descrisa in enunt.

KM: Key managerul are o implementare simpla, ce se foloseste doar de functiile de criptare din ex3.h si de comunicatie prin named pipes folosind primitivele read si write.

Functionarea lui A si B este exact cum este prevazut in enunt. Pentru transmiterea fisierului am folosit urmatoarele tehnici:
La fel ca la exercitiile anterioare, citirea si scrierea se face pe blocuri, pentru a nu incarca tot fisierul in memorie deoedata. Pe langa aceasta, desi nu este necesar pentru fisierele text (ce e dupa null terminator nu conteaza oricum), am implementat si o modalitate de a face padding pentru blocul final transmis (aceasta modalitate este inspirata (daca nu chiar identica) cu PKCS - pentru ultimul bloc transmis, cel putin un byte va fi de padding. Acesta va spune cati bytes de padding exista in blocul curent - cu exceptia ultimului, pentru simplitate (de exemplu un bloc cu 15 bytes de date va avea byteul de padding  = 0) , pentru a sti cu exactitate care este zona de date)
De partea cealalta, la B, se realizeaza unpaddingul atunci cand se ajunge la eof.

Pentru modul de operare ECB tot ce trebuie sa folosim sunt primitivele AES. In schimb, pentru CBC, am recurs la un pointer previousBlock, care este != NULL atunci cand este folosit. Acesta va pointa intotdeauna spre blocul anterior de cripto text ce va trebui utilizat in decriptarea blocului curent. (initial iv)

Mentiune: Programele vor afisa si mesaje de debug pe care nu le-am mai scos pe motiv ca sunt utile pentru prezentarea acestuia.

exemplu de rulare:
make a b km
./a C       -> pentru modul C = CBC, E = ECB
./b
./km



