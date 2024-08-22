# SimpleJson
SimpleJson is an attempt at building an easy to use JSON library for C++ with syntax ispiration from the standard containers

This is a work in progress and mostly a portfolio piece (though I may use it for other projects myself).
Use in the real world is not recommended

Using cast and assignment operators, it seems possible to use the syntax:
Read:
std::string name = json.get("name");

Write:
json.get("name") = "John Deere";

Experiments seemed to work, see advanced casting repo
