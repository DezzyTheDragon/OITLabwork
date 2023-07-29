{testing scope}
PROGRAM test4c;
DECLARE
VAR
    aa : integer;
    PROCEDURE fooA(x : integer);
    DECLARE
    VAR
        aa : integer;
    BEGIN
        aa := x + 10;
        WRITE(aa);
    END;
    PROCEDURE fooB(x : integer);
    BEGIN
        WRITE(x - 5);
    END;
BEGIN
    aa := 10;
    WRITE(aa);
    fooA(aa);
    WRITE(aa);
    fooB(aa);
END.
