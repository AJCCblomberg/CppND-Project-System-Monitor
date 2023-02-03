#include <string>
#include <math.h>
#include <iomanip>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
//string Format::ElapsedTime(long seconds[[maybe_unused]]) {
string Format::ElapsedTime(long seconds) {    
    double HH{0};
    std::string stringHH;
    double MM{0};
    std::string stringMM;
    double SS{0};
    std::string stringSS;

    // Create output string stream
    std::ostringstream streamObj;
    // Set fixed point notation, width and fill
    streamObj << std::fixed << std::setw(2) << std::setfill('0');
    // Set precision to 0 digits
    streamObj << std::setprecision(0);

    HH = floor(seconds/3600.0);
    if(HH < 10) {
        stringHH = "0" + std::to_string((long) HH);
    } else {
        stringHH = std::to_string((long) HH);
    }

    MM = floor(((seconds/3600.0) - HH) * 60.0);
    if(MM < 10) {
        stringMM = "0" + std::to_string((long) MM);
    } else {
        stringMM = std::to_string((long) MM);
    }

    SS = ((seconds/3600.0 - HH) * 60.0 - MM) * 60;
    // Add double to stream
    streamObj << SS;
    // Get string from output string stream
    stringSS = streamObj.str();

    return string(stringHH + ":" + stringMM + ":" + stringSS); 
    }