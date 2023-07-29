PROGRAM test5d ;
DECLARE
    TYPE 
        array1Type = ARRAY [0..5] of integer;
        array2Type = ARRAY [0..5] of integer;
    VAR
        vvv : integer;
        arr1 : array1Type;
        arr1b : array1Type;
        arr2 : array2Type;

BEGIN
    vvv := arr1;
    arr1 := arr1b;
    arr2 := arr1;
END.
