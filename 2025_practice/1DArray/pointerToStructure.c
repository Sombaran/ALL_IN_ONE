#include <stdio.h>




typedef struct   {
    /* data */
    int mData;

}myDataStruct;


int main (int argc, char** argv) {

    myDataStruct arrayOfStructs[3] = { {10}, {20}, {30} };
    myDataStruct *ptrToStruct = arrayOfStructs;
    for (size_t i = 0; i < 3; i++) {
        printf("Struct %zu: mData = %d\n", i, (ptrToStruct + i)->mData);
    }

    myDataStruct obj;
    obj.mData = 50;
    printf("Object obj: mData = %d\n", obj.mData);

    typedef myDataStruct* myDataStructPtr;
    myDataStructPtr ptrToObj = &obj;
    printf("Pointer to obj: mData = %d\n", ptrToObj->mData);

    typedef myDataStruct* arrayOfMyDataStructs[5];
    arrayOfMyDataStructs arrayOfPtrsObj = arrayOfStructs;
    printf("Array of pointers to structs: First mData = %d\n", arrayOfPtrsObj[0]->mData);    
    return (0);
}

