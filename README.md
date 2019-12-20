## Tetris be biți 

I) Functia `main()`

In functia `main()` citim de la `Standard Input` mapa, nr. de miscari(piese) si insusi piesele. Pentru fiecare piesa se introduc 8 miscari pe orizontala(pentru fiecare linie). Exemplu de input:
> 4177266232 2 3084 2 0 0 0 0 0 0 0 3084 2 0 0 0 0 0 0 0

Pentru a afisa miscarile avem nevoie de 2 cicluri: 1 ciclu pentru a parcurge toate piesele si al doilea pentru miscarea fiecarei piese.

Acest ciclu: 
```c
for( i = 0; i < nr_of_pieces; i++ )
```
 parcurge piesele una cite una pina la variabila `nr_of_pieces` primita din input. In el se afla ciclul de miscare a piesei si 2 controale.

In ciclul de miscare a piesei

```c
for( j = 1; j < 9 ; j++ )
```
 se afla functia `moveDown` care misca piesa pe verticala. Functia `moveDown` returneaza harta actualizata cu 1 miscare, sau harta din ciclul anterior in caz ca miscarea data nu este posibila. 

Am creat o variabila numita `success` , pentru a cunoaste daca miscarea in jos s-a efectuat sau nu. In caz ca miscarea in jos s-a efectuat `(success == 1)`, 
afisam harta cu miscarea efectuata. Daca nu s-a efectuat `(success == 0, calea este blocata)` iesim din acest ciclu.

```c
if (j == 1 || (j == 2 && Tab[i][0] > 255)) j == 1 
```
 ciclul de miscare a piesei s-a oprit la prima iteratie; 
 ```c
j == 2 && Tab[i][0] > 255 
```
ciclul de miscare a piesei s-a oprit la a doua iteratie, pe harta a incaput doar jumatate din piesa (piesa este de 2 linii). Ambele cazuri inseamna ca piesa nu a incaput complet, respectiv nu mai citim piese si putem sa iesim din ciclul principal.

```c
if (clearLines(&harta_dynamic, &completed_lines))
```
Aici facem call la functia clearLines, care elimina liniile complete. Daca `clearLines` a returnat alta valoare decit 0 (au fost linii eliminate), afisam harta actualizata.

`harta_static = harta_dynamic;` 

harta_dynamic se modifica la fiecare miscare pe verticala a piesei `harta_stati`c se modifica dupa fiecare asezare a unei piese; Deci dupa incheierea ciclului din interior, actualizam `harta_static`.

II) Functia `afiseazaMapa()` a fost creata cu scopul de a printa harta. Variabila `v` este o masca cu valoarea care are numai un bit setat (initial doar primul bit este setat - reprezentarea binara a variabilei v = 100...000).

Pentru a printa harta avem nevoie de un ciclu. In fiecare iteratie din acest ciclu determinam daca un bit din harta este 1 sau 0. Daca si-logic intre v si harta este egal cu 0, atunci printam `.` (punct), altfel printam diez `#`. 

Apoi facem shift dreapta variabilei v pentru ca in urmatoarea iteratie sa verificam urmatorul bit din harta. Deoarece harta are 8 biti pentru fiecare rind, la fiecare a 8-a iteratie printam rand nou. Astfel am creat harta 8x8 (fiecare rand are cate 8 biti).

III) Functia `moveDown()` Acesta functia misca piesa pe verticala.

```c
piesa_map = *piesa;
```
Piesa_map este o variabila `uint64_t` . Atunci cind salvam piesa in variabila `piesa_map`, creem o harta pe care se afla doar piesa respectiva. Piesa se afla pe randul (randurile) de jos ale hartii. 
> Ex. ........ ........ ........ ........ ........ ....xx.. ....xx..

```c
piesa_map = piesa_map << (7 - nr_rind) * 8; 
```
Piesa trebuie sa cada de sus, de aceea avem nevoie ca harta cu piesa sa reflecte pozitia piesei in functie de numarul de ordine a miscarii acestei piese. Facem operatia `<<` (right bitshift) pentru a muta piesa in partea de sus a hartii. La prima iteratie `(nr_rind == 0)` mutam in bitii in dreapta(piesa in sus) cu valoarea (7 - 0) * 8 == 56. In rezultat avem aceasta harta:

nr_rind == 0
La a doua iteratie mutam cu (7 - 1 ) * 8 == 48:
nr_rind == 1 ....xx.. ........ ........ ....xx.. ........ ....xx.. ........ ........ ........ ........ ........ ........ ........ ........

```c
if( (harta & piesa_map) == 0 )
```

Aici testam cu operatia `&` (si logic) daca piesa nu cade deasupra unei pozitii deja ocupate. Daca pozitia este ocupata, operatia (`harta` & `piesa_map`) va returna o valoare diferita de 0.

a) In cazul ca pozitia nu este ocupata, chemam functia `moveSideways(piesa, horiz_movement, &piesa_map, harta)` pentru a misca piesa pe orizontala.

`harta = harta | piesa_map` . Acesta operatie (sau logic) va aseza piesa modificata de functia `moveSideways` pe harta cu celalalte piese.

b) In cazul in care pozitia este ocupata `(harta & piesa_map > 0)` , reintoarcem piesa cu o pozitie mai sus si actualizam harta la pozitia anterioara: 
```c
piesa_map = piesa_map << (7 - nr_rind + 1) * 8; harta = harta | piesa_map;
```

IV) `moveSideways()`

Acesta functie misca piesa pe orizontala, cite o pozitie, pina la valoarea horiz_movement, sau pina la prima pozitie ocupata. a) horiz_movement < 0 (miscare pe orizontala la stinga).

```c
if ((((*piesa & 255) << 1) >> 8) > 0) break;
```
aici testam ca rindul de sus al unei piese sa incapa in harta. operatia `(*piesa & 255)` lasa doar rindul de jos al piesei. operatia `<<` 1 muta in stinga rindul de jos cu un bit. operatia `>>` 8 muta in dreapta rindul de jos cu 8 biti.

Daca in rezultat avem valoare mai mare ca 0, atunci integem ca rindul de jos nu incape in harta, deci iesim.

```c
else if ((((*piesa << 1) >> 16) == 0) 
```

aici testam ca rindul de sus al unei piese sa incapa in harta. mutam piesa cu un bit la stinga, apoi cu 16 la dreapta. Astfel, daca valoarea va fi mai mare ca 0, va insemna ca piesa nu a incaput in harta dupa aceasta miscare.

```c
(*piesa_map << 1 & harta) == 0 aici testam ca sa nu se suprapuna piesa pe alte piese
```


b) `horiz_movement > 0` (miscare pe orizontala la dreapta).

```c
if (*piesa >> 8 != *piesa >> 9 << 1)
```
aici testam ca rindul de sus al unei piese sa incapa pe harta. operatia `*piesa >> 8` muta 8 biti in dreapta, deci lasam doar rindul de sus. operatia `*piesa >> 9 multa 9 biti in dreapta, deci stergem astfel rindul de jos si un bit din rindul de sus. dupa aceasta cu operatia `<<` 1 , mutam inapoi cu 1 bit la dreapta

In cazul in care partile nu sunt egale, inseamna ca rindul de sus al piesei nu a incaput.

```c
if ((*piesa == (*piesa >> 1) << 1) && (*piesa_map >> 1 & harta) == 0)
``` 
Aici testam ca rindul de jos al unei piese sa incapa pe harta. in cazul in care piesa nu este egala cu piesa mutata in dreapta cu un bit `>> 1`, si apoi returnata in stinga `<< 1`, inseamna ca piesa nu a incaput).

```c
(*piesa_map >> 1 & harta) == 0 
```
aici testam ca sa nu se suprapuna piesa pe alte piese

V) `clearLines()` Aceasta functie sterge liniile complete.

```c
if ((*harta & a_line) == a_line)
```

aceasta operatie de `&` logic verifica daca o linie este completa,

```c
temp = *harta & toPower(i - 1); //aici salvam ceea ce e mai jos de linia completa.
```

```c
if (temp > 0) *harta = ((*harta >> 8 * i) << 8 * i - 8) | temp;
```
 aici stergem linia completa, apoi punem inapoi ceea de era mai jos de aceasta linie.
 
`else { a_line = a_line << 8;`
aici mutam linia linia completa de referita in sus cu ajutorul careia testam daca urmatoarea linie de sus a hartii este completa.




