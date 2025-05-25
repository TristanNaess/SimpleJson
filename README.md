# SimpleJson

SimpleJson is an attempt at building an easy to use JSON library for C++ with syntax ispiration from the standard containers

This is a work in progress and mostly a portfolio piece (though I may use it for other projects myself).  
Use in the real world is not recommended

## Design

The goal is to remove the need for specifying the type of the data at access, since all data types in JSON are unambiguous.  
From what I have written, this is achievable through the use of overloaded assignment operator and casting operators to read and (over)write.  
Insertion will also need to be overloaded to take all types, but that is through the normal overloaded argument types.  
Deletion is already type agnostic.  

The library uses exceptions if the type requested is incorrect or a function is invalid. While a little cumbersome, I'm pretty sure this is necessary for the type-erasure I'm attempting, as using templates require the types be known at compile time, and subclassing can't remove parent functions as far as I know.  

Internally the library stores the JSON as a string and parses the data as necessary. This is definitely not the fastest, but  
a) optimizes for space, rather than using some sort of tree and  
b) is part of the specification for the original database project I based this on.

Sub-objects and arrays are accessed through a accessor class that performs the string manipulation on the data and handles most of the data conversion.  
It shouldn't be directly accessable by the end user and only acts as a translation wrapper around the data to allow the desired syntax.

After I get this working, I will probably try to furthere optimize runtime vs storage for specific use cases.

### Future Additions
I don't want to replace the string based representation with parsing at request, because of the afore-mentioned project this is based on, but think parsing the active json data into a structure would be more efficient if the same data is accessed multiple times.
In this case I will probably change the class names to reflect what kind of storage is used, and include a macro to select the version with `-D VERSION_FLAG` if json::json is used
If possible I'd like to be able to use move constructors to serialize and deserialize the data to/from a record type that can be stored more efficiently, at the cost of parsing at construction

## Syntax

This is all the desired syntax, I haven't run into any problems with this yet, but is subject to change

### Read

Data is read through a combination of the subscript operator and casting. The subscript operator accesses fields and elements of objects and arrays, and the data is then converted to the expected type by casting or an error is thrown.  
`int id = data["id number"];`  
`std::string lastName = data["name"][1];`  

### Write

Writing data uses the subscript operator and assignment operator. I'm leaning toward not allowing simple writing to change the type of a field, and requiring deletion and insertion to make those changes for safety.  
`data["salary"] = 120000;`  
`data["name"][1] = "Smithe";`  
`data["name"][0] = 42;` is probably not allowed because it changes the type from array to number  

### Insert

Insertion is (not yet added to header) done through an explicit insert() function. This can be called on either the json object directly for first level fields or after a subscript operator for nested fields.  
Arguments are `insert(key or index, value)`  
`data.insert("department", "IT");`  
`data.insert("hobbies", "["hiking", "wood working", "baking", "knitting"]");` This will definitely work as a string; I would like to remove the outer quotes if possible.  
`data.insert("sub-object", "{"field 1": "value 1", "field 2", false}");` Same with this  
`data["name"].insert(1, "Alfred");` inserts string at index 1 in array "name"  

### Delete

Deletion works the same as insertion  
`data.delete("sub-object");`  
`data["name"].delete(1);`

### Additional

There are other functions for getting information about the data both at top level or for sub-fields
`keys()` returns the keys of an object as a vector  
`contains(key)` checks if a key exists in the object  
`size()` returns the number of elements in an array  
`type()` returns the type of the data. This applies to the current level only. I decided it wasn't necessary to make a version that takes a key or value, when you can just get that field and then call `.type()`  

e.g.
```
if (data.type() != json::Type::Object) std::cerr << "Data is not an object";
for (const auto& k : data.keys())
{
    switch (data[k].type())
    {
        case Object:
            // etc
            break;
        case Array:
            // etc
            break;

            ...
    }
}
```
