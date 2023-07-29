PROGRAM test1;
DECLARE
    TYPE
        longint = integer 8;
        double = real 8;
    VAR
        aa : char;
        bb : integer;
        cc : real;
BEGIN
    aa := 4;
    aa := 5000;
    aa := cc;
    bb := aa;
    bb := 5;
    bb := 7000;
    bb := 3.5;
    bb := cc;
    cc := aa;
    cc := bb;
    cc := 3.5;

    IF 1 THEN
    DECLARE
        VAR
            dd : longint;
            ee : double;
    BEGIN
        aa := dd;
        aa := ee;
        bb := dd;
        bb := ee;
        cc := dd;
        cc := ee;
        dd := aa;
        dd := bb;
        dd := cc;
        dd := dd;
        dd := ee;
        ee := aa;
        ee := bb;
        ee := cc;
        ee := dd;
        ee := ee;
    END
END.
