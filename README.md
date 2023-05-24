# Tema PA - LAN PARTY
Programul scris gestioneaza o un fisier cu echipe si comite actiuni asupra acestora. Descrierea programului va gravita in jurul cerintelor propuse pentru rezolvare. Astfel:
- Cerinta 1: ~ modelul utilizat pentru citirea datelor din fisier este o lista care contine numarul total de echipe si echipele cu toate componentele lor. Echipele formeaza o lista simplu inlantuita cu alocare dinmica pentru fiecare grup participant, iar playeri din acestea sunt reprezentati prin intermediul unui vector in care li se aloca dinamic firstname si secondname. Citirea listei se face prin adaugarea elementelor la inceput, astfel ultima echipa din fisier este primma din lista noastra.
- Cerinta 2: ~ Primul pas este construirea numarului limita pe care il poate adimite lista (cea mai mare putere de 2 < numarul de echipe), apoi, incepe stergea elementelor cu punctaj minim pana la atingerea acestui prag. Modul in care functioneaza functia:
    - se parcurge lista, se gaseste primul minim;
    - se mai parcurge lista odata pana la intalnirea minimului si se sterge elementul respectiv;
    - odata cu stergea se scade numarul de echipe din liste;
    - se procedeaza in aceasta maniera pana cand numarul de echipe = numar_limita' 
 La final se afiseaza in fisier noua lista de echipe;
 - Cerinta 3:  Se creeaza coada pentru afisarea primei ture de meciuri, apoi se intra intr-un while in care se apeleaza functii pana la scaderea numarului de echipe la 1. Functiile din bucla :
    - show_match - afiseaza meciurile pe runde si distrubuie echipele in cele doua stive - castigatori si invinsi;
    - in caz de se ajunge la 16 se executa instructiunea in care se salveaza intr-o noua lista cele 8 echipe ramase;
    - se sterge coada pentru a face loc urmatoarelor meciuri intre echipele ramase;
    - se creeaza meciurile, si se sterg cele doua stive pentru a permite distrubutia in urmatoarele runde;
    - se imparte numarul de echipe la 2.
- Cerinta 4: Din lista creeata la 3 se insereaza intr-un BST, constructia acestuia se realizeaza in urma conditiilor impuse si ii se face afisarea in Inordine in fisier;
- Cerinta 5: Elementele din  BST sunt inserate si supuse asupra acelorasi conditii anterioare, urmate fiind apoi de rotiri pana la echilibrarea acestuia. Se creeaza o functie care afiseaza elelmentele, de la inaltimea ceruta, in fisier.
    La final, se elibereaza memoria pentru fiecare structura de date utliziata;