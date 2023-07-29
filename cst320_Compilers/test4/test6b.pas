PROGRAM test6b ;
DECLARE
    VAR
        aaa, bbb : integer;

    FUNCTION func1 : integer ;
    DECLARE
        VAR
            aaa, bbb : integer;

        FUNCTION func1a ( VAR aaa : integer ; bbb : integer ) : integer ;
        BEGIN
            WRITE(aaa, bbb);
            func1a := aaa;
        END;

    BEGIN
        aaa := func1a(aaa, bbb);
        func1 := aaa + func1a(1,5);
    END;

BEGIN
   aaa := func1;
   WRITE(aaa, bbb);
END.

