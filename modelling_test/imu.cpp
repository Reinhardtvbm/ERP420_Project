#include "imu.h"

IMU::IMU() :
    gyroscope(),
    accelerometer(),
    magnetomer()
{ }

void IMU::pitch_up(float time, float sample_time, float ang_velocity) {
    int steps = time / sample_time;
    
    for (int i = 0; i < steps; ++i) {
        gyroscope.push_back(Eigen::Vector3f {0, ang_velocity, 0});  
    }
}

std::vector<Eigen::Vector3f> IMU::get_gyroscope() {
    return gyroscope;
}
