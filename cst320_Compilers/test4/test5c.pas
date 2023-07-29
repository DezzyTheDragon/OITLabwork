PROGRAM test5c;
DECLARE
    VAR
        aaa, bbb : integer;
BEGIN
    IF aaa THEN
        WRITE(aaa);
    ELSE
        BEGIN
            WHILE aaa < 5 DO
            BEGIN
                aaa := aaa - 1;
                WRITE(aaa, bbb);
            END
        END
END.
