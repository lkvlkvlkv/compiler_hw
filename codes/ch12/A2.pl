PROGRAM A2;
  VAR
    m, n;
  PROCEDURE B;
    VAR
      n, k;
    BEGIN
      n:=2;
      WRITE(n);
    END;
  BEGIN
    n:=1;
    WRITE(n);
    CALL B;
    WRITE(k);
  END.
