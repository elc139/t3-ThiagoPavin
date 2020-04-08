# T3: Scheduling com OpenMP
-------------------------------------

**Nome:** Thiago Bellotti Pavin
**Disciplina:** ELC139 - Programação Paralela

## Cláusula Schedule

No programa foram utilizadas 6 opções do schedule com e sem exclusão mútua e com a definição de chunks e sem definição de chunks.

### Schedule `runtime` 

Com `runtime` o OpenMP determina o schedule pela variavel de controle interno `run-sched-var`, que pode ser definida com o uso da função `omp_set_schedule`.

``` bash
Case 1: using runtime (with Mutex)
BACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBAC
A=20 B=20 C=20 

Case 2: using runtime (without Mutex)
B--B--B--B-AB-ABCABCABCABCAB-AB-ABCABCABCAB-AB-AB--B--BC-B--
A=13 B=20 C=8 
```

### Schedule `auto`

Com `auto` o OpenMP deixa a decisão de Schedule para o compilador em tempo de compilação ou para o sistema em tempo de execução.

``` bash
Case 3: using auto (with Mutex)
AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCC
A=20 B=20 C=20 

Case 4: using auto (without Mutex)
BACBACBACBABCABCABCABCABCABCABCBACBACBACBACBACABCBACBACBAC--
A=19 B=20 C=19 
```

### Schedule `static`

Com `static` o OpenMP divide as iterações em chunks de tamanho chunk-size e distribui nas threads em uma ordem circular.

``` bash
Case 5: using static without chunk (with Mutex)
AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCC
A=20 B=20 C=20 

Case 6: using static without chunk (without Mutex)
ABACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBAC--
A=20 B=19 C=19 

Case 7: using static with chunk 8 (with Mutex)
AAAAAAAABBBBBBBBCCCCCCCCAAAAAAAABBBBBBBBCCCCCCCCAAAAAAAABBBB
A=24 B=20 C=16 

Case 8: using static with chunk 8 (without Mutex)
CACBCABCABABCABCABCABC-BCBACBACBACCBACBACABCAB-ABABABABA-AAA
A=22 B=19 C=16 
```

### Schedule `dynamic`

Com `dynamic` o OpenMP divide as iterações em chunks de tamanho chunk-size e atribui uma para cada thread, quando a thread termina o processamento, ela solicita um chunk novo. Se o chunk-size não é informado, o OpenMP usa 1 como default.

``` bash
Case 9: using dynamic without chunk (with Mutex)
ACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACBACB
A=20 B=20 C=20 

Case 10: using dynamic without chunk (without Mutex)
A-BC--CB-C--C--C--CBACBACB-C-AC-AC--CBAC-BC-BCB-CBACBACBAC--
A=9 B=12 C=19 

Case 11: using dynamic with chunk 8 (with Mutex)
CCCCCCCCBBBBBBBBAAAAAAAACCCCCCCCBBBBBBBBAAAAAAAACCCCCCCCBBBB
A=16 B=20 C=24 

Case 12: using dynamic with chunk 8 (without Mutex)
BCBABCACBACBABCABCABCA-CBBCABCABCABCABCABCABCA-CACACACAC-CCC
A=18 B=16 C=23 
```

### Schedule `guided`

O `guided` é semelhante ao dinamico, a diferença está no tamanho dos chunks, o tamanho de um chunk é proporcional a quantidade de iterações faltando divido pela quantidade de threads. Portanto, o tamanho dos pedaços diminui e o tamanho mínimo de um chunk é definido pelo chunk-size.

``` bash
Case 13: using guided without chunk (with Mutex)
AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBCCCCCCCCCAAAAAABBBBCCCAABC
A=28 B=19 C=13

Case 14: using guided without chunk (without Mutex)
ABACBACABCACBCABCABCABBCABC-CBCBCBCB-CCCCCCACBACBABC--CCA--C
A=13 B=16 C=25

Case 15: using guided with chunk 8 (with Mutex)
CCCCCCCCCCCCCCCCCCCCBBBBBBBBBBBBBBAAAAAAAAACCCCCCCCBBBBBBBBA
A=10 B=22 C=28

Case 16: using guided with chunk 8 (without Mutex)
CBCABCAABCABCABCBACABCABCAB-ABBABABA-AAAAAACBA-BABABABABABA-
A=26 B=20 C=10
```

## Referências

http://jakascorner.com/blog/2016/06/omp-for-scheduling.html

http://jakascorner.com/blog/2016/07/omp-critical.html

https://computing.llnl.gov/tutorials/openMP/

