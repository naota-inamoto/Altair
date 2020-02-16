
boat(0, 1) :- .
boat(1, 0) :- .
boat(1, 1) :- .
boat(0, 2) :- .
boat(2, 0) :- .

safe(m, b)  :- 0 <= m, m <= 3, 0 <= b, b <= 3, safe1(m, b) .

safe1(0, *) :- .
safe1(m, b) :- m  > 0, m >= b .

search(0, 0, 1, s, r) :- write("solved\n", "state = " + s + "\n", "boat = " + r + "\n", "\n") == 0 .

search(m, b, 0, s, r) :-
     boat(m1, b1), m2 == m - m1, b2 == b - b1, safe(m2, b2),
     member((m2, b2), s) == null,
     search(m2, b2, 1, append(s, ((m2, b2))), append(r, ((m1, b1)))) .

search(m, b, 1, s, r) :-
     boat(m1, b1), m2 == m + m1, b2 == b + b1, safe(m2, b2),
     member((m2, b2), s) == null,
     search(m2, b2, 0, append(s, ((m2, b2))), append(r, ((m1, b1)))) .

search(3, 3, 0, ((3, 3)), null) .

