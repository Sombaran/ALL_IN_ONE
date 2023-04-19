
// Why array index starts from zero ?

//     Consider int arr[100]. The answer lies in the fact how the compiler interprets arr[i]
//         ( 0<=i<100).
//     arr[i] is interpreted as *(arr + i).
//     Now, arr is the address of the array or address of 0th index element of 
//         the array. So, address of next element in the array is arr + 1
//     (because elements in the array are stored in consecutive memory locations),
//         further address of next location is arr + 2 and so on.
//     Going with the above arguments, arr + i mean the address at i distance away
//         from the starting element of the array.
//     Therefore, going by this definition, i will be zero for the starting element
//     of the array because the starting element is at 0 distance away from
//     the starting element of the array. To fit this definition of arr[i],
//         indexing of array starts from 0.