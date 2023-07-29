PROGRAM test8b;
DECLARE
    TYPE
        array1 = ARRAY [0..5] of integer;
        array2 = ARRAY [0..5, 1..12] of real;
    VAR
        arr1 : array1;
        arr2 : array2;
BEGIN
    arr1[5] := arr2[12*5, 8*3];
END.
