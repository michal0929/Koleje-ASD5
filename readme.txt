W pewnym postepowym kraju od stuleci rozwija sie transport kolejowy. Poczatkowo polaczenia
kolejowe obejmowaly tylko najwieksze i najszybciej rozwijajace sie miasta, a predkosc pociagów
pozostawiala wiele do zyczenia. Wedlug opowiesci starszych mozna bylo wyskoczyc na grzyby,
gdy pociag przejezdzal przez las, by po grzybobraniu zalapac sie jeszcze do ostatniego wagonu. Z
czasem predkosci pociagów rosly, by w koncu doprowadzic do likwidacji konwencjonalnych
przejazdów kolejowych (ze wzgledów bezpieczenstwa). Zmienil sie tez wyglad pociagów –
poczatkowo przypominaly fabryki z kominem, a obecnie blizej im do filmów SF. W kraju tym nie
toleruje sie obnizenia predkosci pociagów i po modernizacji danego odcinka srednia predkosc na
nim moze jedynie wzrosnac. Co wiecej, postój na kazdej stacji wynosi 5 minut i nigdy nie ma
opóznien, a z zadnego miasta nie wychodzi wiecej niz 50 polaczen. W bazie danych kolei istnieje
cale archiwum dotyczace budowy i modernizacji odcinków laczacych poszczególne miasta.
Zlecono napisanie oprogramowania do analizy tych danych. Twoim zadaniem jest ustalenie, kiedy
czas przejazdu pociagiem miedzy zadana para miast skrócil sie do zadanego poziomu.
Wejscie:
W pierwszej linii wejscia podane sa trzy liczby calkowite n, m i z (1<=n<=10000,1<=m<=100000,
1<=z<=10) oznaczajace odpowiednio liczbe miast, liczbe budowanych/modernizowanych
odcinków oraz liczbe zapytan do programu. W kolejnych m liniach znajduja sie chronologiczne
informacje dotyczace budowy. W kazdej z tych linii podane sa: data (format rrrr-mm-dd), nastepnie
znak 'm' lub 'b' w zaleznosci od tego czy chodzi o budowe (polaczenie wczesniej nie istnialo) czy o
modernizacje istniejacego juz odcinka, nastepnie para róznych miast m1, m2 (1<=m1, m2<=n)
miedzy którymi budowane/modernizowane jest polaczenie, a nastepnie srednia predkosc v (w km/h)
na tym odcinku (1<=v<=500) oraz (w przypadku budowy) równiez dlugosc d (w km) nowobudowanego
polaczenia (1<=d<=1000). v i d sa liczbami calkowitymi i 60*d mod v = 0. W
kolejnych z liniach znajduja sie zapytania zlozone z liczb m1, m2, c (1<=m1, m2<=n,
1<=c<=10000) oznaczajacych pare miast oraz maks. czas polaczenia miedzy nimi (w minutach).
Wyjscie:
W z liniach wyjscia nalezy odpowiedziec na zapytania: kiedy po raz pierwszy udalo sie osiagnac
czas najszybszego polaczenia (posredniego lub bezposredniego) miedzy danymi miastami nie
przekraczajacy zadanego limitu. Jesli taki czas nie zostal dotad osiagniety, nalezy wypisac 'NIE'.
Przyklad:
Wejscie:
5 10 3 //5 miast, 10 polaczen, 3 zapytania
1900-05-30 b 1 2 30 60 //budowane polaczenie miedzy miastami 1 i 2, v=30 km/h, d=60 km
1900-07-15 b 2 5 40 120 //itd.
1905-04-19 b 1 4 35 70
1910-06-03 b 4 5 50 100
1950-10-25 m 2 5 72 //modernizowane polaczenie miedzy miastami 2 i 5: nowe v=72 km/h
1955-01-31 b 1 3 60 90
1990-12-12 b 3 5 180 90
2000-09-20 b 3 4 240 60
2005-06-14 m 1 4 280 //itd.
2008-03-07 b 2 3 150 50
1 5 230 //Kiedy pomiedzy miastami 1 i 5 osiagnieto czas podrózy maks. 230 minut
1 5 75 //itd.
2 4 35
Wyjscie:
1950-10-25 //od 1950-10-25 miedzy miastami 1 i 5 dalo sie przejechac w 225 minut (<=230)
2005-06-14 //od 2005-06-14 miedzy miastami 1 i 5 dalo sie przejechac w 70 minut (<=75)
NIE //najszybsze polaczenie miedzy 2 i 4 wymaga obecnie 40 minut