PROGRAM test2a ;
DECLARE
    VAR
        aaa : integer;

    FUNCTION func1(a : integer) : integer; 
        BEGIN
            WRITE(a);
            func1 := a;
        END;

BEGIN
    aaa := 5;
    aaa := func1(aaa);
END.
