(defrule start
  (declare (salience 500))
  ?init <- (initial-fact)
  =>
  (printout t "Witaj" crlf)
  (retract ?init)
  (assert (pytanie jeden))
  )

(defrule  jaka-pora-roku
         ?ask <- (pytanie jeden)
         =>
        (retract ?ask)
        (printout t "Jaka mamy pore roku? (wiosna/lato/jesien/zima)" crlf)
        (assert (pora-roku (read)))
        (assert (pytanie dwa))
   )

(defrule  jaka-wysokosc-przyrostu
         ?ask <- (pytanie dwa)
         =>
        (retract ?ask)
        (printout t "Jak dlugie sa przyrosty?" crlf)
        (assert (pora-roku (read)))
        (assert (pytanie trzy))
   )