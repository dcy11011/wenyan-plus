let temp_1 = [];
name_0 = temp_1;
var temp_2 = (temp_1 = Array.from(temp_1).concat(1));
let temp_3 = 3;
name_1 = temp_3;
var temp_4 = 20;
for(var temp_5 = 0; temp_5 < temp_4; temp_5++)
{
  let temp_6 = name_1;
  let temp_7 = 13 * temp_6 * temp_6 + 111;
  let temp_8 = temp_7 % 233;
  name_1 = temp_8;
  var temp_9 = (name_0 = Array.from(name_0).concat(temp_8));
}
var name_2 = _ => {};
name_2 = name_3 => name_4 =>  {
  let temp_10 = [];
  name_5 = temp_10;
  for(var name_6 of name_3)
  {
    if (name_6 > name_4)
    {
      var temp_11 = (name_5 = Array.from(name_5).concat(name_6));
    }
  }
  return name_5;
};
var name_7 = _ => {};
name_7 = name_3 => name_4 =>  {
  let temp_12 = [];
  name_5 = temp_12;
  for(var name_6 of name_3)
  {
    if (name_6 > name_4)
    {
      let temp_13 = Infinity;
    }
    else
    {
      var temp_14 = (name_5 = Array.from(name_5).concat(name_6));
    }
  }
  return name_5;
};
let temp_15 = 100;
name_1 = temp_15;
let temp_16 = name_0;
var temp_17 = name_2 (temp_16)(name_1);
console.log(temp_17);
let temp_18 = name_0;
var temp_19 = name_7 (temp_18)(name_1);
console.log(temp_19);
let temp_20 = (name_0).length;
console.log(temp_20);
var name_8 = _ => {};
name_8 = name_9 =>  {
  let temp_21 = 0;
  name_10 = temp_21;
  let temp_22 = 1;
  name_11 = temp_22;
  while(true)  {
    if (name_11 > name_9)
    {
      break;
    }
    let temp_23 = name_10 + name_11;
    name_10 = temp_23;
    let temp_24 = name_11 + 1;
    name_11 = temp_24;
  }
  let temp_25 = name_10;
  console.log(temp_25);
  return temp_25;
};
var temp_26 = name_8 (100);
let temp_27 = temp_26 % 233;
console.log(temp_27);
var name_12 = _ => {};
name_12 = name_9 =>  {
  var temp_28 = name_9;
  for(var temp_29 = 0; temp_29 < temp_28; temp_29++)
  {
    console.log(name_9 == 5);
    if (name_9 == 0)
    {
      return 0;
    }
    if (name_9 == 1)
    {
      return 1;
    }
    else
    {
      return 2;
    }
  }
};
let temp_30 = 3;
name_13 = temp_30;
var temp_31 = name_12 (name_13);
console.log(temp_31);
let temp_32 = 1;
var temp_33 = name_12 (temp_32);
console.log(temp_33);
let temp_34 = 3 * 8 + 100000;
console.log(temp_34);
let temp_35 = temp_34 / 88;
let temp_36 = Math.round(temp_35);
console.log(temp_36);
let temp_37 = temp_36 % 233;
console.log(temp_37);
var name_14 = _ => {};
name_14 = name_9 => name_15 =>  {
  return name_9;
};
var name_16 = _ => {};
name_16 = name_9 => name_15 =>  {
  return name_15;
};
var name_17 = _ => {};
name_17 = name_9 =>  {
  var temp_38 = name_9 (name_16)(name_14);
  return temp_38;
};
