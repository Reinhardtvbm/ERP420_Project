#include "Eigen/Dense"
#include <vector>

class IMU {
    public:
        IMU();
        void pitch_up(float time, float sample_time, float ang_velocity);
        std::vector<Eigen::Vector3f> get_gyroscope();

    private:
        std::vector<Eigen::Vector3f> gyroscope, accelerometer, magnetomer;
};
