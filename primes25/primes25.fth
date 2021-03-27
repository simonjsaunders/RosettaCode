: prime? ( n -- ? ) here + c@ 0= ;
: notprime! ( n -- ) here + 1 swap c! ;

: prime_sieve { n -- }
  here n erase
  0 notprime!
  1 notprime!
  n 4 > if
    n 4 do i notprime! 2 +loop
  then
  3
  begin
    dup dup * n <
  while
    dup prime? if
      n over dup * do
        i notprime!
      dup 2* +loop
    then
    2 +
  repeat
  drop ;

: digit_sum ( u -- u )
  dup 10 < if exit then
  10 /mod recurse + ;

: prime25? { p -- ? }
  p prime? if
    p digit_sum 25 =
  else
    false
  then ;  

: .prime25 { n -- }
  ." Primes < " n . ." whose digits sum to 25:" cr
  n prime_sieve
  0 0
  begin
    dup n <
  while
    dup prime25? if
      dup 5 .r
      swap 1+ dup 10 mod 0= if cr then swap
    then
    1+
  repeat
  drop cr ." Count: " . cr ;

5000 .prime25
bye
