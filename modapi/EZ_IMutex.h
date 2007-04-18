#ifndef __IMUTEX_H__
# define __IMUTEX_H__

class   EZ_IMutex
{
	public :
        virtual         ~EZ_IMutex(){}
        virtual bool    lock(void) = 0;         // used to lock a code section... true if ok, false otherwise
        virtual bool    unlock(void) = 0;       // used to unlock a code section... true if ok, false otherwise
        virtual bool    trylock(void) = 0;      // guess...
};

#endif /* __IMUTEX_H__ */