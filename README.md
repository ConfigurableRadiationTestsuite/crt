# General Information

This Configurable Raditation Testsuite was designed for testing (multiple) 
devices under irradiation. 

It was made it the Qt5 framework and it presents 
different component groups in the various tabs. E.g. the first tab is all the 
power supplies in the test-setup, here the voltage and current can be set and 
the supply switched on/off either manually or automatically.

# Documentation

The user manual can be found in the `doc` folder, it explains the manual use as 
well as the automated creation of configuration files.

# Configurations

In the `configs` folder are several configurations from practical radiation 
test and also a few example configurations. Additionally a python script 
`LBJ_to_CRT.py` can be used to generate a labjack configuration from a 
`*.csv` file derived from the `Labjack_Setup.xlsx`.

# Build

## Install Debian / Ubuntu packages

```
sudo apt install qt5-qmake qt5base-dev liblxi-dev libtirpc-dev
```

## Labjack library

To install the Labjack library download the 
[LJM Software](https://labjack.com/support/software/installers/ljm).

```
wget https://labjack.com/sites/default/files/software/labjack_ljm_software_2019_07_16_x86_64.tar.gz
tar -xf labjack_ljm_software_2019_07_16_x86_64.tar.gz
cd labjack_ljm_software_2019_07_16_x86_64
sudo ./labjack_ljm_installer.run
```

## Build

Instead of using `qtcreator`, you can use `qmake` manually:

```
qmake
make
```

# Run

```
./configuable-radiation-testsuite
```
