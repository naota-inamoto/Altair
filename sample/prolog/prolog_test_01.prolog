
append(null, x, x) :- .
append(cons(h, x), y, cons(h, z)) :- append(x, y, z) .

append((1, 2, 3), (4, 5, 6), z), write("z = " + z + "\n") == 1 .

