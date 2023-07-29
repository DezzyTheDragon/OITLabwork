{test assigning variables to bariables and changing values}
PROGRAM test2b;
DECLARE
    VAR
        aa : integer;
        bb : integer;
        cc : integer;
BEGIN
    aa := 5;
    bb := aa;
    WRITE(bb);
    cc := bb;
    WRITE(cc);
    cc := 4;
    WRITE(cc);
END.
