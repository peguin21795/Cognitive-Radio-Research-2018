#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <libbladeRF.h>

using namespace std;

#define SELECT_LEN 4

bool testBladeRF(bladerf *bf, bladerf_devinfo dbf)
{
	int status = bladerf_open_with_devinfo(&bf, &dbf);
	if (status != 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void menuBladeRF()
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
				cout << 0 << endl;
				break;
			}
			case 1:
			{
				cout << 1 << endl;
				break;
			}
			case 2:
			{
				cout << 2 << endl;
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

void acquireData()
{
	return;
}

void createOutputTxt()
{
	ofstream ofile;
	return;
}

void modifySettings()
{
	return;
}

int main()
{
	// Testing the bladeRF x40 to make sure that it is successfully connected to the host PC.
	struct bladerf *dev = NULL;
	struct bladerf_devinfo dev_info;
	int EXIT_CODE;

	bladerf_init_devinfo(&dev_info);
	
	if (testBladeRF(dev, dev_info))
	{
		menuBladeRF();
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