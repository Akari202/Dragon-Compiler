program ScopingExample(input, output);
var
    x, y: integer;  { Global scope variables }

procedure TestProcedure;
var
    z: integer;  { Procedure-level scope variable }
begin
    z := 10;  { Assign a value to the procedure-level variable }
    x := z + 5;  { Use the global variable x }
end;

begin
    x := 0;  { Assign a value to the global variable x }
    y := 1;  { Assign a value to the global variable y }

    { Nested block demonstrating scoping }
    begin
        x := 3.14;  { Assign a value to the local x }
        y := y + 1;  { Use the global variable y }
    end;

    TestProcedure();  { Call the procedure }

    { Another nested block }
    begin
        z := x + y;  { Use global x and y }
    end;
end.