//
//  Random.h
//  kge
//
//  Created by Kevin on 14-8-4.
//  Copyright (c) 2014年 Kevin. All rights reserved.
//

#ifndef RANDOM_HPP
#define RANDOM_HPP

#include "util_config.h"
#include "singleton.h"

/**
 *	This class provides a random number generator based upon the Mersenne 
 *	twister:
 *
 *	http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
 *
 *	It is about one and a half-times faster than rand(), is more random,
 *	has a nicer interface (it is easier to set bounds on the random numbers)
 *	and can allow multiple random number generators at once.
 */
namespace ora{

	class Random //: public Singleton<Random> 有些全局变量初始化会依赖Random
	{
	public:
        static Random * instance()
        {
            static Random s_random;
            return &s_random;
        }

		Random();
		~Random();

		bool init(uint32 seed);
		bool init(uint32 const *seed, size_t size);

		void fini();
		
		uint32 generate();
		int generate(int min, int max);
		float generate(float min, float max);

	private:
		uint32                  mt[624]; // state vector
		int                     mti;
	};


} // end of namespace ora

int     ora_randomi();
float   ora_randomf();

#endif // RANDOM_HPP