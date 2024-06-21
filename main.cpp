#include <iostream>
#include <sstream>

#define MAX_WATER_PIPE_TEMP 5
#define MIN_WATER_PIPE_TEMP 0
#define TEMPERATURE_DIFFERENCE 575
#define MIN_INSIDE_TEMP 25
#define MAX_INSIDE_TEMP 30
#define HEATERS_TEMP 22


enum Switchers {
    LIGHTS_INSIDE      = 0b00000001,
    LIGHTS_OUTSIDE     = 0b00000010,
    HEATERS            = 0b00000100,
    WATER_PIPE_HEATING = 0b00001000,
    CONDITIONER        = 0b00010000,
};

std::string timing(std::string time) {
    time[1]++;
    if (time[1] > '9' && time[0] <= '2') {
        time[1] = '0';
        time[0]++;
    }
    if (time[0] == '2' && time[1] == '4') {
        time = "00:00";
    }

    return time;
}

int switchWaterPipe(int temperature, int homeDevicesState) {
    if (temperature < MIN_WATER_PIPE_TEMP) {
        if (!(homeDevicesState & WATER_PIPE_HEATING)) {
            homeDevicesState |= WATER_PIPE_HEATING;
            std::cout << "The water pipe is on!" << std::endl;
        }
    }
    if (temperature >= MAX_WATER_PIPE_TEMP) {
        if (homeDevicesState & WATER_PIPE_HEATING) {
            homeDevicesState &= ~WATER_PIPE_HEATING;
            std::cout << "The water pipe is off!" << std::endl;
        }
    }

    return homeDevicesState;
}

int switchGardenLight(std::string time, std::string movement, int homeDevicesState) {
    if (((time[0] > '1' && time[1] > '6') || time[0] == '0' && time[1] < '5') && movement == "yes") {
        if (!(homeDevicesState & LIGHTS_OUTSIDE)) {
            homeDevicesState |= LIGHTS_OUTSIDE;
            std::cout << "Garden light is on!" << std::endl;
        }
    } else {
        if (homeDevicesState & LIGHTS_OUTSIDE) {
            homeDevicesState &= ~LIGHTS_OUTSIDE;
            std::cout << "Garden light is off!" << std::endl;
        }
    }

    return homeDevicesState;
}

int switchHeaters(int temperatureInside, int homeDevicesState) {
    if (temperatureInside < HEATERS_TEMP) {
        if (!(homeDevicesState & HEATERS)) {
            homeDevicesState |= HEATERS;
            std::cout << "Heaters are on!" << std::endl;
        }
    } else {
        if (homeDevicesState & HEATERS) {
            homeDevicesState &= ~HEATERS;
            std::cout << "Heaters are off!" << std::endl;
        }
    }

    return homeDevicesState;
}

int switchConditioner(int temperatureInside, int homeDevicesState) {
    if (temperatureInside >= MAX_INSIDE_TEMP) {
        if (!(homeDevicesState & CONDITIONER)) {
            homeDevicesState |= CONDITIONER;
            std::cout << "Conditioner is on!" << std::endl;
        }
    }
    if (temperatureInside <= MIN_INSIDE_TEMP) {
        if (homeDevicesState & CONDITIONER) {
            homeDevicesState &= ~CONDITIONER;
            std::cout << "Conditioner is off!" << std::endl;
        }
    }

    return homeDevicesState;
}

int switchLightInside(int homeDeviceState, std::string input) {
    if (input == "yes" || input == "Yes") {
        if (!(homeDeviceState & LIGHTS_INSIDE)) {
            homeDeviceState |= LIGHTS_INSIDE;
        }
    } else {
        if (homeDeviceState & LIGHTS_INSIDE) {
            homeDeviceState &= ~LIGHTS_INSIDE;
        }
    }
    return homeDeviceState;
}

int colorTemperature(std::string time, int homeDeviceState, int counter) {
    int temp = TEMPERATURE_DIFFERENCE * counter;
    if (homeDeviceState & LIGHTS_INSIDE) {
        if ((time[0] >= '1' && time[1] >= '6') && (time[0] <= '2' && time[1] == '0')) {
            std::cout << "Color temperature: " << temp << std::endl;
        } else {
            std::cout << "Color temperature: 5000K" << std::endl;
        }
    }

    return homeDeviceState;
}

int main() {
    int homeDevicesState = 0b0;
    std::string inputState;
    int temperatureInside, temperatureOutside;
    std::string movement, lightInside;
    int timeCounter = 0;
    int colorCounter = 0;
    std::string time = "00:00";

    while (timeCounter < 48) {
        std::cout << "Temperature inside, temperature outside, movement, lights:" << std::endl;
        std::getline(std::cin, inputState);
        std::stringstream buffer_stream(inputState);
        buffer_stream >> temperatureInside >> temperatureOutside >> movement >> lightInside;

        homeDevicesState = switchWaterPipe(temperatureOutside, homeDevicesState);
        homeDevicesState = switchGardenLight(time, movement, homeDevicesState);
        homeDevicesState = switchHeaters(temperatureInside, homeDevicesState);
        homeDevicesState = switchConditioner(temperatureInside, homeDevicesState);
        homeDevicesState = switchLightInside(homeDevicesState, lightInside);
        homeDevicesState = colorTemperature(time, homeDevicesState, colorCounter);

        if (colorCounter == 4) {
            colorCounter = 0;
        }

        timing(time);
        timeCounter++;
        colorCounter++;
    }
    return 0;
}
