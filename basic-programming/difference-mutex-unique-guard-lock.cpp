// Difference between mutex lock and unique-guard-lock

/*
    The characteristics of std::lock_guard are:

    1. It holds the lock from the time of construction until the time of destruction.
    2. When it is created, it tries to take control of the mutex which contains resources it is provided.
    3. When this control is over, the std::lock_guard will destroy and mutex frees.
    4.It is defined under the header file mutex.h.

    The characteristics of std::unique_lock are:

    1.It does not lock as soon as it is created and unlocks at any time within the time it exists.
    2.It is similar to std::lock_guard but provides many methods for locking, unlocking and removing locks.
    3.This transfers lock ownership from one instance of the resource to another.
    4.It is defined under the header file mutex.h.

    Difference between std::lock_guard and std::unique_lock
    1. The programmer does not have the ability to unlock the std::lock_guard but he can unlock the std::unique_lock.

    2. The object creation in std::lock_guard and std::unique_lock is as follows:

        std::lock_guard lock1(resource); // resource is the resouce to lock and lock1 is the object
        std::unique_lock lock2(resource); // resource is the resource to lock and lock2 is the object

    3. The constructor of the object lock1 locks the resource and when the object scope is over, the object destructor unlocks the mutex.

    4. The constructor of the object lock2 locks the resource and the object destructor unlocks the mutex.
    The programmer has the ability to unlock the resources with the lock object in the case of std::unique_lock.

*/
