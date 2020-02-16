
select(x, cons(x, t), t) :- .
select(x, cons(h, t), cons(h, t2)) :- select(x, t, t2).

attack(x, ys) :- attack_sub(x, 1, ys) .
attack_sub(x, n, cons(y, *)) :- x == y + n .
attack_sub(x, n, cons(y, *)) :- x == y - n .
attack_sub(x, n, cons(y, ys)) :- attack_sub(x, n + 1, ys).

queen_sub(null, q, q) :- .
queen_sub(l, safe_qs, q) :- select(x, l, rest_qs),
   not(attack(x, safe_qs)), queen_sub(rest_qs, cons(x, safe_qs), q).

queen_f(q) :- queen_sub((1,2,3,4,5,6,7,8), null, q) .

queen_f(q), write("solution = " + q + "\n") == 0 .

