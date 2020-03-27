// C++17-compatibele lijst. Ontbrekende elementen: move en copy, iterator
#ifndef __LIJST_H
#define __LIJST_H

//
// speciale code voor debuggen
//
#define DEBUG

//
// speciale code voor iteratoren
//
#define ITERATOR

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
using std::endl;
using std::ofstream;
using std::ostream;
using std::string;

template <class T>
class Lijstknoop;
template <class T>
class Lijst;

template <class T>
using Lijstknoopptr = std::unique_ptr<Lijstknoop<T>>;
template <class T>
ostream& operator<<(ostream& os, const Lijst<T>& l);

template <class T>
class Lijst : private Lijstknoopptr<T>
{
public:
    // toekenning, constructoren
    // overname constructoren van unique_ptr

    using std::unique_ptr<Lijstknoop<T>>::operator=;
    using std::unique_ptr<Lijstknoop<T>>::swap;

    Lijst() = default;
    Lijst(const Lijst& andere);
    Lijst(Lijst&& andere);
    Lijst& operator=(const Lijst& andere);
    Lijst& operator=(Lijst&& andere);
    virtual ~Lijst() = default;

    // operaties
    // duplicaten zijn toegelaten.

    void voegToe(const T&);
    // geefaantal geeft het aantal keer dat de sleutel voorkomt.
    // gebruikt de zoekfunctie om door de lijst te lopen!
    // zonder argument: geef lengte lijst
    int geefAantal(const T&) const;
    int geefAantal() const;
    // verwijder verwijdert slechts het eerste exemplaar met de gegeven
    // T, en geeft geen fout als de T niet gevonden wordt.
    // gebruik de zoekfunctie om door de lijst te lopen!
    void verwijder(const T&);
    // verwijder eerste knoop.
    void verwijderEerste();    
    bool isClone(const Lijst<T>&) const;
    void applySocialDistancing(const T& N, const T& isolationKey);
    
    // uitschrijf- en tekenoperaties
    // dotformaat:

    void teken(const char* bestandsnaam) const;
    // uitschrijven: voor elke knoop de T-waarde, gescheiden door komma's
    friend ostream& operator<<<>(ostream& os, const Lijst& l);
    void schrijf(ostream& os) const;

    // iterator; gaat ervan uit dat alles const is
    class iterator
    {
    private:
        Lijstknoop<T>* lkptr;

    public:
        iterator(Lijstknoop<T>* lkptr = nullptr);

        const T& operator*() const;
        const iterator& operator++();
        bool operator==(const iterator& andere);
        bool operator!=(const iterator& andere);
    };

    iterator begin() const;
    iterator end() const;

protected:
    // zoek geeft een pointer naar de Lijst die de sleutelwaarde bevat,
    // en geeft een pointer naar de lege lijst op het einde als de sleutel niet
    // voorkomt.
    const Lijst* zoek(const T&) const;
    Lijst* zoek(const T&);
    // preconditie zoekgesorteerd: lijst is gesorteerd
    // teruggeefwaarde: wijst naar Lijst waar sleutel staat/zou moeten staan.
    Lijst<T>* zoekGesorteerd(const T& sleutel);
};

template <class T>
class Lijstknoop
{
    friend class Lijst<T>;
    friend ostream& operator<<<>(ostream& os, const Lijst<T>& l);

public:
    Lijst<T> volgend;
    Lijstknoop(const T&);
#ifdef DEBUG
    ~Lijstknoop();
#endif
protected:
    T sleutel;
#ifdef DEBUG
public:
    static bool controle(int gemaakt, int verwijderd);

protected:
    static int aantalGemaakt;
    static int aantalVerwijderd;
#endif
};

template <class T>
int Lijstknoop<T>::aantalGemaakt = 0;
template <class T>
int Lijstknoop<T>::aantalVerwijderd = 0;

////////////////////////////////////////////////////////////////////////////////

template <class T>
Lijst<T>::Lijst(const Lijst& andere)
{
    if (this == &andere)
    {
        return;
    }

    const Lijst* it_andere = &andere;
    Lijst* it_this = this;

    while (it_andere && *it_andere)
    {
        *it_this = std::make_unique<Lijstknoop<T>>((*it_andere)->sleutel);

        it_andere = &((*it_andere)->volgend);
        it_this = &((*it_this)->volgend);
    }
}

template <class T>
Lijst<T>::Lijst(Lijst&& andere) : std::unique_ptr<Lijstknoop<T>>{std::move(andere)}
{
}

template <class T>
Lijst<T>& Lijst<T>::operator=(const Lijst& andere)
{
    if (this != &andere)
    {
        Lijst temp{andere};
        swap(temp);
    }

    return (*this);
}

template <class T>
Lijst<T>& Lijst<T>::operator=(Lijst&& andere)
{
    // swap(andere); // of
    // this->reset(andere.release()); // of
    std::unique_ptr<Lijstknoop<T>>::operator=(std::move(andere));

    return (*this);
}

////////////////////////////////////////////////////////////////////////////////

template <class T>
Lijst<T>::iterator::iterator(Lijstknoop<T>* lkptr) : lkptr{lkptr}
{
}

template <class T>
const T& Lijst<T>::iterator::operator*() const
{
    return lkptr->sleutel;
}

template <class T>
const typename Lijst<T>::iterator& Lijst<T>::iterator::operator++()
{
    lkptr = (lkptr->volgend).get();

    return *this;
}

template <class T>
bool Lijst<T>::iterator::operator==(const iterator& andere)
{
    return (lkptr == andere.lkptr);
}

template <class T>
bool Lijst<T>::iterator::operator!=(const iterator& andere)
{
    return !(*this == andere);
}

template <class T>
typename Lijst<T>::iterator Lijst<T>::begin() const
{
    return iterator(this->get());
}

template <class T>
typename Lijst<T>::iterator Lijst<T>::end() const
{
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////

template <class T>
Lijstknoop<T>::Lijstknoop(const T& _sl) : sleutel(_sl)
{
    // std::cerr<<"Knoop met sleutel "<<sleutel<<" wordt gemaakt\n";
    aantalGemaakt++;
}
#ifdef DEBUG

template <class T>
Lijstknoop<T>::~Lijstknoop()
{
    // std::cerr<<"Knoop met sleutel "<<sleutel<<" wordt verwijderd\n";
    aantalVerwijderd++;
}
template <class T>
bool Lijstknoop<T>::controle(int gemaakt, int verwijderd)
{
    if (aantalGemaakt == gemaakt && aantalVerwijderd == verwijderd)
        return true;
    else
    {
        std::cerr << "Fout bij controle:\n";
        std::cerr << "Aantal gemaakte knopen   : " << aantalGemaakt << " (moet zijn: " << gemaakt << ")\n";
        std::cerr << "Aantal verwijderde knopen: " << aantalVerwijderd << " (moet zijn: " << verwijderd << ")\n";
        throw "Mislukte controle";
    };
};
#endif

template <class T>
ostream& operator<<(ostream& os, const Lijst<T>& l)
{
#ifdef ITERATOR
    for (auto&& sleutel : l)
        os << sleutel << ", ";
#else
    if (l.get())
    {
        os << l.get()->sleutel << ", ";
        os << l.get()->volgend;
    }
#endif
    return os;
}

template <class T>
void Lijst<T>::schrijf(ostream& os) const
{
#ifdef ITERATOR
    if (this->get() != 0)
    {
        os << this->get()->sleutel;
        std::for_each(++begin(), end(), [&](const T& sleutel) { os << " . " << sleutel; });
    }
#else
    {
        Lijstknoop<T>* kn = this->get();
        if (kn != 0)
        {
            os << kn->sleutel;
            kn = kn->volgend.get();
        };
        while (kn != 0)
        {
            os << " . " << kn->sleutel;
            kn = kn->volgend.get();
        };
    }
#endif
}

template <class T>
bool Lijst<T>::isClone(const Lijst<T>& ander) const
{
    const Lijst<T>*l1 = this, *l2 = &ander; //twee lopers
    while (*l1 && *l2 && (*l1)->sleutel == (*l2)->sleutel)
    {
        l1 = &((*l1)->volgend);
        l2 = &((*l2)->volgend);
    };
    return (!(*l1) && !(*l2));
};

template <class T>
const Lijst<T>* Lijst<T>::zoek(const T& sleutel) const
{
    const Lijst<T>* pl = this;
    while (*pl && pl->get()->sleutel != sleutel)
        pl = &(pl->get()->volgend);
    return pl;
}

template <class T>
int Lijst<T>::geefAantal(const T& sleutel) const
{
    int aantal = 0;
    const Lijst<T>* pl = this;
    while (*pl)
    {
        if (sleutel == (*pl)->sleutel)
            ++aantal;
        pl = &(pl->get()->volgend);
    };
    return aantal;
};

template <class T>
int Lijst<T>::geefAantal() const
{
    int aantal = 0;
    const Lijst<T>* pl = this;
    while (*pl)
    {
        ++aantal;
        pl = &(pl->get()->volgend);
    };
    return aantal;
};

template <class T>
Lijst<T>* Lijst<T>::zoek(const T& sleutel)
{
    Lijst* pl = this;
    while (*pl && pl->get()->sleutel != sleutel)
        pl = &(pl->get()->volgend);
    return pl;
}

template <class T>
void Lijst<T>::voegToe(const T& sleutel)
{
    Lijstknoopptr<T> nieuw = std::make_unique<Lijstknoop<T>>(sleutel);
    Lijstknoopptr<T>::swap(nieuw->volgend);
    *this = std::move(nieuw);
}

template <class T>
void Lijst<T>::verwijderEerste()
{
    if (this->get() != 0)
    {
        Lijstknoopptr<T> staart(std::move(this->get()->volgend));
        this->reset();
        Lijstknoopptr<T>::swap(staart);
    }
}

template <class T>
void Lijst<T>::verwijder(const T& sleutel)
{
    zoek(sleutel)->verwijderEerste();
}

template <class T>
Lijst<T>* Lijst<T>::zoekGesorteerd(const T& sleutel)
{
    Lijst* plaats = this;
    while (*plaats && plaats->get()->sleutel < sleutel)
        plaats = &plaats->get()->volgend;
    return plaats;
};

template <class T>
void Lijst<T>::teken(const char* bestandsnaam) const
{
    ofstream uit(bestandsnaam);
    assert(uit);
    uit << "digraph {\nrankdir=\"LR\";\n\"0\"[label=\"\",shape=diamond];\n\"0\" -> \"1\";\n";
    int knoopteller = 1; // knopen moeten een eigen nummer krijgen.
    const Lijst<T>* loper = this;
    while (*loper)
    {
        uit << "subgraph cluster_" << knoopteller << " {\nrankdir=\"LR\";\n";
        uit << "\"" << knoopteller << "\" [label=\"" << (*loper)->sleutel << "\",color=white];\n";
        uit << "\"" << knoopteller << "v\" [shape=diamond,label=\"\"];\n";
        uit << "\"" << knoopteller << "\" -> \"" << knoopteller << "v\" [color=white];\n";

        uit << "}\n";
        uit << "\"" << knoopteller << "v\" -> \"" << knoopteller + 1 << "\" [lhead=cluster_" << knoopteller
            << " ltail=cluster_" << knoopteller + 1 << "];\n";
        loper = &((*loper)->volgend);
        knoopteller++;
    };
    uit << "\"" << knoopteller << "\" [shape=point];\n";
    uit << "}";
};

// calculates the overlay (int) between two intervals of min/max values
int getOverlap(int x_min, int x_max, int y_min, int y_max) {
    if(y_min < x_min && x_min < y_max) {
        return y_max - x_min + 1;
    } else if (x_min < y_min && y_min < x_max) {
        return x_max - y_min + 1;
    }
    return 0;
}

template <class T>
void Lijst<T>::applySocialDistancing(const T& N, const T& isolationKey) {
    int prevMin, prevMax, min, max, correctMin, correctMax;

    // sublistStart will always point to the start of the current sublist
    Lijst* sublistStart = this;
    // sublistEnd will always point to the end of the current sublist
    Lijst* sublistEnd;
    // runner will iterate over all elements after sublistStart to find 
    // the longest possible list as long as the conditions are met
    Lijst* runner;
    // adding isolation elements is done in one iteration through the list elements are always added before a sublist 
    // so isolation elements can only be added starting from sublist 2, the purpose of this counter is exactly this
    int sublistCount = 1;

    // iterate over the linked list until the end of the list
    while (*sublistStart) {
        // keep track of the min/max values in the current sublist for 2 reasons: 
        // 1) check if the condition (max-min <= N) is met
        // 2) calculate the overlap to add the appropriate amount of isolation elements
        min = max = sublistStart->get()->sleutel;
        // set the runner at the start of the sublist
        runner = sublistStart;

        // iterate over the possible sublist until there are no more nodes or until the condition fails
        while(*runner && max-min <= N) {
            // the min/max can change in this while loop without it being a valid min/max according to the condition (max-min <= N)
            // correctMin/correctMax is used to make sure that the last correct min/max values are used after this while loop 
            correctMin = min;
            correctMax = max;

            // check if the key of the current node (runner) is a new min or max
            if(min > runner->get()->sleutel) {
                min = runner->get()->sleutel;
            }
            if(max < runner->get()->sleutel) {
                max = runner->get()->sleutel;
            }

            // set sublistEnd to this current runner if the condition is still met
            if(max-min <= N) {
                sublistEnd = &runner->get()->volgend;
            }

            // get the next node
            runner = &runner->get()->volgend;
        }
        // keep the last correct min/max as explained above
        min = correctMin;
        max = correctMax;

        // isolation elements are added before every sublist starting from sublist 2
        if(sublistCount++ > 1) {
            // get the overlap based on the min/max of the previous sublist and the min/max of the current sublist
            int overlap = getOverlap(prevMin, prevMax, min, max);
            // add 1 isolation element if there is no overlay
            overlap = (overlap == 0) ? 1 : overlap;

            // add isolation elements according to the overlap between the previous and current sublist
            while(overlap-- > 0) {
                // add elements in front of the current sublist (between the previous and current sublist)
                (*sublistStart).voegToe(isolationKey);
            }
        }

        // keep track of the min/max values of the current sublist that will be the previous sublist in the next iteration
        prevMin = min;
        prevMax = max;

        // sublistEnd is always pointing to one element after the last, this is equal to the first element of the next sublist
        sublistStart = sublistEnd;
    };
}
#endif
