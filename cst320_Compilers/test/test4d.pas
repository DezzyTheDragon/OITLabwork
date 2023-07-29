{testing multiple parameters}
PROGRAM test4d;
DECLARE
    PROCEDURE foo(x : integer; y : integer);
    BEGIN
        WRITE(x + y);
    END;
BEGIN
    foo(5, 5);
END.
