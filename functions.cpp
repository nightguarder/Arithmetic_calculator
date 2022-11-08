#include "error.h"
//initial check if the parameters entered when executing the program is correct
int erorr_handle(int argc, char** argv){   
    if (argc < 1){
      printf("Too few arguments entered.\n");
      exit(0);
    }
    
    if (argc =='q')
    {
      printf("Exit the program...\n");
      exit(0);
    }
    if (!isdigit(argc))
    {
      printf("Expected a number!\n");
      exit(0);
    }
    
    //program is working only with 2 parameters passed
    else if( argc == 2 ){
      printf("Expected ./main.exe number -filename('priklad.txt'):\n");
      printf("You only passed: %s %s \n",argv[0],argv[1]);
      exit(0);
    }
    //too many? exit
    else if( argc > 3 )
      exit(0);
    //program not found
    else if(argv[0] == NULL)
      exit(0);
    //no paramater passed
    else if(argv[1] == NULL)
      exit(0);
    //other errors
    else 
      printf("-------------\t A problem has occured... \t-------------\n");
      printf("-------------\t Exiting the program now...\t-------------\n");
      exit(0);
    
    return 0;
}
//Check for invalid parameters passed
void check_param(int argc, char* argv[], int *pr, int *pr2){
    char *p = 0;
    errno = 0;
    int n = 0;
    //convert string to int &p = modified end pointer 10 = radix number 
    int temp = strtol(argv[1],&p,10);
    //no parameters passed
    if (argc == NULL)
      erorr_handle(argc,argv);
    //program only needs 2 parameters .\program -number -filename
    //if (argv[1]==NULL)
      //erorr_handle(argc,argv); 
    //if parameter passed is invalid or larger than INT_MAX
    else if (errno != 0 || *p != '\0'|| temp>INT_MAX || temp<INT_MIN)
      erorr_handle(n,argv);
    else{
      printf("parameters passed: %s, %s\n",argv[1],argv[2]);
      printf("-------------\t VALID ENTRY starting now... \t-------------\n");
    }
}

//Prints out custom error message
void error(const char err[]){
    printf("\n");
    fprintf(stderr, err);
    printf("\n Exiting the program now..\n");
    exit(0);
}