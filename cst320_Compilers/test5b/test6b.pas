PROGRAM test5a ;
DECLARE
    TYPE 
        arrayType1 = ARRAY [0..5] of integer;
        arrayType2 = ARRAY [0..5, 5..10] of integer;
    VAR
        arr1 : arrayType1;
        arr2 : arrayType2;
        vvv : integer;

BEGIN
    vvv := arr1[8];
    vvv := arr1[5, 6];
    vvv := arr2[8];
    vvv := arr2[5, 6];
    vvv := arr2[5, 6, 1000];
END.
