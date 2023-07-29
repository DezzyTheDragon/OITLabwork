PROGRAM test7;
DECLARE
    TYPE 
        record1 = RECORD
            field1, field2 : integer ;
            field3 : real ;
        END ;

    VAR
        var1 : record1 ;

BEGIN
    var1.field1 := 5;
    var1.field2 := 3;
END.
