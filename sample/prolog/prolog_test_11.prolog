
hanoi(1, from, to, *  ) :- write(from, " to ", to, "\n") == 2 .
hanoi(n, from, to, via) :- n1 == n - 1,
        hanoi(n1, from, via, to), write(from, " to ", to, "\n") == 2, hanoi(n1, via, to, from) .

hanoi(3, "a", "b", "c") .

