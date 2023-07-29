{Testing basic procedure}
PROGRAM test4;
DECLARE
VAR
    aa : integer;
    PROCEDURE foo(x : integer);
    BEGIN
        WRITE(x);
    END;
BEGIN
    foo(5);
    aa := 10;
    foo(aa);
END.
