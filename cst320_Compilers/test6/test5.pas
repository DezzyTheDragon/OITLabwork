PROGRAM test5;
DECLARE
    TYPE
        short = integer 2;
        long = integer 8;
        double = real 8;
        prime = integer 13;
    VAR
        aa : char;
        bb : short;
        cc : integer;
        dd : long;
        ee : real;
        ff : double;
        gg : prime;
        hh : integer;

    PROCEDURE foo(aa : short; bb : long; cc : double; dd : prime; ee : integer);
    BEGIN
        aa := 0;
        bb := 4;
        cc := 12;
        dd := 20;
        ee := 36;
    END;
BEGIN
    aa := 0;
    bb := 4;
    cc := 8;
    dd := 12;
    ee := 20;
    ff := 24;
    gg := 32;
    hh := 48;
END.
