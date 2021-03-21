: popcount { n -- u }
  0
  begin
    n 0 <>
  while
    n n 1- and to n
    1+
  repeat ;

: prime? { n -- ? }
  n 2 < if false exit then
  n 2 mod 0= if n 2 = exit then
  n 3 mod 0= if n 3 = exit then
  5
  begin
    dup dup * n <=
  while
    n over mod 0= if drop false exit then
    2 +
    n over mod 0= if drop false exit then
    4 +
  repeat
  drop true ;

: pernicious? ( n -- ? )
  popcount prime? ;

: first_n_pernicious_numbers { n -- }
  ." First " n . ." pernicious numbers:" cr
  1
  begin
    n 0 >
  while
    dup pernicious? if
      dup .
      n 1- to n
    then
    1+
  repeat
  drop cr ;

: pernicious_numbers_between { m n -- }
  ." Pernicious numbers between " m . ." and " n 1 .r ." :" cr
  n 1+ m do
    i pernicious? if i . then
  loop
  cr ;

25 first_n_pernicious_numbers
888888877 888888888 pernicious_numbers_between

bye
