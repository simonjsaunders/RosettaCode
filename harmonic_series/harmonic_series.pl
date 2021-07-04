main:-
    print_harmonic_series(20),
    nl,
    print_nth_harmonic_number(100),
    nl,
    print_first_harmonic_greater_than(10).

print_harmonic_series(N):-
    harmonic_series(N, S),
    writef('First %t harmonic numbers:\n', [N]),
    forall(member(h(I, T), S),
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
    print_first_harmonic_greater_than(1, N, H).

print_first_harmonic_greater_than(N, L, _):-
    N > L,
    !.
print_first_harmonic_greater_than(N, L, H):-
    H = h(P, T),
    T > N,
    !,
    writef('Position of first term >%3r: %t\n', [N, P]),
    N1 is N + 1,
    harmonic_next(H, H1),
    print_first_harmonic_greater_than(N1, L, H1).
print_first_harmonic_greater_than(N, L, H):-
    harmonic_next(H, H1),
    print_first_harmonic_greater_than(N, L, H1).

harmonic_first(h(1, 1)).

harmonic_next(h(N1, T1), h(N2, T2)):-
    N2 is N1 + 1,
    T2 is T1 + 1 rdiv N2.

harmonic_series(N, S):-
    harmonic_first(H),
    harmonic_series(N, S, H).

harmonic_series(N, [H], H):-
    H = h(N, _),
    !.
harmonic_series(N, [H1|Terms], H1):-
    harmonic_next(H1, H2),
    harmonic_series(N, Terms, H2).

nth_harmonic_number(N, T):-
    harmonic_first(H),
    nth_harmonic_number(N, T, H).

nth_harmonic_number(N, T, h(N, T)):-!.
nth_harmonic_number(N, T, H1):-
    harmonic_next(H1, H2),
    nth_harmonic_number(N, T, H2).
