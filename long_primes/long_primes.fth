: prime? ( n -- ? ) here + c@ 0= ;
: notprime! ( n -- ) here + 1 swap c! ;

: sieve ( n -- )
  here over erase
  0 notprime!
  1 notprime!
  2
  begin
    2dup dup * >
  while
    dup prime? if
      2dup dup * do
        i notprime!
      dup +loop
    then
    1+
  repeat
  2drop ;

: modpow ( n1 n2 n3 -- n )
  dup 1 = if 2drop drop 0 exit then
  1 >r
  rot over mod -rot
  begin
    over 0>
  while
    over 1 and 1 = if
      2 pick r> * over mod >r
    then
    rot dup * over mod -rot
    swap 2/ swap
  repeat
  2drop drop r> ;

: modpow10 ( n1 n2 -- n )
  10 -rot modpow ;

: divide_out ( n1 n2 -- n )
  begin
    2dup mod 0=
  while
    tuck / swap
  repeat drop ;

: long_prime? ( n -- ? )
  dup prime? invert if drop false exit then
  10 over mod 0= if drop false exit then
  dup 1-
  2 >r
  begin
    over r@ dup * >
  while
    r@ prime? if
      dup r@ mod 0= if
        over dup 1- r@ / swap modpow10 1 = if
          2drop rdrop false exit
        then
        r@ divide_out
      then
    then
    r> 1+ >r
  repeat
  rdrop
  dup 1 = if 2drop true exit then
  over 1- swap / swap modpow10 1 <> ;

: next_long_prime ( n -- n )
  begin 2 + dup long_prime? until ;

500    constant limit1
512000 constant limit2

: main
  limit2 1+ sieve
  limit2 limit1 3
  0 >r
  ." Long primes up to " over 1 .r ." :" cr
  begin
    2 pick over >
  while
    next_long_prime
    dup limit1 < if dup . then
    dup 2 pick > if
      over limit1 = if cr then
      ." Number of long primes up to " over 6 .r ." : " r@ 5 .r cr
      swap 2* swap
    then
    r> 1+ >r
  repeat
  2drop drop rdrop ;

main
bye
