{test parenthesies and using variables in math}
PROGRAM test3b;
DECLARE
    VAR
        aa : integer;
        bb : integer;
BEGIN
    aa := 5 + (4 * 3);
    WRITE(aa);
    bb := aa + 10;
    WRITE(bb);
END.
