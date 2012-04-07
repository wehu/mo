(require-mo "test/test1.mo" 'bb)
(require-mo "test/test1.mo" 'bb1)
(require-mo "test/test1.mo" 'bb2)

(gc)

(display bb/a)
(display bb1/a)

