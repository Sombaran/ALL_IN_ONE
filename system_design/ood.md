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
Consistency is the behaviour to showcase regularity.
There are two types of consistency:
- Eventual consistency
	- All Nodes would be updated at the SAME TIME even if there has to be non-availability
	- No stale Data Possible
- Strong consistency
	- All Nodes will EVENTUALLY be consistent
	- Can have Stale Data for very short time
	- System should also be available
- CAP theorem
	- Consistency
	- Availability
	- Partition Tolerance: Distributed Systems (Fault Tolerance)
	- Out of 3, Any Distributed System can have only 2
	- CP or AP Systems
	- Partition Tolerance:
	- No Single Point of Failure
	- Given By Distributed Systems
	- CP system
		- Consistency > non-availability
		- Strong Consistency
		- My system goes down for sometime, I am fine with it
		- No Stale Data
		- For eg. (Uber/Zomato/Flipkart/Amazon/Bank transaction)
		- Use case of ATM:
		- Transaction: 100 USD
		- 1000 USD ----> 900 USD
		- All ATM Must Show the updated balance
		- If in meantime, Bank App/ ATM is not up for that user its fine
		- No Stale Data to be Shown
	- AP system
		- Availability > Consistency
		- Eventual Consistency
		- My System MUST NOT Go Down
		- Stale Data
		- For eg. (Social media)
		- Use case of Social media
		- Homepage Service/News Feed: Always Available
		- Priority: Instagram app should be UP and Available for All Users
		- Eventually (TTL: 200-500 ms), you will see the updated posts
		- Stale Data/ Old Posts: Okay

```
4.  Load Balancing
```
- Helps to spread the traffic (incoming requests) across a cluster of servers to improve responsiveness and availability of a system
- LB also keeps track of the status of all resources (servers) while distributing requests (Heart beat)
- IF a Server is Not Available, LB will stop sending request/traffic to that server

Client -----  LB --------  Web Server - 1
				| 
				 --------  Web Server - 2
				|
				 --------  Web Server - 3

- For eg. (NginX Plus, AWS Elastic Load Balancer (ELB), Citrix ADC (Prev: NetApp), Google Cloud Load Balancer)
- Total Request: 100K
	- LB:
	- Web Server-1: 50K
    - Web Server-2: 50K
```
5.  Caching
6.  Data Partitioning
7.  CAP Theorem
8.  Choice of DB
9.  Reliability
10. Efficiency
11. Manageability
12. Resiliency







