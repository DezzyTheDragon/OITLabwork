PROGRAM test2b ;
DECLARE
    CONST
        ccc = 5;
    TYPE 
        aaa = integer 4;
    VAR
        vvv : integer;

    FUNCTION ccc(a : integer) : integer; 
        BEGIN
            WRITE(a);
            ccc := a;
        END;

    FUNCTION vvv(a : integer) : integer; 
        BEGIN
            WRITE(a);
            vvv := a;
        END;

    FUNCTION func1(a : integer) : integer; 
        BEGIN
            WRITE(a);
            func1 := a;
        END;

BEGIN
    vvv := 5;
    vvv := func1(vvv);
END.
