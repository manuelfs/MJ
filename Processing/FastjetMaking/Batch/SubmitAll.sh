#!/bin/bash

for DATASET in `cat datasetCSA.txt | grep -v "#"`; do
    
    echo " [DEBUG] Submitting " $DATASET 
    ./RunOneDataset.sh $DATASET  

    #sleep 10800 # wait for 3h before submitting jobs for next dataset 
    #sleep 7200 # wait for 2h before submitting jobs for next dataset 
done
