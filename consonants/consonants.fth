: vowel? ( c -- t/f )
  case
    'a' of true endof
    'e' of true endof
    'i' of true endof
    'o' of true endof
    'u' of true endof
    false swap
  endcase ;

: consonant? ( c -- t/f )
  dup 'a' 'z' within if
    vowel? invert
  else
    drop false
  then ;

: consonants ( addr u -- count )
  0 0 { count unique }
  0 do
    dup c@ dup consonant? if
      'a' - 1 swap lshift dup
      unique and if
        2drop 0 unloop exit
      endif
      unique or to unique
      count 1+ to count
    else
      drop
    endif
    1+
  loop
  drop
  count ;

256 constant max-line
create line-buffer max-line 2 + allot

20 constant max-consonant
create word-list-heads max-consonant cells allot
create word-list-tails max-consonant cells allot

: word-list-head ( n -- addr )
  word-list-heads swap cells + ;

: word-list-tail ( n -- addr )
  word-list-tails swap cells + ;

: word-list-init
  word-list-heads max-consonant cells erase
  max-consonant 0 do
    i word-list-head i word-list-tail !
  loop ;

: word-list-append { addr length index -- }
  0 here !
  length here cell+ !
  here index word-list-tail @ !
  here index word-list-tail !
  2 cells allot
  addr here length cmove
  length allot align ;

: word-list-print ( index -- )
  dup word-list-head @
  dup 0= if 2drop exit then
  swap . ." consonants:" cr
  begin
    dup 0 <>
  while
    dup 2 cells +
    over cell+ @
    type cr
    @
  repeat
  drop cr ;

: main
  word-list-init
  0 { fd-in }
  s" unixdict.txt" r/o open-file throw to fd-in
  begin
    line-buffer max-line fd-in read-line throw
  while
    dup 10 > if
      line-buffer swap 2dup consonants 
      dup 0= if
        drop 2drop
      else
        word-list-append  
      then
    else
      drop
    then
  repeat
  drop
  fd-in close-file throw
  max-consonant 0 do
    i word-list-print
  loop ;

main
bye
