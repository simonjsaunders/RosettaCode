: is_prime? ( n -- flag )
  dup 2 < if drop false exit then
  dup 2 mod 0= if 2 = exit then
  dup 3 mod 0= if 3 = exit then
  5
  begin
    2dup dup * >=
  while
    2dup mod 0= if 2drop false exit then
    2 +
    2dup mod 0= if 2drop false exit then
    4 +
  repeat
  2drop true ;

: next_prime_digit_number ( n -- n )
  dup 0= if drop 2 exit then
  dup 10 mod
  dup 2 = if drop 1+ exit then
  dup 3 = if drop 2 + exit then
  5 = if 2 + exit then
  10 / recurse 10 * 2 + ;

: digit_sum ( n -- n )
  0
  begin
    over 0 >
  while
    over 10 mod +
    swap 10 / swap
  repeat
  swap drop ;

: extra_primes ( n -- )
  2
  begin
    2dup >
  while    
    dup is_prime? if dup digit_sum is_prime? if dup . cr then then
    next_prime_digit_number
  repeat
  2drop ;

." Extra primes under 10000:" cr
10000 extra_primes
