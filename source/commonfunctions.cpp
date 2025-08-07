#include "commonfunctions.h"

extern bool debug;

void putSCCNumAndSize(int n, int *p) {
    int *size = new int[n];
    int *distribution = new int[n + 1];
    for (int u = 0; u < n; ++u) {
        size[u] = 0;
        distribution[u] = 0;
    }
    distribution[n] = 0;
    for (int u = 0; u < n; ++u) {
        int now = u;
        while (p[now] != now) {
            now = p[now];
        }
        size[now]++;
    }
    int num = 0;
    for (int i = 0; i < n; ++i) {
        if (size[i] > 0) {
            num++;
            distribution[size[i]]++;
        }
    }
    std::cout << "SCC num: " << num << std::endl;
    std::cout << "SCC distribution: " << std::endl;
    for (int i = 1; i <= n; ++i) {
        if (distribution[i] > 0) {
            std::cout << "  " << i << ": " << distribution[i] << std::endl;
        }
    }
    delete [] size;
    delete [] distribution;
}

void putProcess(double procedure, unsigned long long time_used) {

    if (debug) {
        std::cout << std::fixed << std::setprecision(3) << "Processing: " << procedure * 100 << "%" \
                << "\t\tTime: " << timeFormatting(time_used).str() \
                << "\t\tEstimate remaining time: " << timeFormatting((1 - procedure) / procedure * time_used).str() << std::endl;
    }
    
}

std::stringstream timeFormatting(unsigned long long microSeconds) {

    std::stringstream ret;
    ret << microSeconds << "μs" << " (";
    unsigned long long seconds = microSeconds / 1000000ull;
    if (seconds < 60) {
        ret << seconds << "s";
    }
    else if (seconds < 3600) {
        ret << seconds / 60ull << "min " << seconds % 60ull << "s";
    }
    else {
        ret << seconds / 3600ull << "h " << seconds % 3600ull / 60ull << "min " << seconds % 60ull << "s";
    }
    ret << ")";
    return ret;

}

unsigned long long currentTime() {

    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::chrono::system_clock::duration duration = now.time_since_epoch();
    unsigned long long microSecondsOfDuration = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    return microSecondsOfDuration;

}