PROGRAM test4;
DECLARE
    VAR
        aa : integer;
        bb : integer;
        cc : real;
    PROCEDURE foo ;
    BEGIN
        NIL;
    END;
BEGIN
    aa := 4;
    cc := bb;
    cc := dd;
    cc := aa;
    foo := 5;
    aa := foo;
END.
