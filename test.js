var name_0 = _ => {};
name_0 = name_1 =>  {
  var temp_1 = name_1;
  for(var temp_2 = 0; temp_2 < temp_1; temp_2++)  {
    console.log(name_1 == 5);
    if (name_1 == 0)
    {
      return 0;
    }
    if (name_1 == 1)
    {
      return 1;
    }
    else
    {
      return 2;
    }
  }
};
var temp_2 = name_0 (3);
console.log(temp_2);
var name_2 = _ => {};
name_2 = name_1 => name_3 =>  {
  return name_1;
};
var name_4 = _ => {};
name_4 = name_1 => name_3 =>  {
  return name_3;
};
var name_5 = _ => {};
name_5 = name_1 =>  {
  var temp_2 = name_1 (name_4)(name_2);
  return temp_2;
};
