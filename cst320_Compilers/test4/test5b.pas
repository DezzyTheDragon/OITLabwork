PROGRAM test5b;
DECLARE
    VAR
        count, value : integer;
BEGIN
    count := 0;
    value := 0;

    WHILE count < 10 DO
    BEGIN
        count := count + 1;
        value := value + count;
    END
END.

