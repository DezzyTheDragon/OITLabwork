PROGRAM test6a ;
DECLARE
    TYPE 
        arrayType = ARRAY [0..5] of integer;
    VAR
        vvv : integer;
        arr : arrayType;

BEGIN
    vvv := 5;
    vvv := arr[5];
    vvv := vvv[3];
    arr[vvv] := 7;
    vvv[5] := 3;
END.
