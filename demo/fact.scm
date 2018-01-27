(define (fact n)
  (if (= n 0) 
    1
    (* n (fact (- n 1)))))

(display "float fact_table[] = \n{\n")

(do ((i 0 (+ i 1)))
  ((= i 20) 'done)
  (display "\t")
  (display (exact->inexact (fact i)))
  (if (= (modulo i 5) 0)
    (display ",\n")
    (display ", "))
  )

(display "};\n")


