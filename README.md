
# The INAETICS Demonstator Project [![Build Status](https://travis-ci.org/INAETICS/demonstrator.svg?branch=master)](https://travis-ci.org/INAETICS/demonstrator)

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

This branch contains an implementation of the demonstrator based on Celix bundles architecture. The Sample Queue Service expose services for storing in a queue samples, the Producer is able to track the Sample Queue Service and generate some dummy storing load. The Processor takes the load from the Queue and forwards it to a DataStore.

## Building and execution instructions

0.   Be sure to have installed cmake,apr,apr-util,zlib,curl and jansson libraries
1.   Download, compile and install Celix (sources can be checked out via git clone https://github.com/apache/celix.git) use the following cmake command to ensure that the required Apache Celix bundles are installed: `cmake -DBUILD_SHELL=ON -DBUILD_SHELL_TUI=ON -DBUILD_REMOTE_SERVICE_ADMIN=ON -DBUILD_RSA_DISCOVERY_CONFIGURED=ON -DBUILD_RSA_REMOTE_SERVICE_ADMIN_HTTP=ON -DBUILD_RSA_TOPOLOGY_MANAGER=ON -DCMAKE_INSTALL_PREFIX=<celix-install-dir> <celix-src-dir>`
2.   Checkout the demonstrator source code: git clone https://github.com/INAETICS/demonstrator.git
3.   Create a build folder mkdir demonstrator/build && cd demonstrator/build
4.   Start cmake with either: `cmake -DCELIX_DIR=<celix-install-dir>`  ..  or: `ccmake  -DCELIX_DIR=<celix-install-dir>` .. to configure the project via the interactive menu
5.   make all
6.   make deploy
7.  cd deploy/inaetics-demonstrator
8.  rm -rf .cache && sh run.sh. Celix Framework will be started ( a shell for controlling the bundles, the sample_queue, the producer and the processor) and you should see the Producer storing samples, the Processor retrieving samples and the Sample Queue Service logging all the operations. Equivalent deployment using remote services can be found within the according deploy folders inaetics-demonstrator-processor, inaetics-demonstrator-producer and inaetics-demonstrator-queue.

