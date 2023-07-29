{basic function test}
PROGRAM test5;
DECLARE
VAR
    aa : integer;
    FUNCTION foo(x : integer) : integer;
    DECLARE
    VAR
        aa : integer;
    BEGIN
        aa := x + 5 * 2;
        foo := aa;
    END;
BEGIN
    aa := foo(5);
    WRITE(aa);

END.
