
#The INAETICS Demonstator Project
The purpose of the demonstator project is to demonstrate the capabilities of an INAETICS system.

##Celix Bundles Implementation
This branch contains an implementation of the demonstrator based on Celix bundles architecture. The Sample Queue Service expose services for storing in a queue samples, the Producer is able to track the Sample Queue Service and generate some dummy storing load. The Processor takes the load from the Queue and forwards it to a DataStore (nyi).

##Building and execution instructions

0.   Be sure to have installed cmake,apr,apr-util,zlib,curl and jansson libraries
1.   Download, compile and install Celix (sources can be checked out from  https://svn.apache.org/repos/asf/celix/trunk/. Building and configuring instructions are included.)
2.   Checkout the demonstrator source code: git clone https://github.com/INAETICS/demonstrator.git
3.   Switch to the celix branch: gut checkout celix_bundles_impl 
4.   Create a build folder mkdir build && cd build 
5.   Start cmake with either: cmake -DCELIX_DIR=<celix installation folder>  ..  or: ccmake ..  -DCELIX_DIR=<celix installatin folder> to configure the project via the interactive menu
6.   make all
7.   make deploy
8.  cd deploy/inaetics-demonstrator
9.  rm -rf .cache && sh run.sh. Celix Framework will be started ( a shell for controlling the bundles, the sample_queue, the producer and the processor) and you should see the Producer storing samples, the Processor retrieving samples and the Sample Queue Service logging all the operations. Equivalent deployment using remote services can be found within the according deploy folders inaetics-demonstrator-processor, inaetics-demonstrator-producer and inaetics-demonstrator-queue.
