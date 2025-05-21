// Testing symbols and operators
{} () [] ; : -> 
> < >= <= == != =

// Testing strings
"Hello, world!" "String with \"escaped quotes\"" "Multi
line string"

// Testing a complete example
struct Point {
    x: f32;
    y: f32;
}

fn calculate(a: Point, b: Point) -> f32 {
    let distance = (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y);
    return distance;
} 