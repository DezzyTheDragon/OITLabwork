{multi parameter function test}
PROGRAM test5b;
DECLARE
VAR
    aa : integer;
    FUNCTION add(x : integer; y : integer) : integer;
    BEGIN
        add := x + y;
    END;
BEGIN
    WRITE(add(5, 4));
END.
