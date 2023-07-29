PROGRAM test2d ;
DECLARE
    VAR
        aaa: integer;

    FUNCTION func1(a : integer) : integer; FORWARD; 

    FUNCTION func1(a : integer) : real; 
        BEGIN
            WRITE(a);
            func1 := a;
        END;

    FUNCTION func2(a : integer) : integer; 
        BEGIN
            WRITE(a);
            func1 := a;
        END;

    FUNCTION func3(a, b: integer) : integer; FORWARD;

    FUNCTION func3(a: integer; b: real; c: integer) : integer;
        BEGIN
            WRITE(a, b, c);
            func3 := a+b;
        END;

BEGIN
    aaa := 5;
    aaa := func1(aaa);
    aaa := func2(aaa);
    aaa := func3(a, b, c);
END.
