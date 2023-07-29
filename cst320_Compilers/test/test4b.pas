{testing procedure in a procedure}
PROGRAM test4b;
DECLARE
    PROCEDURE fooA(x : integer);
    DECLARE
        PROCEDURE fooB(x : integer);
        BEGIN
            WRITE(x + 5);
        END;
    BEGIN
        fooB(x);
        WRITE(x - 1);
    END;
BEGIN
    fooA(5);
END.
