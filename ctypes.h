struct ns__array_of_string 
{
    char **__ptr; // pointer to array of elements in memory
    int __size;   // number of elements pointed to 
    int __offset; // optional SOAP 1.1 array offset
};

typedef struct ns__array_of_string ns__array_string;

 
