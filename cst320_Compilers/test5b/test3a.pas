PROGRAM test3a ;
DECLARE
    VAR
        aaa: integer;
        notFunc: integer;

    FUNCTION func1(a : integer) : integer; FORWARD; 

    FUNCTION func2(a : integer) : integer; 
        BEGIN
            WRITE(a);
            func1 := a;
        END;

BEGIN
    aaa := func1(aaa);
    aaa := func3(1, 2, 3);
    aaa := notFunc(1);
END.
