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

: next_prime ( u -- u )
  begin
    1+ dup prime?
  until ;

: next_honaker_prime ( u u -- u u )
  begin
    swap next_prime swap 1+
    2dup digit_sum swap digit_sum =
  until ;

: print_pair ( u u -- )
  ." (" 3 .r ." , " 4 .r ." )" ;

: main
  5000000 prime_sieve
  ." First 50 Honaker primes (index, prime):" cr
  0 0 0   \ prime prime-index honaker-index
  begin
    dup 50 <
  while
    -rot next_honaker_prime
    2dup print_pair rot 1+
    dup 5 mod 0= if cr else space then
  repeat
  begin
    dup 10000 <
  while
    -rot next_honaker_prime rot 1+
  repeat
  drop
  cr ." Ten thousandth: " print_pair ;

main cr bye
