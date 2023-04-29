# Applying a Sepia Filter using C and MPICH

This project is about applying a sepia filter to a given image using C and MPICH. MPICH is an implementation of the MPI (Message Passing Interface) standard for distributed memory computing.

## Requirements

To run this program, you need to have:

- A C compiler (such as mpicc)
- MPICH
- An input image file in png format

## Usage

First, you need to compile the program using the following command:
`mpicc *.c -lz -lm -o example`

Then, you execute the program using the following command:
`mpiexec -n 8 ./example`

Where `<n>` is the number of processes to use. For example, in this case we're using 8 processes.

## How it works

The program works as follows:

1. The master process reads the input image file and divides it into equal-sized chunks of rows.
2. Each worker process applies the sepia filter to its chunk.
3. Each worker process sends its filtered chunk back to the master process.
4. The master process merges the filtered chunks and saves the result to the output image file.

The sepia filter algorithm used is saved in the `example.c` file.

## Limitations

- The input image file must be in png format.
- The output image file will also be in png format.

## Example
Here we have the input image on the left and the output image on the right:
<img src="/yule.png" alt="Input image" width="200" height="200"/> <img src="/out.png" alt="Output image" width="200" height="200"/>



## Credits

This program was created by [Miriam Valdez] for the [High Performance Computing Course] at [ENES, Morelia] taught by [Dr. Victor De la Luz].
