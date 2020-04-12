sexy_prime_group(1, N, _, [N]):-
    is_prime(N),
    !.
sexy_prime_group(Size, N, Limit, [N|Group]):-
    is_prime(N),
    N1 is N + 6,
    N1 =< Limit,
    S1 is Size - 1,
    sexy_prime_group(S1, N1, Limit, Group).
  
sexy_prime_groups(_, [], _, []):-
    !.
sexy_prime_groups(Size, [P|Primes], Limit, [Group|Groups]):-
    sexy_prime_group(Size, P, Limit, Group),
    !,
    sexy_prime_groups(Size, Primes, Limit, Groups).
sexy_prime_groups(Size, [_|Primes], Limit, Groups):-
    sexy_prime_groups(Size, Primes, Limit, Groups).

print_sexy_prime_groups(Size, Primes, Limit):-
    sexy_prime_groups(Size, Primes, Limit, Groups),
    length(Groups, Len),
    writef('Number of groups of size %t is %t\n', [Size, Len]),
    last_n(Groups, 5, Len, Last, Last_len),
    writef('Last %t groups of size %t: %t\n\n', [Last_len, Size, Last]).

last_n([], _, L, [], L):-!.
last_n([_|List], Max, Length, Last, Last_len):-
    Max < Length,
    !,
    Len1 is Length - 1,
    last_n(List, Max, Len1, Last, Last_len).
last_n([E|List], Max, Length, [E|Last], Last_len):-
    last_n(List, Max, Length, Last, Last_len).

unsexy_prime(P):-
    is_prime(P),
    P1 is P + 6,
    \+is_prime(P1),
    P2 is P - 6,
    \+is_prime(P2).

unsexy_primes(Limit, [2|P]):-
    unsexy_primes(3, Limit, P).

unsexy_primes(From, To, []):-
    From > To,
    !.
unsexy_primes(From, To, [From|Rest]):-
    unsexy_prime(From),
    !,
    Next is From + 2,
    unsexy_primes(Next, To, Rest).
unsexy_primes(From, To, Rest):-
    Next is From + 2,
    unsexy_primes(Next, To, Rest).

main(Limit):-
    Max is Limit + 6,
    find_prime_numbers(Max),
    findall(P, (is_prime(P), P =< Limit), Primes),
    print_sexy_prime_groups(2, Primes, Limit),
    print_sexy_prime_groups(3, Primes, Limit),
    print_sexy_prime_groups(4, Primes, Limit),
    print_sexy_prime_groups(5, Primes, Limit),
    unsexy_primes(Limit, Unsexy),
    length(Unsexy, Count),
    writef('Number of unsexy primes is %t\n', [Count]),
    last_n(Unsexy, 10, Count, Last10, _),
    writef('Last 10 unsexy primes: %t', [Last10]).

main:-
    main(1000035).
