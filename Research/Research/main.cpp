#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <libbladeRF.h>

using namespace std;

#define SELECT_LEN 4

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

void acquireData(bladerf **bf)
{
	cout << "Verification: " << &*bf << endl;

	bladerf_enable_module(*bf, BLADERF_MODULE_RX, true);
	bladerf_enable_module(*bf, BLADERF_MODULE_TX, false);
	bladerf_set_tuning_mode(*bf, BLADERF_TUNING_MODE_FPGA);
	bladerf_set_sampling(*bf, BLADERF_SAMPLING_INTERNAL);
	unsigned int specifyFrequency;
	while (true)
	{
		cout << "Specify frequency: ";
		cin >> specifyFrequency;
		if (specifyFrequency >= 300000000u && specifyFrequency <= 3800000000)
		{
			bladerf_set_frequency(*bf, BLADERF_MODULE_RX, specifyFrequency);
			break;
		}
		else
		{
			cout << "Invalid frequency!" << endl;
		}
	}
	// For now, we will set the sampling rate to be exactly 24
	unsigned int samplingRate = 20000000u;
	bladerf_set_sample_rate(*bf, BLADERF_MODULE_RX, samplingRate, NULL);
	unsigned int bandwidth = samplingRate / 2;
	bladerf_set_bandwidth(*bf, BLADERF_MODULE_RX, bandwidth, NULL);

	// Setting up the interface to actually receive the data
	bladerf_sync_config(*bf, BLADERF_MODULE_RX, BLADERF_FORMAT_SC16_Q11, 16, 8192, 8, 3500);
	bool completion = false;

	// Code partially borrowed from Nuand for the purpose of understanding the process


	bladerf_enable_module(*bf, BLADERF_MODULE_RX, false);
	return;
}

void createOutputTxt(bladerf **bf)
{
	ofstream ofile;
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
	vector <double> rawIQ;

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
				acquireData(bf);
				break;
			}
			case 1:
			{
				cout << 1 << endl;
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