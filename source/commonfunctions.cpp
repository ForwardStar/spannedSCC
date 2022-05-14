#include "commonfunctions.h"

void putProcess(double procedure, int time_used) {

    std::cout << std::fixed << std::setprecision(3) << "Processing: " << procedure * 100 << "%" \
            << "\t\tTime: " << timeFormatting(time_used).str() \
            << "\t\tEstimate remaining time: " << timeFormatting(int((1 - procedure) / procedure * time_used)).str() << std::endl;
    
}

std::stringstream timeFormatting(int seconds) {

    std::stringstream ret;
    if (seconds < 60) {
        ret << seconds << "s";
    }
    else if (seconds < 3600) {
        ret << seconds / 60 << "min " << seconds % 60 << "s";
    }
    else {
        ret << seconds / 3600 << "h " << seconds % 3600 / 60 << "min " << seconds % 60 << "s";
    }
    return ret;

}