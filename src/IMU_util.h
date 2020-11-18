#include <Arduino_LSM9DS1.h>

typedef struct imu_data
{
    float x;
    float y;
    float z;
}imu_data_t;


void IMUSetup()
{

    if (!IMU.begin()) {
		Serial.println("Failed to initialize IMU!");
		while (1);
	}
    //gyroscope settings
    // writeRegister(LSM9DS1_ADDRESS, LSM9DS1_CTRL_REG1_G, 0xD8); // 952 Hz, 2000 dps range, 16 Hz BW

    //accelerometer settings
    // writeRegister(LSM9DS1_ADDRESS, LSM9DS1_CTRL_REG6_XL, 0xD0); // 952 Hz, 4G range

    //magnetometer settings
    //writeRegister(LSM9DS1_ADDRESS_M, LSM9DS1_CTRL_REG1_M, 0xB4); // Temperature compensation enable, medium performance, 20 Hz
    //writeRegister(LSM9DS1_ADDRESS_M, LSM9DS1_CTRL_REG1_M, 0xBC); // Temperature compensation enable, medium performance, 80 Hz
    // writeRegister(LSM9DS1_ADDRESS_M, LSM9DS1_CTRL_REG1_M, 0xFC); // Temperature compensation enable, ultrahigh performance, 80 Hz
}

imu_data_t readGyro(imu_data_t data)
{
    float x, y, z;
    if (IMU.gyroscopeAvailable()) 
    {
        IMU.readGyroscope(x, y, z);
        data.x = x;
        data.y = y;
        data.z = z;
    }
    return data;
}

imu_data_t readAcel(imu_data_t data)
{
    float x, y, z;
    if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(x, y, z);
        data.x = x;
        data.y = y;
        data.z = z;
    }
    return data;
}

imu_data_t readMagnet(imu_data_t data)
{
    float x, y, z;
    if (IMU.magneticFieldAvailable()) {
        IMU.readMagneticField(x, y, z);
        data.x = x;
        data.y = y;
        data.z = z;
    }
    return data;
}

imu_data_t filterIMU(imu_data_t new_data, imu_data_t prev_data, float scaleFactor)
{
    prev_data.x = prev_data.x + scaleFactor * (new_data.x - prev_data.x);
    prev_data.y = prev_data.y + scaleFactor * (new_data.y - prev_data.y);
    prev_data.z = prev_data.z + scaleFactor * (new_data.z - prev_data.z);
    return prev_data;
}

void printIMU(imu_data_t data)
{
    /* Serial.print(data.x);
    Serial.print('\t');
    Serial.print(data.y);
    Serial.print('\t'); 
    Serial.println(data.z); */

    // unsigned long x = (data.x*100), y = (data.y*100), z = (data.z*100);
    char buf[64]; 
    sprintf(buf, "%lu, %lu, %lu", data.x, data.y, data.z);
    sprintf(buf, "%f, %f, %f", data.x, data.y, data.z);
    Serial.println(buf);
}