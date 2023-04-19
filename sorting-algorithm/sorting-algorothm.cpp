// What is Sorting:

//     Sorting refers to the operation or technique of arranging and rearranging sets of data in some specific order.
//     A collection of records called a list where every record has one or more fields.
//     The fields which contain a unique value for each record is termed as the key field.
//     For example, a phone number directory can be thought of as a list where each record has three fields -
//     'name' of the person,
//     'address' of that person, and their
//     'phone numbers'. Being unique phone number can work as a key to locate any record in the list.

//     Sorting is the operation performed to arrange the records of a table or list
//     in some order according to some specific ordering criterion.
//     Sorting is performed according to some key value of each record.

//     The records are either sorted either numerically or alphanumerically.
//     The records are then arranged in ascending or descending order depending on the numerical value of the key.
//     Here is an example, where the sorting of a lists of marks obtained by a student in any particular subject of a class.

//     Categories of Sorting:
//     The techniques of sorting can be divided into two categories. These are:
//          1. Internal Sorting
//          2. External Sorting

//             Internal Sorting:
//                 If all the data that is to be sorted can be adjusted at a time in the main memory,
//                 the internal sorting method is being performed.

//             External Sorting:
//                 When the data that is to be sorted cannot be accommodated in the memory at the same time and
//                 some has to be kept in auxiliary memory such as hard disk, floppy disk, magnetic tapes etc,
//                 then external sorting methods are performed.

//     The Complexity of Sorting Algorithms:
//         The complexity of sorting algorithm calculates the running time of a function
//         in which 'n' number of items are to be sorted.
//         The selection of sorting method is suitable for a problem depends on several dependency configurations.
//         The most noteworthy of these considerations are:

//         1. The length of time spent by the programmer in programming a specific sorting program
//         2. Amount of machine time necessary for running the program
//         3. The amount of memory necessary for running the program

//     The Efficiency of Sorting Techniques:
//         To get the amount of time required to sort an array of 'n' elements by a particular method,
//         the normal approach is to analyze the method to find the number of comparisons (or exchanges) required by it.
//         Most of the sorting techniques are data sensitive, and
//         so the metrics for them depends on the order in which they appear in an input array.

//     Various sorting techniques are analyzed in various cases and named these cases as follows:
//         1. Best case
//         2. Worst case
//         3. Average case
//         Hnce, the result of these cases is often a formula giving the average time required for a particular sort of size 'n.'
//         Most of the sort methods have time requirements that range from O(nlog n) to O(n2).

//     Types of Sorting Techniques:
//         Bubble Sort
//         Selection Sort
//         Merge Sort
//         Insertion Sort
//         Quick Sort
//         Heap Sort