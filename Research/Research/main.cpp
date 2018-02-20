#include <iostream>
#include <libbladeRF.h>

using namespace std;

int main()
{
	struct bladerf *dev = NULL;
	struct bladerf_devinfo dev_info;
	bladerf_init_devinfo(&dev_info);
	int status = bladerf_open_with_devinfo(&dev, &dev_info); 
	cout << "Is FPGA configured? ";
	cout << bladerf_is_fpga_configured(dev) << endl;
	bladerf_close(dev);
	system("PAUSE");
	return 0;
}