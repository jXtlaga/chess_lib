# Algorytmy szachowe
Zoptymaalizowany algorytm do przeszukiwania wszystkich możliwych posunięć. Jest to przygotowanie biblioteki, żeby w przyszłości wytrenować model uczenia maszynowego do gry w szachy.
Do generowania ruchów figur dalekiego zasięgu (wież, gońców) wykorzystywana jest technika magic bitboard – wydajna metoda mapowania układu przeszkód na indeks tablicy za pomocą specjalnej stałej („magicznej liczby”). Pozwala to na błyskawiczne odczytywanie dostępnych pól z przygotowanej tablicy, znacznie przyspieszając działanie algorytmu.
# przykładowy wynik
```c
         ,,                                 
       `7MM                                 
         MM                                 
 ,p6"bo  MMpMMMb.  .gP"Ya  ,pP"Ybd ,pP"Ybd 
6M'  OO  MM    MM ,M'   Yb 8I   `" 8I   `" 
8M       MM    MM 8M"""""" `YMMMa. `YMMMa. 
YM.    , MM    MM YM.    , L.   I8 L.   I8 
 YMbmd'.JMML  JMML.`Mbmmd' M9mmmP' M9mmmP' 
                                            
                                            
White to move
castling white rights: 3
castling black rights: 3
en passant: 0
half move clock: 0
move number: 0
r . . . k . . r 
p . p p q p b . 
b n . . p n p . 
. . . P N . . . 
. p . . P . . . 
. . N . . Q . p 
P P P B B P P P 
R . . . K . . R 

______________________
depth: 1, vertices count: 48 
checkmate: 0
check: 0
promotion: 0
en passant: 0
castling: 2
______________________
depth: 2, vertices count: 2039 
checkmate: 0
check: 0
promotion: 0
en passant: 1
castling: 91
______________________
depth: 3, vertices count: 97862 
checkmate: 1
check: 0
promotion: 0
en passant: 45
castling: 3162
______________________
depth: 4, vertices count: 4085603 
checkmate: 0
check: 0
promotion: 15172
en passant: 1929
castling: 128013
```
