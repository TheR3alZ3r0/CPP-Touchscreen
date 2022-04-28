#define MAIN_1
#include <iostream>
#include "Calibration.h"
#include "Package.h"
#include "simple-serial-port/SimpleSerial.h"

void bubbleSort(int arr[], int elements)
{
	for (int x = 0; x < elements - 1; x++) {
		for (int y = 0; y < elements - x - 1; y++) {
			if (arr[y] > arr[y + 1]) {
				swap(arr[y], arr[y + 1]);
			}
		}
	}
}

#ifdef MAIN_1
int main() {

	std::string com_port = "\\\\.\\COM3";
	DWORD COM_BAUD_RATE = 19200;
	SimpleSerial Serial((char*)com_port.c_str(), COM_BAUD_RATE);

	bool shouldPoll = true;
	if (Serial.connected_) {
		while (shouldPoll) {
			std::vector<std::string> p = parseStrings(Serial.ReadSerialPort(0.05, "json"), ",");
			if (p.size() == 4) {
				std::vector<int> values;
				values.push_back(abs(stoi(p[0])));
				values.push_back(abs(stoi(p[1])));
				values.push_back(abs(stoi(p[2])));
				values.push_back(abs(stoi(p[3])));
				int predef_minimum = 28900;
				long predef_maximum = 115125;
				int predef_maxthreshold = 300;
				int predef_threshold = 28750;
				long maximum = values[0] + values[1] + values[2] + values[3] - predef_maximum;
				if (maximum > predef_maxthreshold) {
					std::cout << "Value 1: " << values[0] - predef_threshold << " | Value 2: " << values[1] - predef_threshold << " | Value 3: " << values[2] - predef_threshold << " | Value 4: " << values[3] - predef_threshold << " | Maximum: " << maximum << std::endl;
					int sort[4] = { values[0],values[1],values[2],values[3] };
					bubbleSort(sort, 4);
					int maxValue = sort[3] - predef_threshold;
					double percentage[4] = { ((double)values[0]-predef_threshold) / (double)maxValue,((double)values[1] - predef_threshold) / (double)maxValue,((double)values[2] - predef_threshold) / (double)maxValue,((double)values[3] - predef_threshold) / (double)maxValue };
					std::cout << "Percentages: " << percentage[0] << " | " << percentage[1] << " | " << percentage[2] << " | " << percentage[3] << std::endl;
				}

			}
		}
	}

	return 0;
}

#endif
