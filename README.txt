[[ Owl ]]

- a language which only cares about the characters aeiouy
- works with base 6 integers

a register is read depending on the first character
    aei - itself
    o - itself + 1
    u - itself + 2
    y - itself + 3

    a => 0
    e => 1
    i => 2
   oa => 3
   oe => 4
   oi => 5
   oo => 6
   ou => 7
   oy => 8
  uaa => 9
  uae => 10
  uay => 14
  uea => 15
  uey => 20
  uyy => 44
 yaaa => 45
 yyya => 255
 yyye => 256
 yyyi => 257
 yyyo => 258
 yyyu => 259
 yyyy[...] => 260 plus another register value [...]

commands:

  a <reg1> <reg2> - sets <reg1> to the literal value specified by <reg2>
  e <reg1> - outputs <reg1> according to mode
  i <reg> - while(<reg> != 0)
  oa <r1> <r2> <r3>     r1 = r2 + r3
  oe <r1> <r2> <r3>     r1 = r2 - r3
  oi <r1> <r2> <r3>     r1 = r2 * r3
  oo <r1> <r2> <r3>     r1 = r2 / r3
  ou <r1> <r2> <r3>     r1 = r2 % r3
  oy <r1> <r2> <r3>     r1 = r2[r3]
                        (r2 is interpreted as a literal address value)
                        (r3 is an index value)
                        a   i i      ¬ $i = 2
                        a   e uia    ¬ $e = 21 (uia)
                        a   uia oa   ¬ $uia = 3
                        a   uib oy   ¬ $uib = 8
                        a   uic yy   ¬ $uic = 50
                        oya e i      ¬ $a = mem[e + i]
  u - close loop
  y - unassigned
