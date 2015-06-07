//
//  Random.cpp
//  kge
//
//  Created by Kevin on 14-8-4.
//  Copyright (c) 2014年 Kevin. All rights reserved.
//

#include "random.h"
#include <algorithm>
#include <ctime>

namespace ora
{

    namespace
    {
        const uint32 mag01[2]       = {0x0UL, 0x9908b0dfUL};
        const uint32 UPPER_MASK     = 0x80000000UL;
        const uint32 LOWER_MASK     = 0x7fffffffUL;
    }

	IMPLEMENT_SINGLETON(Random);

	Random::Random()
	{	
		// Get the current time:
		time_t t;
		time(&t);

		// Add to the current time a static which keeps increasing every time that
		// this is called.  This means that if two Random's are created in the
		// same second then they will be seeded differently.
		static uint32 s = 0;
		uint32 t32 = (uint32)t + s++;
	
		init(t32);
	}

	Random::~Random()
	{
		fini();
	}


	/**
	 *	This function seeds the Random using a uint32.
	 *
	 *	@param seed			The random seed.
	 */
	bool Random::init(uint32 seed)
	{
		mt[0] = seed;
		for (mti = 1; mti < 624; ++mti) 
		{
			// See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. 
			mt[mti] = 
				(1812433253UL*(mt[mti - 1] ^ (mt[mti - 1] >> 30)) + mti);              
		}
		mti = 624 + 1;

		return true;
	}


	/**
	 *	This function seeds the Random.
	 *
	 *	@param seed			Pointer to an array of seeds.
	 *	@param size			The size of the array.
	 */
	bool Random::init(uint32 const *seed, size_t size)
	{
		init(19650218);
		int i = 1; 
		int j = 0;
		int k = (624 > (int)size ? 624 : (int)size);
		for (; k; k--) 
		{
			mt[i] 
				= (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 30)) * 1664525UL))
					+ seed[j] + j; /* non linear */
			++i; ++j;
			if (i >= 624) 
			{ 
				mt[0] = mt[624 - 1]; 
				i = 1; 
			}
			if (j >= (int)size) 
				j = 0;
		}
		for (k = 624 - 1; k; --k) 
		{
			mt[i] 
				= (mt[i] ^ ((mt[i-1] ^ (mt[i - 1] >> 30)) * 1566083941UL))
					- i; /* non linear */
			++i;
			if (i >= 624) 
			{ 
				mt[0] = mt[624 - 1]; 
				i = 1; 
			}
		}
		mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */ 
		mti = 624 + 1;

		return true;
	}

	void Random::fini()
	{}

	/**
	 *	This generates a random uint32 value.
	 *
	 *	@returns			A random 32-bit value.
	 */
	uint32 Random::generate()
	{
		uint32 y;

		if (mti >= 624) 
		{
			int kk;

			for (kk=0; kk < 624 - 397; ++kk) 
			{
				y = (mt[kk] & UPPER_MASK)|(mt[kk+1] & LOWER_MASK);
				mt[kk] = mt[kk + 397] ^ (y >> 1) ^ mag01[y & 0x1UL];
			}
			for (; kk < 624 - 1; ++kk) 
			{
				y = (mt[kk] & UPPER_MASK)|(mt[kk + 1] & LOWER_MASK);
				mt[kk] = mt[kk + (397 - 624)] ^ (y >> 1) ^ mag01[y & 0x1UL];
			}
			y = (mt[624 - 1] & UPPER_MASK)|(mt[0] & LOWER_MASK);
			mt[624 - 1] = mt[397 - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];
			mti = 0;
		}
  
		y = mt[mti++];

		/* Tempering */
		y ^= (y >> 11);
		y ^= (y <<  7) & 0x9d2c5680UL;
		y ^= (y << 15) & 0xefc60000UL;
		y ^= (y >> 18);

		return y;
	}
	
	/**
	 *	This generates a random number in the range [min, max).
	 *
	 *	@param min          Minimum value in range.
	 *	@param max          Maximum value in range.
	 *	@returns            A random number in the range [min, max).
	 */
	int Random::generate(int min, int max)
	{
		if (min == max)
			return min;
		if (min > max) 
			std::swap(min, max);
		uint32 r = generate();
		return min + r%(max - min);
	}


	/**
	 *	This generates a random number in the range [min, max).
	 *
	 *	@param min          Minimum value in range.
	 *	@param max          Maximum value in range.
	 *	@returns            A random number in the range [min, max).
	 */
	float Random::generate(float min, float max)
	{
		if (min == max)
			return min;
		if (min > max) 
			std::swap(min, max);
		float r = (float)(generate()/4294967295.0); //2^32 - 1
		return min + r*(max - min);
	}


} // end of namespace ora

int ora_randomi()
{
    return int(ora::Random::instance()->generate());
}

float ora_randomf()
{
    return ora::Random::instance()->generate(0.0f, 1.0f);
}
