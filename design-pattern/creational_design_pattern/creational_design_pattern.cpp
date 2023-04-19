//============================================================================
// Name        : creational_design_pattern.cpp
// Author      : Jotirling Swami
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

// *** creational_design_patter ***
//		These design patterns are all about class instantiation or object creation.
//		These patterns can be further categorized into Class-creational patterns and object-creational patterns.
//		While class-creation patterns use inheritance effectively in the instantiation process, object-creation patterns use delegation effectively to get the job done.
//
//	creational design patterns are the below:
//        1. Factory Method
//        2. Abstract Factory,
//        3. Builder,
//        4. Singleton
//        5. Prototype
//        6. Object Pool
//
//	Use case of creational design pattern-
//	1. Data Base Connection :
//		Suppose a developer wants to create a simple DBConnection class to connect to a database.
//		Alos wants to access the database at multiple locations from code.
//		Generally create an DBConnection class and use it for doing database operations wherever required.
//		This results in creating multiple connections from the database as each instance of DBConnection class will have a separate connection to the database.
//		To avoid multiple Database instance.
//		We create DBConnection class as a singleton class, so that only one instance of DBConnection is created and a single connection is established.
//		Because we can manage DB Connection via one instance, we can control load balance, unnecessary connections, etc.
//
//	2. Factory case :
//		Suppose you want to create multiple instances of a similar kind and want to achieve loose coupling then you can go for Factory pattern.
//		A class implementing factory design pattern works as a bridge between multiple classes. Consider an example of using multiple database servers like SQL Server and Oracle. If you are developing an application using SQL Server database as back end, but in the future need to change the database to the oracle.
//		You will need to modify all your code. so as factory design patterns maintain loose coupling and easy implementation,
//		we should go for the factory design pattern in order to achieve loose coupling and the creation of a similar kind of object.

