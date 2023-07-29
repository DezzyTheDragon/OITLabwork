PROGRAM test5a ;
DECLARE
    VAR
        aaa : char;
        bbb : integer;
        ccc : real;

BEGIN
    IF aaa THEN aaa := 1;

    IF bbb+5 THEN
    BEGIN
        bbb := 2;
        ccc := 5.0;
    END

    IF ccc THEN
        aaa := 32;
    ELSE
        aaa := 48;

    IF bbb = 12 THEN
        BEGIN
            bbb := 2;
            ccc := 4.2;
        END
    ELSE
        BEGIN
            bbb := 3;
            ccc := 4.3;
        END
        
END.
