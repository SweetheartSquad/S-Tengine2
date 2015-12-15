#pragma once

namespace sweet{

namespace sweet{
	class NumberUtils{
	public:
		static float pingPong(float _val, float _min, float _max);

		// returns a random number in the range _min <= res < _max
		static float randomFloat(float _min, float _max);
		// returns a random number in the range 0 to 1
		static float randomFloat();
		// returns a random number in the range _min <= res <= _max
		static int randomInt(int _min, int _max);
		// returns a random number in the range 0 to something really really high
		static int randomInt();


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