#include <iostream>
#include <random>
#include <vector>

#include "./Eigen/Dense"
#include "Eigen/src/Core/Matrix.h"
#include "Eigen/src/Geometry/Quaternion.h"
#include "imu.h"

Eigen::Matrix3f gaussian_niose(float mean_x, float mean_y, float mean_z, float variance_x, float variance_y, float variance_z) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::normal_distribution<float> x_gauss(mean_x, variance_x);
    std::normal_distribution<float> y_gauss(mean_y, variance_y);
    std::normal_distribution<float> z_gauss(mean_z, variance_z);

    return Eigen::Matrix3f {
        { x_gauss(gen), 0, 0 },
        { 0, y_gauss(gen), 0 },
        { 0, 0, z_gauss(gen) }
    };

}

Eigen::Quaternion<double> multiply(Eigen::Quaternion<double> quat1, Eigen::Quaternion<double> quat2) {
    auto q1_v = Eigen::Vector3d(quat1.x(), quat1.y(), quat1.z());
    auto q2_v = Eigen::Vector3d(quat2.x(), quat2.y(), quat2.z());
    double q1_w = quat1.w();
    double q2_w = quat2.w();

    double new_w = q1_w * q2_w - q1_v.dot(q2_v);

    auto new_xyz = q1_w * q2_v + q2_w * q1_v + q1_v.cross(q2_v);  

    return Eigen::Quaternion<double> (new_w, new_xyz[0], new_xyz[1], new_xyz[2]);
}

int main() {
    // Define the original quaternion
    Eigen::Quaternion<double> originalQuaternion(0, 1.0, 0, 0);  // w, x, y, z

    // Define the rotation angle in radians and the rotation axis
    double angle = M_PI / 4.0;  // 45 degrees
    angle /= 2;
    Eigen::Vector3d axis(0.0, 0.0, 1.0);  // Rotation around the z-axis

    // Create a rotation quaternion from the angle and axis
    Eigen::Quaternion<double> rotationQuaternion(Eigen::AngleAxisd(angle, axis));

    // Perform the rotation
    Eigen::Quaternion<double> rotatedQuaternion = rotationQuaternion * originalQuaternion * rotationQuaternion.inverse();

    // Define the reference vector (initially pointing along the z-axis)
    Eigen::Vector3d referenceVector(0.0, 0.0, 1.0);

    // Rotate the reference vector by the quaternion
    Eigen::Vector3d rotatedVector = rotatedQuaternion * referenceVector;

    // Normalize the resulting vector
    rotatedVector.normalize();

    // Print the result
    std::cout << "Original Quaternion: " << originalQuaternion << std::endl;
    std::cout << "Rotated Quaternion: " << rotatedQuaternion << std::endl;
    std::cout << "vector: " << rotatedVector << std::endl;
    
    return 1;
}

