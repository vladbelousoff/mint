// Comprehensive test containing all token types

// Symbols
{ } ( ) [ ] : ; . , ->

// Operators
+ - * / += -= *= /= > < >= <= == != =

// Strings
"Hello, world!" "String with \"escaped quotes\"" "Another string"

// Numbers (covered by 003_numbers.mint but included for completeness)
123 456 0 42 3.14 0.5 42.0 123.456

// Identifiers (covered by 004_identifiers.mint but included for completeness)
myVar _test hello123 camelCase snake_case

// Keywords (covered by 004_identifiers.mint but included for completeness)
struct trait impl for fn return self let mut if else while true false

// Complete example combining all tokens
struct Point {
    x: f32;
    y: f32;
}

fn calculate_distance(a: Point, b: Point) -> f32 {
    let mut dx = b.x - a.x;
    let mut dy = b.y - a.y;
    dx *= dx;
    dy *= dy;
    return dx + dy;
}

fn main() {
    let p1 = Point { x: 0.0, y: 0.0 };
    let p2 = Point { x: 3.0, y: 4.0 };
    
    if calculate_distance(p1, p2) >= 25.0 {
        return true;
    } else {
        return false;
    }
}