PROGRAM test5a ;
DECLARE
    TYPE 
        arrayType = ARRAY [0..5] of integer;
    VAR
        vvv : integer;
        rrr : real;
        arr : arrayType;

BEGIN
    vvv := 5;
    vvv := arr[5.5];
    vvv := arr[rrr];
    arr[rrr] := 7;
    arr[8.2] := 5;
END.
