// A comprehensive example of the MINT language
// This file tests a variety of language features in combination

import math;

struct Vector3 {
    x: f32;
    y: f32;
    z: f32;
}

trait Arithmetic {
    fn add(self, other: Self) -> Self;
    fn subtract(self, other: Self) -> Self;
    fn scale(self, factor: f32) -> Self;
    fn dot(self, other: Self) -> f32;
    fn length(self) -> f32;
}

impl Arithmetic for Vector3 {
    fn add(self, other: Self) -> Self {
        return Vector3 {
            x: self.x + other.x,
            y: self.y + other.y,
            z: self.z + other.z
        };
    }

    fn subtract(self, other: Self) -> Self {
        return Vector3 {
            x: self.x - other.x,
            y: self.y - other.y,
            z: self.z - other.z
        };
    }

    fn scale(self, factor: f32) -> Self {
        return Vector3 {
            x: self.x * factor,
            y: self.y * factor,
            z: self.z * factor
        };
    }

    fn dot(self, other: Self) -> f32 {
        return self.x * other.x + self.y * other.y + self.z * other.z;
    }

    fn length(self) -> f32 {
        let squared = math.sqrt(self.x * self.x + self.y * self.y + self.z * self.z);
        return squared;
    }
}

// Function to calculate the distance between two points
fn distance(a: Vector3, b: Vector3) -> f32 {
    let diff = a.subtract(b);
    return diff.length();
}

// Function to determine if three points are collinear
fn are_collinear(a: Vector3, b: Vector3, c: Vector3) -> bool {
    let ab = b.subtract(a);
    let ac = c.subtract(a);
    
    // Cross product magnitude should be zero for collinear points
    let cross_x = ab.y * ac.z - ab.z * ac.y;
    let cross_y = ab.z * ac.x - ab.x * ac.z;
    let cross_z = ab.x * ac.y - ab.y * ac.x;
    
    let epsilon = 0.0001;
    let magnitude = cross_x * cross_x + cross_y * cross_y + cross_z * cross_z;
    
    return magnitude < epsilon;
}

// Main function to demonstrate usage
fn main() {
    let origin = Vector3 { x: 0.0, y: 0.0, z: 0.0 };
    let point1 = Vector3 { x: 1.0, y: 2.0, z: 3.0 };
    let point2 = Vector3 { x: 2.0, y: 4.0, z: 6.0 };
    
    let dist = distance(origin, point1);
    let collinear = are_collinear(origin, point1, point2);
    
    if collinear {
        return "Points are collinear, distance is " + dist;
    } else {
        return "Points are not collinear, distance is " + dist;
    }
} 