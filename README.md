
# The INAETICS Demonstator Project

The purpose of the demonstator project is to demonstrate the capabilities of an
INAETICS system.

## Java implementation

To run this Java implementation, you need a recent Eclipse IDE along with the
latest Bndtools (currently 2.4) plugin.

There are four Bnd run files you should start from Eclipse (use Run As -&gt;
Bnd OSGi Run Launcher):

1. `datastore.bndrun` - starts the in-memory data store;
2. `queue.bndrun` - starts the in-memory, non-persistent, queue;
3. `processor1.bndrun` - starts a single sample processor;
4. `producer1.bndrun` - starts a single sample producer.

Occassionally, you see a "read timeout" exception occur in the console of the
processor, this is a small issue in the used Amdatu RS version.

The run configuration currently use the "configured" discovery from Amdatu RS,
but should run as well with another discovery mechanism, like Bonjour.

## Celix Bundles Implementation

This branch contains an implementation of the demonstrator based on Celix bundles architecture. The Sample Queue Service expose services for storing in a queue samples, the Producer is able to track the Sample Queue Service and generate some dummy storing load. The Processor takes the load from the Queue and forwards it to a DataStore (nyi).

## Building and execution instructions

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

