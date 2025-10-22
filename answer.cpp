// 1

#include <iostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;

class Person {
public:
    string name;
    weak_ptr<Person> mother;
    weak_ptr<Person> father;
    vector<shared_ptr<Person>> kids;

    Person (const string& n,
           shared_ptr<Person> m = nullptr,
           shared_ptr<Person> f = nullptr)
        : name(n), mother(m), father(f) {
    }

    ~Person() {
        cout << "delete " << name << endl;
    }
};

shared_ptr<Person> initFamily (const string& name)
{
    shared_ptr<Person> mom = make_shared<Person>(name+"'s mom");
    shared_ptr<Person> dad = make_shared<Person>(name+"'s dad");
    shared_ptr<Person> kid = make_shared<Person>(name,mom,dad);
    mom->kids.push_back(kid);
    dad->kids.push_back(kid);
    return kid;
}

int main()
{
    shared_ptr<Person> p = initFamily("nico");

    cout << "nico's family exists" << endl;
    cout << "- nico is shared " << p.use_count() << " times" << endl;
    if (auto m = p->mother.lock())  // weak_ptr -> shared_ptr
        cout << "- name of 1st kid of nico's mom: " << m->kids[0]->name << endl;

    p = initFamily("jim");
    cout << "jim's family exists" << endl;
}

//Здесь есть ошибка, у нас происходит циклическая зависимость. Т.е. родители и дети хранят shared_ptr
//друг на друга. Это создает циклическую ссылку, из-за которой счетчик ссылок никогда не будет равным 0.
//Решением этой проблемы было добавление weak_ptr. Получается, что родители владеют ребенком и тогда все будет работать.
//Можно было бы и наоборот сделать, чтобы дети владели родителями, но чисто логически это неверно, зато вывод был бы более правильным.
//Также вместо shared_ptr<Person> mom(new Person(name+"'s mom")); использую make_shared для более эффективного создания shared_ptr.
//Также добавил условие в котором явно сохраняю результат lock() в shared_ptr, чтобы не разыменовывать nullptr, если объект уже был удален.

