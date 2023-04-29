#include <spng.h>
#include <inttypes.h>
#include <stdio.h>
#include <mpi.h>

const char *color_type_str(enum spng_color_type color_type)
{
    switch(color_type)
    {
        case SPNG_COLOR_TYPE_GRAYSCALE: return "grayscale";
        case SPNG_COLOR_TYPE_TRUECOLOR: return "truecolor";
        case SPNG_COLOR_TYPE_INDEXED: return "indexed color";
        case SPNG_COLOR_TYPE_GRAYSCALE_ALPHA: return "grayscale with alpha";
        case SPNG_COLOR_TYPE_TRUECOLOR_ALPHA: return "truecolor with alpha";
        default: return "(invalid)";
    }
}

int main(int argc, char** argv)
{
    char* IMG_PATH = "./yule.png";
    char* OUT_PATH = "./out.png";
    int ret = 0;

    FILE* png;
    spng_ctx* ctx = NULL;
    unsigned char* image = NULL;
    png = fopen(IMG_PATH, "rb");

    ctx = spng_ctx_new(0);

    /* Ignore and don't calculate chunk CRC's */
    spng_set_crc_action(ctx, SPNG_CRC_USE, SPNG_CRC_USE);

    /* Set memory usage limits for storing standard and unknown chunks,
       this is important when reading untrusted files! */
    size_t limit = 1024 * 1024 * 64;
    spng_set_chunk_limits(ctx, limit, limit);

    /* Set source PNG */
    spng_set_png_file(ctx, png); /* or _buffer(), _stream() */

    struct spng_ihdr ihdr;
    spng_get_ihdr(ctx, &ihdr);

    const char *color_name = color_type_str(ihdr.color_type);

    printf("width: %u\n"
           "height: %u\n"
           "bit depth: %u\n"
           "color type: %u - %s\n",
           ihdr.width, ihdr.height, ihdr.bit_depth, ihdr.color_type, color_name);

    printf("compression method: %u\n"
           "filter method: %u\n"
           "interlace method: %u\n",
           ihdr.compression_method, ihdr.filter_method, ihdr.interlace_method);

    struct spng_plte plte = {0};
    spng_get_plte(ctx, &plte);

    printf("palette entries: %u\n", plte.n_entries);

    size_t image_size, image_width;

    int fmt = SPNG_FMT_PNG;

    spng_decoded_image_size(ctx, fmt, &image_size);

    image = malloc(image_size);

    ret = spng_decode_image(ctx, image, image_size, SPNG_FMT_RGBA8, 0);
	
    
    
    /*YOU CAN TOUCH HERE.*/
    int numproc;
    int miproc;

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &miproc); // rango del proceso
    MPI_Comm_size(MPI_COMM_WORLD, &numproc); // cant. de procesos

    image_width = image_size / ihdr.height;
    printf("Image size: %lu\n", image_size);
    
    /* Algoritmo */

    int step = image_size/numproc;
    step = (step / 4) * 4;
    int chunk_start = step*miproc;
    int chunk_end = step * (miproc + 1);
    if (miproc == numproc - 1) {
        chunk_end = image_size;
    }
    for (int i = chunk_start; i < chunk_end; i += 4) {
        image[i] = 239.0/255.0 + (255.0 - 239.0)*(image[i] + image[i + 1] + image[i + 2]) / 3.0;
        image[i + 1] = 239.0/255.0;
        image[i + 2] = 143.0/255.0;
    }
    printf("Process %d started at %d and ended before %d. (Range %d)\n", miproc, chunk_start, chunk_end, chunk_end-chunk_start);

    unsigned int chunk_size = image_size - step*(numproc-1);
    unsigned char* chunk = malloc(chunk_size);

    if (miproc == 0){
        for (int core = 1; core < numproc; core+=1){
            MPI_Recv(chunk, chunk_size, MPI_CHAR, core, 0, MPI_COMM_WORLD, &status);
            chunk_start = step*core;
            chunk_end = step * (core + 1);
            if (core == numproc - 1) {
                chunk_end = image_size;
            }
            for (int i = chunk_start; i < chunk_end; i+=1){
                image[i] = chunk[i-chunk_start];
            }
        }

    }
    else{

        for (int i = chunk_start; i < chunk_end; i += 1) {
            chunk[i-chunk_start]=image[i];
        }
        MPI_Send(chunk, chunk_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }



    /* Cómo modifico mi imagen
       - reducir el rojo * 0.2
       los demás a 0
       verde: i + 1
       azul: i + 2
       Cómo lo hago compatible con mpi
    */


    /*Single-threaded algorithm*/
    
    /*for (int i = 0; i < image_size; i+=4) {
        image[i] = ((float)image[i] * (float)image[i]) / 255.0;
        image[i+1] = ((float)image[i+1] * (float)image[i+1]) / 255.0;
        image[i+2] = ((float)image[i+2] * (float)image[i+2]) / 255.0;
    }
    */
    printf("Successful quadratic inversion.\n");
    /*YOU CAN NO LONGER TOUCH HERE.*/
    if(miproc==0){
        FILE* outpng;
        spng_ctx* outctx = NULL;
        outpng = fopen(OUT_PATH, "wb");
        outctx = spng_ctx_new(SPNG_CTX_ENCODER);
        ret = spng_set_png_file(outctx,outpng);
        ret = spng_set_ihdr(outctx, &ihdr);
        ret = spng_encode_image(outctx, image, image_size, SPNG_FMT_PNG, 0);
        ret = spng_encode_chunks(outctx);
    }
    MPI_Finalize();
    return 0;
}
