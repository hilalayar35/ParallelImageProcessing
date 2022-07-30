#include <stdint.h>
#include <stdio.h>
#include <omp.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"
#define CHANNEL_NUM 1


int main(int argc,char* argv[]) {
    int width, height, bpp;
    int thread_count = strtol(argv[3], NULL, 10); // Number of processes
	int	my_rank = omp_get_thread_num(); //Process rank
   
	double startTime,endTime;
    			
 	int sum=0;
	
	// Reading the image
    uint8_t* rgb_image = stbi_load(argv[1], &width, &height, &bpp, CHANNEL_NUM);

	//Time Start
    startTime = omp_get_wtime();	
	
	//Print the width and height of the image
    printf("Width: %d  Height: %d \n",width,height);
	
	//Print the thread count
    printf("thread_count: %d \n",thread_count);
    
    
    #pragma omp parallel for schedule(guided,1000) num_threads(thread_count) \
    	shared(height,width,rgb_image) private(sum) 
    	
	  for(int k=1;k<height-1;k++){
	    for(int l=1;l<width-1;l++){

	     
	      sum = 0;
	      
	      for(int m=k-1;m<=k+1;m++){
		for(int n=l-1;n<=l+1;n++){

		   sum=sum+rgb_image[m*width + n];

		}
	      }
	      rgb_image[k*width+l]=sum/9;
	    }
	  }
    
    
    
    // Stoing the image 
    stbi_write_jpg(argv[2], width, height, CHANNEL_NUM, rgb_image, 100);
    stbi_image_free(rgb_image);
	
	//Time stop
    endTime = omp_get_wtime();
	
	//Print the measured time
    printf("1-Time: %f \n",endTime-startTime);
    

    return 0;
}
