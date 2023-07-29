PROGRAM test3b ;
DECLARE
    VAR
        aaa: integer;

    FUNCTION func1(a : integer) : integer; 
        BEGIN
            NIL;
        END;

BEGIN
    aaa := func1(aaa);
    aaa := func1(2, 5.3);
    aaa := func1();
END.
