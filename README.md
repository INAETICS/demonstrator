
# The INAETICS Demonstator Project

The purpose of the demonstator project is to demonstrate the capabilities of an
INAETICS system.

## This Branch of the Demonstrator
The purpose of this branch is to demonstate a Queue implementation with a distributed database. With this distributed database in Open Splice the Queue can be re-allocated or restarted after a crash without loosing the data in the Queue.

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
7.  Start seperate terminals to run the Producer, the opensplice Queue and the Processor
    a) cd deploy/inaetics-demonstrator-producer
    b) cd deploy/inaetics-demonstrator-opensplice-queue
    c) cd deploy/inaetics-demonstrator-processor
8.  Start the Producer, Queue and Processor in the terminals with:
rm -rf .cache && sh run.sh. Celix Framework will be started and you should see the Producer storing samples, the Processor retrieving samples and the Sample Queue Service logging all the operations. 
9.  For the demonstration of the distributed database, the queue can be stopped and started again. However to remain the data in the queue when the queue is stopped an other program must have domain participant for the same domain. For this purpose the program dds_daemon is delivered in this branch (demonstrator/dds_daemon). To build and start dds_daemon:
    a) cd demonstrator/dds_daemon/queue
    b) . ./release.com
    c) cd bld
    d) gmake all
    e) cd ../exe
    f) ./dds_daemon
The application will start a connect to the domain 2147483647 (default domain)

## Shortcomings
This demonstration will only run as described above. It is not integrated yet in the demonstrator-cluster, where the bundles are executed on seperate nodes / virtual boxes.
