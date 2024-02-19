// test.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <ormapi.h>

oBaseMemoryMap myBase;

class myobj : public oObject
{
public:
    int i = 81;
public:
    myobj() : oObject(&myBase) { i = 33; }

    size_t serialize(visitor& serializer);
    size_t deserialize(visitor& serializer);
};

size_t myobj::serialize(visitor& serializer)
{
    serializer.visit(i);
    return 1;
}

size_t myobj::deserialize(visitor& deserializer)
{ 
    deserializer.visit(i);
    return 1;
}

int main()
{
    {
        orm_ptr<myobj> o(new myobj());
        orm_ptr<myobj> oo = o;
        orm_ptr<myobj> ooo = std::move(oo);

        o =new myobj();
        oo = o;
        ooo = std::move(oo);

        o.store();
    }
    

    std::cout << "testing orm_ptr\n";
}
