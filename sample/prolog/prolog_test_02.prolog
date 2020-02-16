
append(null, x, x) :- .
append(cons(h, x), y, cons(h, z)) :- append(x, y, z) .

append(x, y, (1, 2, 3, 4)), write("x = " + x + "\n", "y = " + y + "\n", "\n") == 0 .

