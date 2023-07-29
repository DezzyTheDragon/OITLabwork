PROGRAM test4;
DECLARE
    TYPE
        longint = integer 8;
        double = real 8;
    VAR
        aaa : char;
        bbb : integer;
        ccc,ddd : real;
        eee : longint;
        fff : double;

BEGIN
   WRITE(aaa);
   WRITE(bbb);
   WRITE(ccc);
   WRITE(ccc,ddd);
   WRITE(eee,fff);
END.
