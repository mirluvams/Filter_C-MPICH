# Applying a Sepia Filter using C, MPICH, Glade, Gtk and Python.

This project is divided into two parts. 

The first part involves applying a sepia filter to an image using C and MPICH, which is an implementation of the MPI (Message Passing Interface) standard for distributed memory computing.

The second part involves creating a GUI using Glade, a graphical user interface builder for Gtk. We use the Gtk library to load and manipulate the interface from the Python code.

## Requirements

To run this program, you need to have:

- A C compiler (such as mpicc)
- MPICH
- An input image file in png format
- Glade
- Python
- Python PyGObject package

## Usage

First, you need to compile the program using the following command:
`mpicc *.c -lz -lm -o example`

Then, you execute the program using the following command:
`mpiexec -n 8 ./example "yule.png" "yule.out.png"`

Where `<n>` is the number of processes to use. For example, in this case we're using 8 processes; `"yule.png"` is the input image path and `"yule.out.png"` is the output image path.

After doing that, you need to run the GUI using the following command:
`python3 readpng.py` which includes the function `builder.add_from_file("readpng.glade")` that parses the glade file containing a UI definition (refers to the layout and properties of user interface elements in a graphical user interface (GUI) application. This includes the positioning and sizing of widgets such as buttons, labels, and text boxes, as well as their visual appearance, such as font and color), and merges it with the current contents of builder.

## How it works

For the first part, the program works as follows:

1. The master process reads the input image file and divides it into equal-sized chunks of rows.
2. Each worker process applies the sepia filter to its chunk.
3. Each worker process sends its filtered chunk back to the master process.
4. The master process merges the filtered chunks and saves the result to the output image file.

The sepia filter algorithm used is saved in the `example.c` file.

For the second part, the program does:
1. 

## Limitations

- The input image file must be in png format.
- The output image file will also be in png format.

## Example
Here we have the input image on the left and the output image on the right:


<img src="/yule.png" alt="Input image" width="200" height="200"/> <img src="/out.png" alt="Output image" width="200" height="200"/>



## Credits

This program was created by [mirluvams](https://github.com/mirluvams) ([mirluvams@gmail.com](mailto:mirluvams@gmail.com)) for the High Performance Computing Course 2023-2, taught by [Dr. Victor de la Luz](https://github.com/itztli) at [*Escuela Nacional de Estudios Superiores*, campus Morelia](https://www.enesmorelia.unam.mx/), [UNAM](https://www.unam.mx/).
