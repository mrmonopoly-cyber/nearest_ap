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
> Linux is only used for emulation and testing of the library

| Target    | Documentation                         |
|-----------|---------------------------------------|
| Linux     | [README](target/linux/README.md)      |
| Crazyflie | [README](target/crazyflie/README.md)  |

# Documentation

To generate all the docs of the project run:
```sh
make all_doc
```

This will generate:

- a [doxygen](https://www.doxygen.nl/) documentation in ```doc/doxygen```
- a [latex](https://www.latex-project.org/) report in ```doc/report``` with name **report.pdf**

> To make the report [gnuplot](http://www.gnuplot.info/) is required
> To create the report you also need to prepare the data from the simulations. 
> To do that the simplest way is to unzip the data from a release into the **crazyflie_data_log**
> directory and then launch make. Alternative you can run the simulations in the linux target
> but that takes quite a lot of time due to a poor non parallel implementation of the simulator.

To clean the doc:
```sh
make clean
```

For helper and extra commands run:
```sh
make help
```
