
void tempo(unsigned char x){
    int i;
    for(i=0; i<500*x; i++){
        volatile unsigned char j, k;
        for(j=0; j<41; j++) {
            for(k=0; k<3; k++);
        }//end for
    }//end for
}//end void
    

    

