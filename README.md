# Temat projektu
Tematem projektu jest rozwiązanie **Problemu pięciu kucharzy** wykorzystując elementy programowania współbieżnego/mechanizmy synchronizacji procesów.

Zadanie oparte jest na [**problemie ucztujących filozofów**](https://pl.wikipedia.org/wiki/Problem_ucztujących_filozofów), przy czym:

- Tutaj kucharz może albo przygotować porcję do zjedzenia, albo skonsumować porcję przygotowaną wcześniej (być może przez innego kucharza).
- Zarówno do skonsumowania porcji, jak i do jej przygotowania kucharz potrzebuje dwóch widelców.
- Porcje przygotowane przez kucharzy gromadzone są na stole, który ma ograniczoną pojemność — K oraz dopuszczalne obciążenie — W.
- Każda przygotowana porcja zajmuje jedno miejsce na stole, a wagi przygotowywanych porcji są zróżnicowane.
- Zawartość stołu jest reprezentowana przez kolejkę komunikatów

# Wykorzystane mechanizmy sychnchronizacji w programie
Program został w całości zaimplementowany w języku **C++**. W związku z tym, wykorzystano następujące *mechanizmy synchronizacji* z odpowiednich (bibliotek):
- *std::thread* (thread) - Inicjalizacja wątków,
- *std::vector* (vector) - Wykorzystany jako odpowiednik kolejki komunikatów,
- *std::mutex* (mutex) - Synchronizacja wątków, kontrola dostępu do zasobów współdzielonych,
- *std::condition_variable* (condition_variable) - Synchronizacja, warunkowe blokowanie/zwalnianie wątków.

Oprócz tego, wykorzystano biblioteki *chrono*, w celu przejrzystości wyświetlanego wyniku programu, oraz *numeric*, która ma pasującą do obektu typu *vector* metodę sumowania elementów (std::reduce).

# Jak skompilować? 
Aby skompilować program, wymagany jest **co najmniej C++17** oraz obsługujący go kompilator.

Należy wkleić kod programu (5cooks.cpp) do środowiska programistycznego (CodeBlocks, Visual Studio), po czym, odpowiedno do środowiska, skompilować go i uruchomić. Alternatywne, gotowy plik należy przekazać jako parametr polecenia kompilacyjnego w konsoli systemowej.

Linijki 11-14 przeznaczone są do dowolnej edycji parametrów wejściowych.

