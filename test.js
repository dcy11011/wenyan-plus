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
let temp_3 = 3;
name_2 = temp_3;
var temp_4 = name_0 (name_2);
console.log(temp_4);
let temp_5 = 1;
var temp_6 = name_0 (temp_5);
console.log(temp_6);
var name_3 = _ => {};
name_3 = name_1 => name_4 =>  {
  return name_1;
};
var name_5 = _ => {};
name_5 = name_1 => name_4 =>  {
  return name_4;
};
var name_6 = _ => {};
name_6 = name_1 =>  {
  var temp_7 = name_1 (name_5)(name_3);
  return temp_7;
};
