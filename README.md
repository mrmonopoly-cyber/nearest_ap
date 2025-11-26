# Nearest AP

This library allows a storm of drones to find the most suited one to connect to a centralized
access point

# Structure

To allow a easier deployment in different hardware a target system is already prepared. 
This is just a simplification, if the user want to implement its own implementation for his hardware
he's free to do that. In the **src** directory there is the source code for the library.

# Target

Each target has its owm build and flash system and for this reason there is no common one.
Here a list of the current supported targets with links to documentation of each of them.
>> Linux is only used for emulation and testing of the library

| Target    | Documentation                         |
|-----------|---------------------------------------|
| Linux     | [README](target/linux/README.md)      |
| Crazyflie | [README](target/crazyflie/README.md)  |

