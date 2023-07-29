{ operator precedence and associativity }
PROGRAM test4b ;
DECLARE
    VAR
        val: integer;
BEGIN 
    { operators }
    val := 1 + 2;
    val := 1 - 2;
    val := 1 * 2;
    val := 1 / 2;
    val := 1 < 2;
    val := 1 = 2;
    val := 1 > 2;
    val := 1 >= 2;
    val := 1 <= 2;
    val := 1 <> 2;
    val := 1 OR 2;
    val := 1 DIV 2;
    val := 1 MOD 2;
    val := 1 AND 2;
    val := NOT (1 < 2);
    val := -1;

    { associativity }
    val := 1 = 2 = 3;
    val := 1 + 2 + 3 - 4 - 5 + 6;
    val := 1 * 2 * 3 / 4 / 5 MOD 6 MOD 7 / 8 * 9;

    { precedence }
    val := 1 = 2 + 3 = 4;
    val := 1 = 2 - 3 = 4;
    val := 1 + 2 * 3 + 4;
    val := 1 - 2 * 3 - 4;
    val := 1 + 2 / 3 + 4;
    val := 1 - 2 / 3 - 4;
END.


