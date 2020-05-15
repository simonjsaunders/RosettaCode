egyptian_divide(Dividend, Divisor, Quotient, Remainder):-
    powers2_multiples(Dividend, Divisor, 1, [1], Powers, [Divisor], Multiples),
    accumulate(Dividend, Powers, Multiples, 0, Quotient, 0, Acc),
    Remainder is Dividend - Acc.

powers2_multiples(Dividend, Divisor, _, Powers, Powers, Multiples, Multiples):-
    2 * Divisor > Dividend,
    !.
powers2_multiples(Dividend, Divisor, Power, P, Powers, M, Multiples):-
    Power2 is 2 * Power,
    Divisor2 is 2 * Divisor,
    powers2_multiples(Dividend, Divisor2, Power2, [Power2|P], Powers,
                      [Divisor2|M], Multiples).

accumulate(_, [], [], Ans, Ans, Acc, Acc):-!.
accumulate(Dividend, [P|Powers], [M|Multiples], Ans1, Answer, Acc1, Acc):-
    Acc1 + M =< Dividend,
    !,
    Acc2 is Acc1 + M,
    Ans2 is Ans1 + P,
    accumulate(Dividend, Powers, Multiples, Ans2, Answer, Acc2, Acc).
accumulate(Dividend, [_|Powers], [_|Multiples], Ans1, Answer, Acc1, Acc):-
    accumulate(Dividend, Powers, Multiples, Ans1, Answer, Acc1, Acc).

test_egyptian_divide(Dividend, Divisor):-
    egyptian_divide(Dividend, Divisor, Quotient, Remainder),
    writef('%w / %w = %w, remainder = %w\n', [Dividend, Divisor,
           Quotient, Remainder]).

main:-
    test_egyptian_divide(580, 34).
