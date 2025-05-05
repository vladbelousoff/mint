struct Vec3
{
    x: f32;
    y: f32;
    z: f32;
}

trait Dot
{
    /* Dot product
       trait */
    fn dot(self, rhs: Self) -> f32;
}

impl Dot for Vec3
{
    // Implementation
    fn dot(self, rhs: Self) {
        return self.x * rhs.x + self.y * rhs.y + self.z * rhs.z;
    }
}
