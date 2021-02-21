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

: left_truncatable_prime? ( n -- flag )
  dup prime? invert if
    drop false exit
  then
  dup 10
  begin
    dup 3 pick <
  while
    2 pick over mod
    dup 3 pick = if
      2drop 2drop false exit
    then
    dup prime? invert if
      2drop 2drop false exit
    then
    rot drop swap
    10 *
  repeat
  2drop drop true ;

: right_truncatable_prime? ( n -- flag )
  dup prime? invert if
    drop false exit
  then
  10 /
  begin
    dup 0 >
  while
    dup prime? invert if
      drop false exit
    then
    10 /
  repeat
  drop true ;

: max_left_truncatable_prime ( n -- )
  begin
    dup 0 >
  while    
    dup left_truncatable_prime? if . cr exit then
    1-
  repeat drop ;

: max_right_truncatable_prime ( n -- )
  begin
    dup 0 >
  while    
    dup right_truncatable_prime? if . cr exit then
    1-
  repeat drop ;

1000000 constant limit

limit 1+ sieve

." Largest left truncatable prime: "
limit max_left_truncatable_prime

." Largest right truncatable prime: "
limit max_right_truncatable_prime
