# DAQLiveReadout
GUI for plotting serial data from 8-channel ADC and iMet XQ2

Author: Isaac Rowe (isaac.rowe@uky.edu)

This program is intended for use with an [iMet XQ2 UAV Sensor](http://www.intermetsystems.com/products/imet-xq2-uav-sensor) and [Digi XBee3 PRO 2.4 GHz Zigbee Radio](https://www.digi.com/products/embedded-systems/rf-modules/2-4-ghz-modules/xbee3-zigbee-3), although any two devices capable of UART serial communication should work. The ADC referenced in the code is the [Mayhew Labs 16-bit 8 Channel ADC Arduino Shield](http://mayhewlabs.com/products/extended-adc-shield). The data coming from these inputs, along with the meterological data from the iMet, is concatenated into one string for transmission over serial (in this case the XBee radio) by the [iMet Serial Write](https://github.com/ukyuav/iMetSerialWrite) program. 
This application connects to a serial port, then plots and records the above data in real time.

## Quick Start:

### Windows Installation
The final executable and support files for Windows are provided with this repository. Copy the "release" folder to your local machine. Run "terminal.exe" from this folder to launch the program.

### Other Platforms
Download [QT Creator](https://www.qt.io/download) and import the source files from this repository. Set up the project with the target options matching your desired platform and compile. This method has been tested with MacOS Mojave using the "Desktop Qt 5.12.2 clang 64bit" profile. Consider submitting an issue to the repository or contacting the author for assistance in comiling for another platform.

### Usage
1. After launching, select the gear icon to open the settings dialog.
![](/images/Screenshots/MenuIcons.png)
2. Select the serial port for which you want to monitor for data. 
  - On Windows this will be some COM[X] port where [X] is the port number corresponding with the connected device. 
  - On MacOS, this will look like /dev/cu.usbserial-[X].
3. Configure your desired serial communication options. For the intended application (recieving ADC and iMet data via an XBee3 radio), the default settings below should suffice.

![](/images/Screenshots/SerialOptions.png)

4. Select the desitnation to record the serial transmissions. The filename is automatically created as a timestamp, but can be edited from the text field. To change the folder location, select "Browse for File Location" and indicate the desired folder. The absolute path in the text box should update to reflect this.

![](/images/Screenshots/FileNameSelection.png)

5. Select "Apply" to save these settings.
6. Select the "Connect" icon to start monitoring the port and begin recieving data.
  - The raw data should be printed on the left side of the application. 
  - To see how this data is structured, see the [README](https://github.com/ukyuav/iMetSerialWrite/blob/master/README.MD) for the [iMet Serial Write repository](https://github.com/ukyuav/iMetSerialWrite).
7. To stop recieving, click the disconnect icon. The broom icon can also be used to clear settings for a new session.

### The Plots

 The center plot has 8 line series representing the 8 voltage inputs into the onboard ADC. They are reported in volts, with the range of +/- 10 volts.
 The right plot, which can be broken out into its own window, has 4 line series derived from the iMet sensor. 
  - The blue series is Humidity, reported as a ratio on the first vertical axis on the left side.
 - By default the red series is for Pressure, which is reported in hPa on the second vertical axis (left side).
 - The green series is for Temperature, reported in degrees Celsius on the right vertical axis.
 - The yello series is Temperature adjusted for humidity, also reported in degrees Celsius on the right vertical axis.
