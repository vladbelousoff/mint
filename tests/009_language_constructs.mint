// Struct definition
struct Person {
    name: String;
    age: i32;
    height: f32;
}

// Trait definition
trait Printable {
    fn print(self) -> String;
}

// Implementation
impl Printable for Person {
    fn print(self) -> String {
        return "Name: " + self.name + ", Age: " + self.age;
    }
}

// Function definition
fn calculate_bmi(person: Person) -> f32 {
    let height_squared = person.height * person.height;
    let bmi = person.weight / height_squared;
    return bmi;
}

// Control structures
fn process(value: i32) {
    if (value > 10) {
        return "Greater than 10";
    } else if (value < 0) {
        return "Negative";
    } else {
        return "Between 0 and 10";
    }
}

// Loop
fn factorial(n: i32) -> i32 {
    let result = 1;
    while (n > 0) {
        result *= n;
        n -= 1;
    }
    return result;
}

// Mutability
fn increment(x: mut i32) -> i32 {
    x += 1;
    return x;
} 