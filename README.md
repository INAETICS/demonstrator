
#The INAETICS Demonstator Project
The purpose of the demonstator project is to demonstrate the capabilities of an INAETICS system.

##Celix Bundles Implementation
This branch contains an implementantion of the demonstrator based on Celix bundles architecture. The Sample Queue Service expose services for storing in a queue samples, the Producer is able to track the Sample Queue Service and generate some dummy storing load. The Consumer is still not yet implemented (as well as the take/takeall functions of the service)

##Building and execution instructions

0.   Be sure to have installed cmake,apr,apr-util,zlib,curl and jansson libraries
1.   Download, compile and install Celix (sources can be checked out from  https://svn.apache.org/repos/asf/celix/trunk/. Building and configuring instructions are included.)
2.   Checkout the demonstrator source code from org.inaetics.demonstrator.api/src/org/inaetics/demonstrator/api/
3.   mkdir inaetics-demonstrator-src && mkdir inaetics-demonstrator-build
4.   Copy the whole content of org.inaetics.demonstrator.api/src/org/inaetics/demonstrator/api/ in inaetics-demonstrator-src (bundles directory, CMakeLists.txt and deploy.cmake)
5.   In inaetics-demonstrator-src/CMakeLists.txt change properly the Celix Installation Directory ( set(CELIX_DIR "<celix_install_dir>") clause, the directory is the same as specified in step 1 )
6.   cd inaetics-demonstrator-build
7.   ccmake ../inaetics-demonstrator-src and configure properly the project via the interactive menu
8.   make all
9.   make deploy
10.  cd deploy/inaetics-demonstrator
11.  rm -rf .cache && chmod +x run.sh && ./run.sh. Celix Framework will be started ( a shell for controlling the bundles, the sample_queue and the producer) and you should see the Producer storing samples and the Sample Queue Service logging all the put/putAll operations.

##Known Shortcommings
The Celix Demonstrator implementation contains up to now only the Sample_Queue and the Producer. Memory in the Sample_Queue is never freed (will be freed when take/takeAll call will be implemented and used by the Consumer).
