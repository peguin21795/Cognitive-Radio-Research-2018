# Cognitive Radio Research 2018
**About** <br />
This is where I will post all C++ code, MATLAB functions, and MATLAB scripts that were developed throughout my research with Dr. Payam Nayeri into cognitive radios, particularly in the area of energy-based spectrum sensing. We would like to acquire actual data for different modulation techniques through Nuand's bladeRF x40, spectrum analyzer, and a filtenna, and compare theoretical values reported by multiple IEEE papers regarding probability of detection, false alarm, and missed detection. All files in the repository will be updated on a weekly basis.

**Instructions to run C++ code** <br />
Hopefully, I can provide a straightforward process after going through multiple different ways to program the bladeRF through Visual Studio 2017. You first need to follow these steps provided by Nuand before moving on. <br />
Once that has been done and you can successfully build bladeRF.sln, you must follow these steps in order to program the bladeRF in a separate Visual Studio project. <br />
1. Open up the project's properties page by right clicking on the solution and clicking Properties or using the hotkey combination ALT+ENTER. 
2. In C/C++ >> General, add ...\bladeRF\host\libraries\libbladeRF to Additional Include Directories
3. In Linker >> General, add ...\bladeRF\host\build\output\Debug to Additional Library Directories. It contains bladeRF.lib which is needed to successfully compile your program. Also be sure to add bladeRF.lib, bladeRF.dll, pthreadVC2.dll, and libusb-1.0.dll to your project!
4. Lastly, in Linker >> Input, add the following libraries to Additional Dependencies:
  - bladeRF.lib
  - C:\Program Files (x86)\pthreads-win32\lib\x64\pthreadVC2.lib
  - D:\libusb-1.0.21\MS64\dll\libusb-1.0.lib

**Verification** <br />
You can verify that your code works by copying this simple C++ code. 
```C++
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
```
