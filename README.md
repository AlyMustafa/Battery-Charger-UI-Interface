# Battery Charger UI Interface

This project implements the user interface (UI) for a **DC Rectifier/Battery Charger** system. The UI allows users to configure key parameters such as voltage, current, and power through a simple LCD display and button-based interface. The settings are stored in flash memory using EEPROM emulation, ensuring the retention of user-defined values even after power cycles.

## Features

- **Real-Time Display**: Displays parameters such as voltage, current, power, and more on the LCD.
- **User Input via Buttons**: Users can modify parameters by pressing physical buttons that correspond to digits.
- **Memory Emulation**: Configured values are stored in flash memory (EEPROM emulation) for persistence across power cycles.
- **Simple Navigation**: Users can scroll through the parameter list, select a parameter, and modify it easily.
- **Timeout Handling**: The system will reset if the user is inactive for a specified period, improving usability and error prevention.

## Functionality

1. **LCD Interface**: Displays the list of parameters, current values, and an input prompt for the user to enter new values.
2. **Parameter Modification**: Users can scroll through parameters and edit their values by pressing the corresponding numeric buttons.
3. **Value Storage**: After modifying a parameter, the user can confirm the change, and the new value will be saved to flash memory.
4. **Settings Retention**: On reboot, the system automatically loads the last saved settings from flash memory, ensuring persistence of user-configured values.


## Hardware
    - Microcontroller with at least 2K of flash memory.
    - LCD display (e.g., 16x2 or 20x4).
    - Push buttons for user input.

##Compiling and Uploading
    Use an IDE like STM32CubeIDE or Arduino IDE to compile the code and upload it to your microcontroller.

## Usage

1. **Power On**: Upon powering on, the system displays the parameters and their current values.
2. **Modify Parameters**: Navigate through the parameters using buttons, and input new values.
3. **Save Changes**: Confirm the entered value, which will be saved to memory and reflected on the LCD screen.
4. **Timeout Reset**: If the system detects inactivity for a set period, it will automatically reset and return to the main menu.
