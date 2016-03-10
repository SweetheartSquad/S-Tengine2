#pragma once

#include <vector>
#include <stack>

#include <glm\glm.hpp>

namespace sweet{

	// Can be used to randomly retrieve items from a list, never retrieving the same one twice until the whole list has been used
	// Think of it like a deck of cards: you start with all of the cards in order, shuffle them into a deck, deal out the deck in the order (which is now random), then re-shuffle when you run out of cards to deal
	template <class T>
	class ShuffleVector{
	private:
		// list of currently unavailable items
		std::vector<T> used;
		// list of currently available items
		std::stack<T> unused;
		// the number of times that the ShuffleVector has been shuffled
		// i.e. how many times the full list of items has been used
		unsigned long int shuffles;
		
	public:
		// adds _item to the list of unavailable items
		// NOTE: if the vector already has available items, the new one won't be available until they've all been used up
		void push(T _item){
			used.push_back(_item);
		};
		// adds _items to the list of available items
		// NOTE: if the vector already has available items, the new ones won't be available until they've all been used up
		void push(std::vector<T> _items){
			for(T i : _items){
				unused.push(i);
			}
		};

		// returns the total number of items in the shuffle vector
		unsigned long int size() const{
			return unused.size() + used.size();
		}

		// returns the next available item
		// calls shuffle when there aren't any available items
		// if _remove is true, the item is not inserted into the used list (i.e. it is removed from the shuffle vector)
		T pop(bool _remove = false){
			// if there aren't any available items, shuffle
			if(unused.size() <= 0){
				shuffle();
			}
			T res = unused.top();
			unused.pop();
			if(!_remove){
				used.push_back(res);
			}
			return res;
		};

		// randomly inserts all of the unavailable items into the available items list
		// this is called automatically when there aren't any available items
		void shuffle(){
			while(used.size() > 0){
				unsigned long int idx = sweet::NumberUtils::randomInt(0, used.size()-1);
				unused.push(used.at(idx));
				used.erase(used.begin() + idx);
			}
			++shuffles;
		};

		// returns a whole shuffled iteration
		std::vector<T> getShuffledItems(){
			std::vector<T> items;
			if(size() > 0){
				shuffle();
				while(unused.size() > 0){
					items.push_back(pop());
				}
			}
			return items;
		}

		// pops items until every currently available item is used
		// useful for when you want to "reset" the vector, letting it pick randomly from the entire list again
		void clearAvailable(){
			while(unused.size() > 0){
				pop();
			}
		}

		unsigned long int shuffleCount(){
			return shuffles;
		}

		ShuffleVector<T>() :
			shuffles(0)
		{};
	};

	class NumberUtils{
	public:
		// number of calls to randomInt() since the last call to seed()
		static unsigned long int numRandCalls;

		static float pingPong(float _val, float _min, float _max);

		// returns a random number in the range _min < res < _max
		static float randomFloat(float _min, float _max);
		// returns a random number in the range 0 < res < 1
		static float randomFloat();

		// returns a random number in the range _min <= res <= _max
		static int randomInt(int _min, int _max);
		// returns a random number in the range 0 <= res <= INT_MAX/2
		static int randomInt();
		// returns true or false, based on a randomly generated int
		static bool randomBool();

		// returns a random vec3 where each dimension is in the range _min <= _max
		static glm::vec3 randomVec3(glm::vec3 _min, glm::vec3 _max);

		// returns a random item in _items
		// done by generating a random int between 0 and _items.size()-1 and returning _items.at(i)
		template<class T>
		static T randomItem(std::vector<T> _items){
			return _items.at(randomInt(0, _items.size()-1));
		}

		// seeds the random number generator
		static void seed(unsigned long int _seed);

		/* Perlin Noise port */
		static double pNoise(double _x, double _y, double _z);
	private:
		static long int p[512];
		static double fade(double _t);
		static double lerp(double _t, double _a, double _b);
		static double grad(int _hash, double _x, double _y, double _z);
	};
}