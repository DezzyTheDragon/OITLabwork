PROGRAM test4;
DECLARE
    VAR
        aa : integer;
    FUNCTION foo : integer ;
    BEGIN
        foo := 5;
    END;
BEGIN
    aa := foo;
    foo ;
END.
