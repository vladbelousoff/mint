// Testing dots in member access
object.property
a.b.c.d

// Testing dots in numbers vs standalone dots
123.456 .789 12. .

// Testing commas in argument lists
function(arg1, arg2, arg3)
array[1, 2, 3, 4]

// Combined test
let points = [Point{x: 1.0, y: 2.0}, Point{x: 3.5, y: 4.2}];
calculate(points[0], points[1]); 