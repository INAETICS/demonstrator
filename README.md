
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

