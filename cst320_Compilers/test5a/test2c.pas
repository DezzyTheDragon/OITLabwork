PROGRAM test2c ;
DECLARE
    VAR
        aaa : integer;

    FUNCTION func1(a : integer) : integer; FORWARD;

    FUNCTION func1(a : integer) : integer; 
        BEGIN
            WRITE(a);
            func1 := a;
        END;

    FUNCTION func2(a : integer) : integer; 
        BEGIN
            WRITE(a);
            func2 := a;
        END;

BEGIN
    aaa := 5;
    aaa := func1(aaa);
    aaa := func2(aaa);
END.
