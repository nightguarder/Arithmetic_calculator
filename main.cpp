#include "header.h"
//Tento zdrojovy kod jsem vypracoval zcela samostatne bez cizi pomoci
//Neokopiroval jsem ani neopsal jsem cizi zdrojove kody
//Nikdo me pri vypracovani neradil
//Pokud nektery radek porusuje toto pravidlo je oznacen komentarem
//NENI MOJE TVORBA
//Poruseni techto pravidel se povazuje za podvod, ktery lze potrestat VYLOUCENIM ZE STUDIA
//Cyril Steger učo. 36830

//Start the program with 2 parameters: -number -"priklad.txt"
int main(int argc, char **argv){
    int pn,pm,user,temp1 = 0;
    int flag = 1;
    //clears screen before
    system("cls");
    //check for parameters before start
    check_param(argc,argv,&pn,&pm);
    while(flag){
        //Run arithmetic calc
        Arithmetic_Operation(argc,argv);
        //63
        return flag = 0;
    }
    return 0;
}