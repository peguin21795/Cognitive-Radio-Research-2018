#include <iostream>
#include <cstdint>
#include <iterator>
#include <vector>
#include <fstream>
#include <string>
#include <libbladeRF.h>

using namespace std;

#define SELECT_LEN 4

struct IQ
{
	int16_t inPhase;
	int16_t quadrature;
};

void printIQ(IQ iq)
{
	cout << "(" << iq.inPhase << ", " << iq.quadrature << ")" << endl;
}

bool testBladeRF(bladerf **bf, bladerf_devinfo &dbf)
{
	int status = bladerf_open_with_devinfo(bf, &dbf);
	if (status != 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

vector <IQ> acquireData(bladerf **bf)
{
	cout << "Verification: " << &*bf << endl;
	vector <IQ> rawIQ;
	bladerf_enable_module(*bf, BLADERF_MODULE_RX, true);
	bladerf_enable_module(*bf, BLADERF_MODULE_TX, false);
	bladerf_set_sampling(*bf, BLADERF_SAMPLING_INTERNAL);
	bladerf_set_tuning_mode(*bf, BLADERF_TUNING_MODE_FPGA);
	unsigned int specifyFrequency;
	while (true)
	{
		cout << "Specify frequency (Enter -1 for default frequency value): ";
		cin >> specifyFrequency;
		if (specifyFrequency == -1)
		{
			bladerf_get_frequency(*bf, BLADERF_MODULE_RX, &specifyFrequency);
			break;
		}
		else if (specifyFrequency >= 300000000 && specifyFrequency <= 3800000000)
		{
			bladerf_set_frequency(*bf, BLADERF_MODULE_RX, specifyFrequency);
			break;
		}
		else
		{
			cout << "Invalid frequency!" << endl;
		}
	}
	cout << "Frequency: " << specifyFrequency << endl;
	// For now, we will set the sampling rate to be exactly 24
	unsigned int samplingRate = 30000000;
	bladerf_set_sample_rate(*bf, BLADERF_MODULE_RX, samplingRate, NULL);
	unsigned int bandwidth = samplingRate / 2;
	bladerf_set_bandwidth(*bf, BLADERF_MODULE_RX, bandwidth, NULL);

	// Setting up the interface to actually receive the data
	bladerf_sync_config(*bf, BLADERF_MODULE_RX, BLADERF_FORMAT_SC16_Q11, 16, 8192, 8, 1000);
	const int sampleLength = 10000;
	int16_t* rx_samples = new int16_t [2 * sampleLength];

	// Code partially borrowed from Nuand for the purpose of understanding the process
	bladerf_sync_rx(*bf, rx_samples, sampleLength, NULL, 1000);
	bladerf_enable_module(*bf, BLADERF_MODULE_RX, false);
	IQ tempIQ;

	for (int i = 0; i < 2 * sampleLength; i++)
	{
		tempIQ.inPhase = rx_samples[i];
		tempIQ.quadrature = rx_samples[i + 1];
		rawIQ.push_back(tempIQ);
	}
	delete[] rx_samples;
	return rawIQ;
}

void createOutputTxt(vector <IQ> rawIQ, bladerf** bf)
{
	if (rawIQ.size() == 0)
	{
		cerr << "Error: You must first acquire data from the bladeRF before proceeding with data generation!" << endl;
		return;
	}
	ofstream ofile;
	unsigned int frequencyUsed;

	ofile.open("rawIQ.txt");
	ofile << "Raw IQ sampling" << endl;
	for (int i = 0; i < rawIQ.size(); i++)
	{
		ofile << rawIQ[i].inPhase << " " << rawIQ[i].inPhase << "\n";
	}													
	cout << "File successfully created! All elements have been successfully erased" << endl;
	ofile.close();
	return;
}

void modifySettings(bladerf **bf)
{
	int gain;
	unsigned int frequency;
	bladerf_get_rxvga1(*bf, &gain);
	bladerf_get_frequency(*bf, BLADERF_MODULE_RX, &frequency);
	cout << "------------------------------------------------------------------------------" << endl;
	cout << "                    Current Properties Configuration                          " << endl;
	cout << "------------------------------------------------------------------------------" << endl;
	cout << "Gain: " << gain << endl;
	cout << "Frequency: " << frequency << " Hz" << endl;
	cout << endl;
	return;
}

void menuBladeRF(bladerf **bf)
{
	int input;
	bool quitStatus = false;
	vector <IQ> rawIQ;

	string SELECTION[SELECT_LEN] = {"Acquire Data through RX", "Create Output Text File", "Modify Settings", "Quit"};
	cout << "Make a selection" << endl;
	for (int i = 0; i < SELECT_LEN; i++)
	{
		cout << "(" << i << ") " << SELECTION[i] << endl;
	}
	while (true)
	{
		cout << "Selection: ";
		cin >> input;
		switch (input)
		{
			case 0:
			{
				rawIQ = acquireData(bf);
				break;
			}
			case 1:
			{
				createOutputTxt(rawIQ, bf);
				break;
			}
			case 2:
			{
				modifySettings(bf);
				break;
			}
			case 3:
			{
				cout << "Exiting..." << endl;
				quitStatus = true;
				break;
			}
			default:
			{
				cout << "Not a valid input!" << endl;
			}
		}
		if (quitStatus)
		{
			break;
		}
	}
	return;
}

int main()
{
	// Testing the bladeRF x40 to make sure that it is successfully connected to the host PC.
	struct bladerf *dev = NULL;
	struct bladerf_devinfo dev_info;
	int EXIT_CODE;

	bladerf_init_devinfo(&dev_info);
	
	if (testBladeRF(&dev, dev_info))
	{
		cout << &dev << endl;
		menuBladeRF(&dev);
		EXIT_CODE = 0;
	}
	else
	{
		cout << "Error! Connection to the bladeRF failed." << endl;
		EXIT_CODE = -1;
	}
	bladerf_close(dev);
	return EXIT_CODE;
}