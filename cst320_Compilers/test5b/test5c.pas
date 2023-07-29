PROGRAM test5c;
DECLARE
    VAR
        aa : char;
        bb : integer;
        cc : real;
    FUNCTION f1 : integer;
        BEGIN
            f1 := 5;
        END;

    FUNCTION f2 : real;
        BEGIN
            f2 := 5.5;
        END;

BEGIN
    aa := 2 * f1();
    bb := 2 * f1();
    bb := 2.5 * f1();
    bb := 2 * f2();
    cc := 2 * f1();
    cc := 2.5 * f1();
    cc := 2 * f2();
END.
