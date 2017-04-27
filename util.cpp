#include "util.h"
#include "stdafx.h"

std::string exec(const std::string cmd) {
    std::string out;
    FILE* pipe = _popen(cmd.c_str(), "r");
    if (pipe) {
        char buff[128] = { 0 };
        while (!feof(pipe)) {
            if (fgets(buff, 128, pipe)) {
                out.append(buff);
            }
        }
        _pclose(pipe);
    }
    return out;
}

std::string get_snum() {
    std::string device = exec("wmic baseboard get manufacturer,serialnumber\r\n");
    transform(device.begin(), device.end(), device.begin(), tolower);
    std::string snumber;
    for (size_t i = 0; i < device.size(); i++) {
        char c = device[i];
        if (c >= '0'&&c <= '9' || c >= 'a'&&c <= 'z') {
            if (i >= 1) {
                char t = device[i - 1];
                if (t < '0' || (t > '9' && t < 'a') || t > 'z') {
                    snumber.push_back('_');
                }
            }
            snumber.push_back(c);
        }
    }
    size_t index = snumber.find("serialnumber") + 12;
    return "sn" + snumber.substr(index);
}

std::string get_uuid() {
    std::string uuid = exec("wmic csproduct get uuid\r\n");
    transform(uuid.begin(), uuid.end(), uuid.begin(), tolower);
    std::smatch results;
    std::regex reg_uuid("[0-9a-f]{8}(-[0-9a-f]{4}){3}-[0-9a-f]{12}");
    if (regex_search(uuid, results, reg_uuid)) {
        return results.str();
    } else {
        return get_snum();
    }
}
