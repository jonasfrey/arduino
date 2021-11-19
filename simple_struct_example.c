#include <stdio.h>

struct Person{
    char *name;
    int age;
    char dob[10];
};

struct Student{
    struct Person person;
    int rollno;
    float marks[10];
};

struct Student student = {
{"Adam", 25, 20},
101,
90
};
// struct Point {
//     int x; 
//     int y; 
// };

// struct Object_with_point{
//     int numtest;
//     Point location_point;
// }

int main()
{

    // struct Object_with_point test = {0};
    
    printf(" student.rollno is :%i", student.rollno);
    printf(" student.person.name is : %s", student.person.name);

    student.person.name="lol"; 
    printf(" student.person.name is : %s", student.person.name);

    return 0;
}

// gcc simple_struct_example.c -o simple_struct_example && chmod +x simple_struct_example && ./simple_struct_example