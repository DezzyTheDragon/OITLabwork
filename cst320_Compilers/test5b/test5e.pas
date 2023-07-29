PROGRAM test5e;
DECLARE
    VAR
        bb : integer;
        cc : real;
    FUNCTION f1 (arg: integer) : integer;
        BEGIN
            f1 := 5.5;
        END;

BEGIN
    bb := f1(1);
    bb := f1(1000);
    bb := f1(1.2);
    bb := f1(bb);
    bb := f1(cc);
END.
