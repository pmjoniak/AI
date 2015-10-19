(defrule nic-do-roboty
        (pora-roku zima)
        =>
        (printout t "Zima to nie czas na na roboty w ogrodzie" crlf)
        )

(defrule czy-przyciac
        (or (pora-roku wiosna) (pora-roku jesien))
        (wysokosc-przyrostu ?x)
        (test (>  ?x 30))
        =>
         (printout t "Czas najwyzszy przyciac sadzonki" crlf)
        )

(defrule czy-bedzeimy-sadzic-sadzonki
        (or (pora-roku wiosna) (pora-roku jesien))
        =>
        (printout t "Czas wszystkie miejsca gdzie ma rosnac zywoplot sa obsadzone lub masz wystarczajaco sadzonek? (tak/nie)" crlf)
        (assert (nie-potrzeba-sadzonek (read)))
        )

(defrule kupujemy-sadzonki
        (nie-potrzeba-sadzonek nie)
        =>
        (printout t "Kupujemy sadzonki czy budzetowo? (kupuje/ukorzeniam)" crlf)
        (assert (nowe-sadzonki (read)))
        )

(defrule ukorzeniamy-sadzonki
        (nowe-sadzonki ukorzeniam)
        =>
        (printout t "Sadzimy szkolke" crlf)
        )

(defrule sadzimy-sadzonki
        (or (pora-roku wiosna) (pora-roku jesien))
        (not (sadzonki za-malo))
        =>
        (printout t "Bedziemy sadzic sadzonki" crlf)
        (assert (sadzimy nowe sadzonki))
        )

(defrule nalezy-przekopac
        (sadzimy nowe sadzonki)
        (or (stan-ziemi twarda) (chwasty duzo))
        =>
        (printout t "Nalezy przekopac przed sadzeniem nowych sadzonek" crlf)
        )

(defrule podlewamy
        (padalo-temu ?x)
        (test (> ?x 10))
        =>
        (printout t "Podlej posadzone sadzonki i szkolke" crlf)
        )


