: forbidden? ( n -- f )
  dup 0 swap
  begin
    dup 1 > if dup 3 and 0= else false then
  while
    swap 1+ swap
    2 rshift
  repeat
  drop
  2* rshift 7 and 7 =
;

: main
  ." The first 50 forbidden numbers are:" cr
  0 0
  begin
    over 50 <
  while
    dup forbidden? if
      dup 3 .r
      swap 1+ swap
      over 10 mod 0= if cr else space then
    then
    1+
  repeat
  cr
  500 >r
  begin
    r@ 5000000 <=
  while
    dup forbidden? if
      dup r@ > if
        ." There are " over . ." forbidden numbers <= " r@ . cr
        r> 10 * >r
      then
      swap 1+ swap
    then
    1+
  repeat
  rdrop 2drop
;

main
bye
