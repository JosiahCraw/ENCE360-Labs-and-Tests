#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>


#define handle_error(msg) \
        do { perror(msg); exit(EXIT_FAILURE); } while (0)

size_t file_size(int fd) {
  struct stat sb;  
  if (fstat(fd, &sb) == -1) handle_error("fstat");
  
  return  sb.st_size;
}


int main(int argc, char *argv[])
{
  if(argc < 2) {
    fprintf(stderr, "usage: read_write <repeats>\n");
    exit(EXIT_FAILURE);
  }
  
  size_t repeats = atoi(argv[1]);
  
  for(int i = 0; i < repeats; ++i) {

    int src = open("test.dat", O_RDONLY);
    if (src == -1) handle_error("fstat");

    int dst = open("output.dat", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (dst == -1) handle_error("open");

    // get the file size  
    size_t size = file_size(src);
    
    // Need to truncate the file to the right size or mmap will fail
    if(ftruncate(dst, size)) handle_error("ftruncate");

    
    /*
     * TODO: Implement file copying using mmap.
     * 
     * First, mmap both the files to a region of memory.
     * Copy the contents between the memory regions using memcpy.
     * Use munmap to unmap the files and make sure the data is written
     * 
     * MAP_PRIVATE for reading the source  because we don't need to propogate writes to the file   
     * We need MAP_SHARED on the destination so that our writes are written back to the file
     * 
     */
    
    

    close(src);
    close(dst);
  }

  exit(EXIT_SUCCESS);
}