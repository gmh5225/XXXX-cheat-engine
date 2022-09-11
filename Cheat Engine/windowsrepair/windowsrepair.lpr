program windowsrepair;

{$R *.res}


uses windows, registry, classes;

procedure deleteKey(k: string);
var
  l: tstringlist;
  r: tregistry;
  i: integer;
begin
  //log.Add('deleting '+k);
  r:=tregistry.create;
  try
    r.RootKey := HKEY_LOCAL_MACHINE;
    if r.openkey(k, false) then
    begin
      l:=tstringlist.create;
      r.GetKeyNames(l);
      for i:=0 to l.count-1 do
        deletekey(k+'\'+l[i]);

      l.clear;

      r.GetValueNames(l);
      for i:=0 to l.count-1 do
        r.DeleteValue(l[i]);
      l.free;
    end;


  finally
    r.free;
  end;

end;

var
  reg: TRegistry;

begin
  
end.

