/*
Assign 5
Name: Jeff Byerly
Zid: z1860060
Date: 11/18/2020
*/

#ifndef registerfile_H
#define registerfile_H



class registerfile
{
	private:
	       	int32_t rfArray[32];

	public:
		registerfile();
		

		void reset();
		void set(uint32_t r, int32_t val);
		int32_t get(uint32_t r) const;
		void dump() const;
};





#endif
