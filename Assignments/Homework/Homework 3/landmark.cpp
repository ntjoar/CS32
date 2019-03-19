//
//  main.cpp
//  landmark.cpp
//
//  Created by Nathan Tjoar on 2/6/19.
//  Copyright © 2019 Nathan Tjoar. All rights reserved.
//

#include <iostream>
#include <string>
using namespace std;

// Your declarations and implementations would go here
class Landmark
{
public:
    Landmark(string name): m_name(name) {}
    virtual ~Landmark();
    virtual string name() const;
    virtual string icon() const = 0;
    virtual string color() const; // Most colors yellow except for Hospital
private:
    string m_name; //string of the name
};

class Hotel : public Landmark
{
public:
    Hotel(string nm) : Landmark(nm) {}
    virtual ~Hotel();
    virtual string icon() const;
};

class Restaurant : public Landmark
{
public:
    Restaurant(string nm, int cap) : Landmark(nm), m_capacity(cap) {}
    virtual ~Restaurant();
    virtual string icon() const;
private:
    int m_capacity;
};

class Hospital : public Landmark
{
public:
    Hospital(string nm) : Landmark(nm) {}
    virtual ~Hospital();
    virtual string icon() const;
    virtual string color() const; //Color is blue
};

Landmark::~Landmark()
{}

Hotel::~Hotel()
{
    cout << "Destroying the hotel " << Landmark::name() << "." << endl;
}

Restaurant::~Restaurant()
{
    cout << "Destroying the restaurant " << Landmark::name() << "." << endl;
}

Hospital::~Hospital()
{
    cout << "Destroying the hospital " << Landmark::name() << "." << endl;
}

string Landmark::name() const
{
    return m_name;
}

string Landmark::color() const
{
    return "yellow";
}

string Hospital::color() const //Weirdo, hehe
{
    return "blue";
}

string Hotel::icon() const
{
    return "bed";
}

string Restaurant::icon() const
{
    if(m_capacity < 40)
        return "small knife/fork";
    return "large knife/fork";
}

string Hospital::icon() const
{
    return "H";
}

void display(const Landmark* lm)
{
    cout << "Display a " << lm->color() << " " << lm->icon() << " icon for "
    << lm->name() << "." << endl;
}

int main()
{
    Landmark* landmarks[4];
    landmarks[0] = new Hotel("Westwood Rest Good");
    // Restaurants have a name and seating capacity.  Restaurants with a
    // capacity under 40 have a small knife/fork icon; those with a capacity
    // 40 or over have a large knife/fork icon.
    landmarks[1] = new Restaurant("Bruin Bite", 30);
    landmarks[2] = new Restaurant("La Morsure de l'Ours", 100);
    landmarks[3] = new Hospital("UCLA Medical Center");

    cout << "Here are the landmarks." << endl;
    for (int k = 0; k < 4; k++)
        display(landmarks[k]);

    // Clean up the landmarks before exiting
    cout << "Cleaning up." << endl;
    for (int k = 0; k < 4; k++)
        delete landmarks[k];
}
