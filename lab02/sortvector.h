#ifndef __SORTVECTOR
#define __SORTVECTOR
/**
 \class sortvector
 \brief is een klasse van sorteerbare vectoren
 Bovenop de vectoreigenschappen zijn er hulpfuncties om sorteervergelijkingen
 te vergemakkelijken.
*/
#include <iostream>
#include <random>
#include <algorithm>
using std::istream;
using std::ostream;
using std::move;
using std::swap;
#include <iomanip>   // voor setw
#include <cstdlib>   // voor rand - opletten!! 
#include <vector>
using std::vector;

template<class T>
class Sortvector : public vector<T>{
  public:

    /// \fn Constructor: het argument geeft de grootte aan
    /// bij constructie zal de tabel opgevuld worden met
    /// n verschillende Ts in random volgorde
    /// (zie hulplidfuncties)
    Sortvector(int);
    
    Sortvector(const Sortvector<T>& v) = delete;
    Sortvector<T>& operator=(const Sortvector<T>& v) = delete;
    Sortvector<T>& operator=(Sortvector<T>&& v) = delete;
    Sortvector(Sortvector<T>&& v) = delete;

    /// \fn vul_range vul vector met de waarden T(0)...T(size()-1) in volgorde
    void vul_range();
    void draai_om();
    void vul_omgekeerd();
    void shuffle();
    void vul_random_zonder_dubbels();
    void vul_random();
    
    bool is_gesorteerd() const;
    /// \fn is_range controleert of *this eruit ziet als het resultaat van vul_range(), d.w.z.
    /// dat, voor alle i, (*this)[i]==T(i);
    bool is_range() const;

    friend ostream& operator<<(ostream& os, const Sortvector<T>& s){
        s.schrijf(os);
        return os;
    }

  private: 
    void schrijf(ostream & os)const;
    
};

template <class T>
Sortvector<T>::Sortvector(int grootte) : std::vector<T>(grootte)
{
    if (grootte > 0)
    {
        vul_random();
    }
}

template <class T>
void Sortvector<T>::vul_range()
{
	int i = 0;
	std::generate(this->begin(), this->end(), [&i]() {
		return i++;
	});
}

template <class T>
void Sortvector<T>::draai_om()
{
	reverse(this->begin(), this->end());
}

template <class T>
void Sortvector<T>::vul_omgekeerd()
{
	int i = (*this).size()-1;
	std::generate(this->begin(), this->end(), [&i]() {
		return i--;
	});
}

template <class T>
void Sortvector<T>::shuffle()
{
//	auto rng = std::default_random_engine{};
//	std::shuffle(this->begin(), this->end(), rng);
//	OR
	std::random_shuffle(this->begin(), this->end());
}

template <class T>
void Sortvector<T>::vul_random_zonder_dubbels()
{
	this->vul_range();
	this->shuffle();
}

template <class T>
void Sortvector<T>::vul_random()
{
    if (this->empty())
    {
        return;
    }

    std::random_device rd;
    std::mt19937 eng{rd()};
    const auto max_value = (this->size() - 1);
    assert(max_value < std::numeric_limits<int>::max());
    std::uniform_int_distribution<int> dist{0, static_cast<int>(max_value)};

    std::generate(this->begin(), this->end(), [&dist, &rd]() { return dist(rd); });
}

template <class T>
bool Sortvector<T>::is_gesorteerd() const
{
	return std::is_sorted(this->begin(), this->end());
}

template <class T>
bool Sortvector<T>::is_range() const
{
//	int value = 0;
//	for(auto it = this->begin(); it != this->end(); ++it) {
//		if(*it != value++) {
//			return false;
//		}
//	}
//	return true;
//	OR
	for(int i = 0; i<this->size(); i++){
		if((*this)[i] != i) {
			return false;
		}
	}
	return true;
}

template <class T>
void Sortvector<T>::schrijf(ostream & os) const 
{
    for(auto&& t : *this){
        os<<t<<" ";
    }
    os<<"\n";
}
#endif
