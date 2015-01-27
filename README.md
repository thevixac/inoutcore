


Basic explanation:
  This is the C++ client side to my social graph. The idea was to keep all app specific code out of the core, so that I could build as many apps as I wanted using the same database layer. The important stuff in this project is NodeStoreAsync and the related async classes, which describe how to interact with the actual node store. The database looks like a huge collection of nodes. Nodes have metadata, and ties to other nodes, much how facebook works. Then on top of that is the concept of Phenodes, which is a higher level type consisting of one or more nodes, to represent real world objects like users and groups and opinion polls. 
  I used NodestoreAsync to implement a iOS chat and SMS app a while back. 
