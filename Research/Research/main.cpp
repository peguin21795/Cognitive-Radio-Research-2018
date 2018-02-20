#include <iostream>
#include <libbladeRF.h>

using namespace std;

int main()
{
	struct bladerf *dev = NULL;
	struct bladerf_devinfo dev_info;
	bladerf_init_devinfo(&dev_info);
	int status = bladerf_open_with_devinfo(&dev, &dev_info); 
	if (status != 0)
	{
		cout << "Error! Connection to the bladeRF failed." << endl;
		return -1;
	}
	else
	{
		bladerf_close(dev);
		return 0;
	}
}