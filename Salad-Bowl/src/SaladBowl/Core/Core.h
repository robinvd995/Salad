#pragma once

#include "Salad/Core/Log.h"

#define SLD_BOWL_LOG_INFO(...) SLD_CORE_INFO(__VA_ARGS__)
#define SLD_BOWL_LOG_WARN(...) SLD_CORE_INFO(__VA_ARGS__)
#define SLD_BOWL_LOG_ERROR(...) SLD_CORE_INFO(__VA_ARGS__)

#define SLD_BOWL_RESOURCE_OUTPUT_LOCATION ".editor/resource_output.zip"
