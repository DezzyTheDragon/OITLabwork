PROGRAM test2e ;
DECLARE
    VAR
        aaa: integer;

    FUNCTION func1(a : integer) : integer; FORWARD; 

    FUNCTION func1(a : integer) : integer; 
        BEGIN
            WRITE(a);
            func1 := a;
        END;

    FUNCTION func1(a : integer) : integer; 
        BEGIN
            WRITE(a+1);
            func1 := a;
        END;

BEGIN
    aaa := 5;
END.
