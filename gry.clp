(defrule pytanie_ilu_graczy 
        (not(ilu_graczy ?))
        =>
        (printout t "Ilu macie graczy?" crlf)
        (assert (ilu_graczy (read)))
)

(defrule pytanie_internet
        (not(internet ?))
        =>
        (printout t "Czy macie polaczenie z internetem? (tak/nie)" crlf)
        (assert (internet (read)))
)

(defrule pytanie_steam
        (internet tak)
        (not(steam ?))
        =>
        (printout t "Czy macie konta steam? (tak/nie)" crlf)
        (assert (steam (read)))
)

(defrule pytanie_cena
        (steam tak)
        (not(cena ?))
        =>
        (printout t "Ile jestescie gotowi zaplacic za gre?" crlf)
        (assert (cena (read)))
)

(defrule pytanie_czas
        (not(czas ?))
        =>
        (printout t "Czy macie sporo czasu? (tak/nie)" crlf)
        (assert (czas (read)))
)

(defrule pytanie_druzyny
        (not(druzyny ?))
        =>
        (printout t "Wolicie rywalizowac czy dzialac razem? (rywalizacja/razem)" crlf)
        (assert (druzyny (read)))
)

(defrule pytanie_typ
        (not(typ ?))
        =>
        (printout t "Jaki typ gry wam odpowiada? (strzelanka/startegia)" crlf)
        (assert (typ (read)))
)


(defrule babo_violent2
        (druzyny rywalizacja)
        (typ strzelanki)
        =>
        (printout t "----------Propozycja gry: Baboviolent 2(darmowa, po lanie, szczalanka" crlf)
)

(defrule garrys_mode
        (druzyny razem)
        (cena ?x)
        (test (>  ?x 15))
        (steam tak)
        =>
        (printout t "----------Propozycja gry: Garrys Mode" crlf)
)        

(defrule soldat
        (druzyny rywalizacja)
        (typ strzelanki)
        =>
        (printout t "----------Propozycja gry: Soldat 2(darmowa, po lanie, szczalanka, rywalizacja" crlf)
)  

(defrule left4dead2
        (druzyny razem)
        (typ strzelanka)
        (cena ?x)
        (test (>  ?x 10))
        (steam tak)
        =>
        (printout t "----------Propozycja gry: Left 4 Dead 2" crlf)
)

(defrule heroes3
        (typ strategia)
        (czas tak)
        =>
        (printout t "----------Propozycja gry: Heroes 3" crlf)
)

(defrule Insurgency
        (druzyny razem)
        (typ strzelanka)
        (cena ?x)
        (test (>  ?x 20))
        (steam tak)
        =>
        (printout t "----------Propozycja gry: Insurgency" crlf)
)


(defrule StarCraft2
        (typ strategia)
        (internet tak)
        =>
        (printout t "----------Propozycja gry: StartCraft 2" crlf)
)

(defrule littlefigter2
        (druzyny razem)
        (ilu_graczy ?x)
        (test (<  ?x 8))
        =>
        (printout t "----------Propozycja gry: Little Fighter 2" crlf)
)

(defrule QuakeLive
        (typ strzelanka)
        (steam tak)
        =>
        (printout t "----------Propozycja gry: Quake Live" crlf)
)

(defrule Chivalry
        (cena ?x)
        (test (>  ?x 40))
        (steam tak)
        =>
        (printout t "----------Propozycja gry: Chivalry" crlf)
)

(defrule Twierdza
        (czas tak)
        =>
        (printout t "----------Propozycja gry: Twierdza" crlf)
)

(defrule speedrunner
        (druzyny rywalizacja)
        (steam tak)
        (ilu_graczy ?x)
        (test (<  ?x 4))
        =>
        (printout t "----------Propozycja gry: Speed Runner" crlf)
)