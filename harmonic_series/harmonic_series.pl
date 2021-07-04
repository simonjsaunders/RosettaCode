main:-
    print_harmonic_series(20),
    nl,
    print_nth_harmonic_number(100),
    nl,
    print_first_harmonic_greater_than(10).

print_harmonic_series(N):-
    harmonic_series(N, S),
    writef('First %t harmonic numbers:\n', [N]),
    forall(nth1(I, S, T),
           (Num is numerator(T),
            Denom is denominator(T),
            writef('%3r. %t/%t\n', [I, Num, Denom]))).

print_nth_harmonic_number(N):-
    nth_harmonic_number(N, T),
    Num is numerator(T),
    Denom is denominator(T),
    writef('100th harmonic number: %t/%t\n', [Num, Denom]).

print_first_harmonic_greater_than(N):-
    harmonic_first(H),
    print_first_harmonic_greater_than(1, N, 1, H).

print_first_harmonic_greater_than(N, L, _, _):-
    N > L,
    !.
print_first_harmonic_greater_than(N, L, P, H):-
    H = h(_, T),
    T > N,
    !,
    writef('Position of first term >%3r: %t\n', [N, P]),
    N1 is N + 1,
    P1 is P + 1,
    harmonic_next(H, H1),
    print_first_harmonic_greater_than(N1, L, P1, H1).
print_first_harmonic_greater_than(N, L, P, H):-
    P1 is P + 1,
    harmonic_next(H, H1),
    print_first_harmonic_greater_than(N, L, P1, H1).

harmonic_first(h(1, 1)).

harmonic_next(h(N1, T1), h(N2, T2)):-
    N2 is N1 + 1,
    T2 is T1 + 1 rdiv N2.

harmonic_series(N, S):-
    harmonic_first(H),
    harmonic_series(N, S, H).

harmonic_series(1, [T], h(_, T)):-!.
harmonic_series(N1, [T|Terms], H1):-
    N2 is N1 - 1,
    H1 = h(_, T),
    harmonic_next(H1, H2),
    harmonic_series(N2, Terms, H2).

nth_harmonic_number(N, T):-
    harmonic_first(H),
    nth_harmonic_number(N, T, H).

nth_harmonic_number(1, T, h(_, T)):-!.
nth_harmonic_number(N1, T, H1):-
    N2 is N1 - 1,
    harmonic_next(H1, H2),
    nth_harmonic_number(N2, T, H2).
