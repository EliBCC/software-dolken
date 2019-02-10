#pragma once
#include "measurement.hpp"
#include "actuator.hpp"
#include "can-thread.hpp"

extern MeasurementThread measurementThread;
extern ActuatorThread actuatorThread;
extern CanThread canThread;