PROGRAM test4a;
DECLARE
VAR
    a : integer;
FUNCTION foo( par1 : integer; par2 : real) : integer;
    DECLARE
    VAR
        a,b : char;
        c,d : integer;
    BEGIN
        par2 := par1 + a + b + c + d;
        foo := c + d;
    END;
BEGIN
    a := foo(a, 3.2*a);
END.

