# COP4520-Raytracing

## Building the Project

⚠️ This project was coded using **ISO C++ 20 standard (/std:c++20)**, since it is the most stable at the moment. ⚠️

**You need to have CMake installed.**

In your terminal of choice, type the following command:

If you are using a single configuration generator (Ninja/Unix-Makefiles) you must specify the CMAKE_BUILD_TYPE. **This will most likely be the case.**

```
cmake -S . -B <build directory>/ -D CMAKE_BUILD_TYPE=Release 
cmake --build <build directory>/
```

---

For multi-configuration generators it's slightly different (Ninja Multi-Config, Visual Studio).

```
cmake -S . -B <build directory>
cmake --build <build directory> --config Release
```

A lot of people like to name the build directory `out/build` or `build`, but this is up to you.

### Running the Project
Inside of the *build directory* there are two ways you can run the program on a terminal depending on your operating system:

#### Windows

```
COP4520-Raytracing.exe > <nameOfImage>.ppm
```

#### Linux

```
./COP4520-Raytracing > <nameOfImage>.ppm
```

You can name the final output image however you like. The image will be located in the same directory as the output directory.

## Viewing the Output Image

You can view the final output [in this page](https://www.cs.rhodes.edu/welshc/COMP141_F16/ppmReader.html).
