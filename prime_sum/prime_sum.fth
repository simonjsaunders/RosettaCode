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

: main
  0 0 { count sum }
  ." count  prime      sum" cr
  100000 prime_sieve
  1000 2 do
    i prime? if
      count 1+ to count
      sum i + to sum
      sum prime? if
        ."   " count 3 .r ."     " i 3 .r ."     " sum 5 .r cr
      then
    then
  loop ;

main
bye
