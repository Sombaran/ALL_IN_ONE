### Basics of system design

Software System interacting with different components

1. Load Balancer
2. Client
3. Server
4. DB
5. Cache
6. Web Server
7. Application Server

## HLD:
1. High Level Design
2. Overview of different components in a System and how they interact with each other


## LLD:
1. Low Level Design
2. Component Level Design
3. UML Diagrams: Class Diagrams, Use Case Diagram, Activity Diagram, Sequence Diagrams
4. Design Principles and Best Practised: SOLID, KISS, DRY, YAGNIK etc
5. Design Patterns: 23 (Gang of Four)

## Software Architecture/ SDLC:

1. Requirements
2. Solution/Design
3. High Level Design: Tech Lead/ Architect
4. LLD: Classes, Interfaces, Abstraction, Service Layer, Controller etc - CODE
5. Unit Testing/ Integration Testing
6. Deployment/ Release

## Key Characteristics of a Distributed System:

1.  Scalability
```
Scalability is the capability of a system, process or a network to grow and manage increased demands.

Need for scaling:
- Increased Demands:
- Increased Users
- Increased Requests
- Increased Data

There are 3 types of scaling:
- Horizontal scaling
	- Scale by adding more servers into pool of resources
	- Easier to scale dynamically by adding more machines, no downtime
	- Costly
	- Existing: S1 S2 S3
	- New: S1 S2 S3 S4 S5 S6 S7
	- For eg. (Apache Cassandra or MongoDB)
	Suppose a server capable of handling 65 K (5K internal ports) request, now at some point of time server receives 500 K requests adding load to the server, so in this case extra servers are needed.How many server needed? (500 K/ 60 K) 9 extra server are needed to meet the demand. 
- Vertical scaling
	- Scale by adding more power (CPU, Storage, RAM) to an Existing Server
	- Limited to capacity of a single server and involves downtime
	- Cheaper
	- For eg.(MySQL)
	"Scheduled Maintainance: Website will be down at 1 AM - 3 AM on 25th May"
- Auto scaling
	- Upscaling + Downscaling
	- Configuration
	- Memory/CPU/RAM Usage: Crosses 80% ----> Get Another Instance
	- Memory/CPU/RAM Usage: Reaches less than 10% ----> Turn Off That Instance
	- For eg. (Kibana, Grafana)

```
2.  Availability
```
Availability is the time a system remains operational to its request.
Usually measured in terms of 9
Ideally: 9 9s or 11 9s

90%
99% - 2 9s
99.99999 - 7 9s
.........Closer to 100% Available
Eg: Whatsapp

- Single Tick: Send from your End
- Double Tick: Received by Receiver
- Blue/Green Tick: Read (Read Receipts On)

Availability Example:
- Send a Message: (Single Tick)
- Opened any App: Works as expected
- Zomato: 
	- Homepage Service: Available
	- Cart Service: Available

```
3.  Consistency
```

```
4.  Load Balancing
5.  Caching
6.  Data Partitioning
7.  CAP Theorem
8.  Choice of DB
9.  Reliability
10. Efficiency
11. Manageability
12. Resiliency







