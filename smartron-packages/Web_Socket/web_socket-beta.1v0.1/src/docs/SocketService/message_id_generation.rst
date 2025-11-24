=====
Message ID generation scheme
=====
Message id will be used to identify request from different client for Web Socket and identify processed data from Device Manager. This should signify client and command type to be easily mapped and identified



Component
------------------------
- **Client** : client from where command (*length*: 2)
- **Command Type**: Single control type or multiple control type (*length*: 2)
- **Message Sequence number**: Sequence number of job for every message incremented by 1 and reset when exceed from 999999 (*length*: 6)
- **Part**: In case of multiple things control command part wil be assigned like P01, P02 else will be P00 (*length*: 3)

Notes:
-----------------------
1. Total length of message id will be 13
2. We can fetch client, command type, sequence number and part from message id
3. We can not fetch things detail from message id
4. message id must be assigned to every request received on listening port
5. every processing with message id must be logged to log file
6. incoming request will be matched with message id
7. after assigned message id to task, task should be put into Queue and worker will process it in async fashion
8. a timeout will be initialize for every message id in Queue


client code
------------------------
1. Socket Server: 'SS'
2. Mobile Agent: 'MA'
3. Voice Assistant: 'VA'

command type
----------------------------
1. Single device control: 'ST'
2. Multiple device control: 'MT'

Message Sequence & Part Range
---------------------------------
**Range:** 000001 - 999999
**Part Range:** P00 - P99



