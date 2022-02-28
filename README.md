<h1 align="center">Image Processing and Path Detection</h1>

A scientific research project focused on characteristic extraction and route mapping for
an autonomous vehicle.

## Configuration

Some header files contain defines that can be changed to update how the code behaves in
compilation time.

### Cone Detection Algorithm

The images to be analysed must be stored in the `source` folder. Output
will contain a step-by-step transformation of the image, and will be saved in the
`destination` folder.

## Dependencies

This project is built using
- `C++17`
- `OpenCV 4`
- `CMake 3.18.4`

## Execution

CMake's configuration files can be created using `cmake -B build`. In the `build` folder,
use `make` to compile and `./main` to run the code.
